#include "Wire.h"

#define I2C_DEV_ADDR 0x3F // address of the display
#define DEBUGGING true

uint16_t data[4];                // data array
uint8_t index = 0;               // word array for i2c packets
uint8_t first_digit_offset = 52; // offset for i2c words where the first measurement beggins
char digits[8];                  // array for the digits

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

void setup()
{
    Serial.begin(115200);
    Wire.setClock(400000);
    Wire.onReceive(onReceive);
    Wire.begin(I2C_DEV_ADDR);
}

void loop() {}