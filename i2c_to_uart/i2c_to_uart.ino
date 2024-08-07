#include "Wire.h"

#define I2C_DEV_ADDR 0x3F  // address of the display
#define DEBUGGING false

const int power_pin = 10;    // pin for powering the device
const int measure_pin = 11;  // pin for measuring
const int buzzer_pin = 17;   // pin for toggling the buzzer

uint16_t data[4];                 // data array
uint8_t data_index = 0;           // word array for i2c packets
uint8_t first_digit_offset = 50;  // offset for i2c words where the first measurement beggins
char digits[8];                   // array for the digits

uint8_t command_byte = 0;  // command byte for the uart communication

inline void clearData() {
  for (int i = 0; i < 4; i++)
    data[i] = 0;
  memset(digits, 0, sizeof(digits));
}

void tableLookup() {
  uint8_t digit_i = 0;

  for (uint8_t i = 0; i < 4; i++) {
    switch (data[i]) {
      case 0x121E:
        digits[digit_i++] = '0';
        break;
      case 0x1A1E:
        digits[digit_i++] = '.';
        digits[digit_i++] = '0';
        break;
      case 0x0006:
        digits[digit_i++] = '1';
        break;
      case 0x0806:
        digits[digit_i++] = '.';
        digits[digit_i++] = '1';
        break;
      case 0x061C:
        digits[digit_i++] = '2';
        break;
      case 0x0E1C:
        digits[digit_i++] = '.';
        digits[digit_i++] = '2';
        break;
      case 0x041E:
        digits[digit_i++] = '3';
        break;
      case 0x0C1E:
        digits[digit_i++] = '.';
        digits[digit_i++] = '3';
        break;
      case 0x1406:
        digits[digit_i++] = '4';
        break;
      case 0x1C06:
        digits[digit_i++] = '.';
        digits[digit_i++] = '4';
        break;
      case 0x141A:
        digits[digit_i++] = '5';
        break;
      case 0x1C1A:
        digits[digit_i++] = '.';
        digits[digit_i++] = '5';
        break;
      case 0x161A:
        digits[digit_i++] = '6';
        break;
      case 0x1E1A:
        digits[digit_i++] = '.';
        digits[digit_i++] = '6';
        break;
      case 0x0016:
        digits[digit_i++] = '7';
        break;
      case 0x0816:
        digits[digit_i++] = '.';
        digits[digit_i++] = '7';
        break;
      case 0x161E:
        digits[digit_i++] = '8';
        break;
      case 0x1E1E:
        digits[digit_i++] = '.';
        digits[digit_i++] = '8';
        break;
      case 0x141E:
        digits[digit_i++] = '9';
        break;
      case 0x1C1E:
        digits[digit_i++] = '.';
        digits[digit_i++] = '9';
        break;
      default:
        Serial.println(data[i], HEX);
        digits[0] = '-';
        break;
    }
  }
}

void onReceive(int len) {
  while (Wire.available()) {
    uint16_t b = Wire.read();
    b = b << 8;
    b |= Wire.read();
    //Serial.print(data_index);
    //Serial.print(' ');
    //Serial.println(b, HEX);
    if (b == 0x80B0) {
      tableLookup();
      data_index = 0;
      if (digits[0] != 0)
        Serial.println(digits);
      clearData();
    } else {

      if (data_index >= first_digit_offset && data_index <= first_digit_offset + 7) {
        uint8_t abs_index = data_index - first_digit_offset;
        abs_index /= 2;
        if (data_index % 2 == 0) {
          //Serial.print(b, HEX);
          //Serial.print(" ");
          data[abs_index] = b << 8;
          //Serial.println(data[0], HEX);
        }

        else if (data_index % 2 == 1) {
          //Serial.print(b, HEX);
          //Serial.print(" ");
          b = b & 0x00FF;
          data[abs_index] += b;
          //Serial.print(b, HEX);
          //Serial.print(" ");
          //Serial.println(data[abs_index], HEX);
        }
      }
      data_index++;
    }



    /*
    if (data_index >= first_digit_offset && data_index <= first_digit_offset + 7) {

      //uint8_t temp_i = (data_index - first_digit_offset) / 2;
      if (data_index % 2 == 0){
        b = b & 0x00FF;
        b = b << 8;
        data[data_index] =  b;
        Serial.println(data[data_index], HEX);
        //delay(20000);
      }
      
      /*
      if (data_index % 2 == 0) {
        //data[temp_i] = b >> 8;
        //Serial.print(String(data_index) + " Par ");
        //Serial.println(b, HEX);
      } else {
        data[temp_i] |= b;
        //Serial.print(String(data_index) + " ImPar ");
        //Serial.println(b, HEX);
      }
    }*/
    b = 0;
  }
}

void stop_device() {
  digitalWrite(power_pin, HIGH);
  delay(5000);
  digitalWrite(power_pin, LOW);
}

void start_device() {
  digitalWrite(power_pin, HIGH);
  delay(1000);
  digitalWrite(power_pin, LOW);
}

void measure_distance() {
  digitalWrite(measure_pin, LOW);
  delay(200);
  digitalWrite(measure_pin, HIGH);
  delay(200);
  digitalWrite(measure_pin, LOW);
  delay(200);
  digitalWrite(measure_pin, HIGH);
}

void toggle_buzzer() {
  pinMode(buzzer_pin, OUTPUT);
  digitalWrite(buzzer_pin, LOW);
  delay(50);
  pinMode(buzzer_pin, INPUT_PULLUP);
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting capture");

  pinMode(power_pin, OUTPUT);
  digitalWrite(power_pin, LOW);
  pinMode(measure_pin, OUTPUT);
  pinMode(buzzer_pin, INPUT_PULLUP);

  Wire.setClock(400000);
  Wire.onReceive(onReceive);
  Wire.begin(I2C_DEV_ADDR);

  start_device();

  delay(2000);
  measure_distance();
}

void loop() {
  
  uint32_t time_now = micros();
  if (Serial.available())
    if (micros() - time_now > 500000) {
      command_byte = Serial.read();
      Serial.println(command_byte);
      if (command_byte == 'm') {
        measure_distance();
      } else if (command_byte == 't') {
        toggle_buzzer();
      } else if (command_byte == 's') {
        stop_device();
      } else if (command_byte == 'p') {
        stop_device();
      }
      time_now = micros();
    }
}