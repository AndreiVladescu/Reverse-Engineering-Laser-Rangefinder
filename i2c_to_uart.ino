#include "Wire.h"

#define I2C_DEV_ADDR 0x3F // address of the display
#define DEBUGGING true

const int power_pin = 15;   // pin for powering the device
const int measure_pin = 16; // pin for measuring
const int buzzer_pin = 17; // pin for toggling the buzzer

uint16_t data[4];                // data array
uint8_t index = 0;               // word array for i2c packets
uint8_t first_digit_offset = 52; // offset for i2c words where the first measurement beggins
char digits[8];                  // array for the digits

uint8_t command_byte = 0;       // command byte for the uart communication

inline void clearData()
{
    memset(data, 0, sizeof(data));
    memset(digits, 0, sizeof(digits));
}

void tableLookup()
{
    uint8_t digit_i = 0;

    for (uint8_t i = 0; i < 4; i++)
    {
        switch (data[i])
        {
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
            digits[0] = '-';
            break;
        }
    }
}

void onReceive(int len)
{
    while (Wire.available())
    {
        uint16_t b = Wire.read();
        b = b << 8;
        b |= Wire.read();

        if (b == 0x80B0)
        {
#ifdef DEBUGGING
            Serial.println();
#endif
            index = 0;
            Serial.println(digits);
            clearData();
        }
        else
            index++;

        if (index >= first_digit_offset && index <= first_digit_offset + 7)
        {
            uint8_t temp_i = (index - first_digit_offset) / 2;
            if (index % 2 == 0)
                data[temp_i] = b << 8;
            else
                data[temp_i] |= b;
        }

#ifdef DEBUGGING
        Serial.print(b, HEX);
#endif
        b = 0;
    }
#ifdef DEBUGGING
    Serial.println();
#endif
}

void stop_device()
{
    digitalWrite(power_pin, HIGH);
    delay(5000);
    digitalWrite(power_pin, LOW);
}

void start_device()
{
    digitalWrite(power_pin, HIGH);
    delay(100);
    digitalWrite(power_pin, LOW);
}

void measure_distance()
{
    digitalWrite(measure_pin, LOW);
    delay(50);
    digitalWrite(measure_pin, HIGH);
    delay(50);
    digitalWrite(measure_pin, LOW);
    delay(50);
    digitalWrite(measure_pin, HIGH);
}

void toggle_buzzer()
{
    pinMode(buzzer_pin, OUTPUT);
    digitalWrite(buzzer_pin, LOW);
    delay(50);
    pinMode(buzzer_pin, INPUT_PULLUP);
}

void setup()
{
    Serial.begin(115200);

    pinMode(power_pin, OUTPUT);
    digialWrite(power_pin, LOW);
    pinMode(measure_pin, OUTPUT);
    pinMode(buzzer_pin, INPUT_PULLUP);

    start_device();

    Wire.setClock(400000);
    Wire.onReceive(onReceive);
    Wire.begin(I2C_DEV_ADDR);
    delay(2000);
}

void loop() {

    uint32_t time_now = micros();

    if (micros() - time_now > 50000){
        command_byte = Serial.read();
        
        if (command_byte == 0x11){
            measure_distance();
        }
        else if (command_byte == 0x22){
            toggle_buzzer();
        }
        else if (command_byte == 0x33){
            stop_device();
        }
        else if (command_byte == 0x44){
            stop_device();
        }
        time_now = micros();
    }
    
}