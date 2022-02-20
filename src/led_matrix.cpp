#include <Arduino.h>

// set pin definition
int DIN_pin = 11;
int LOAD_pin = 3;
int CLOCK_pin = 13;

// MAX7219 register address
byte max7219_REG_noop = 0x00;
byte max7219_REG_decodeMode = 0x09;
byte max7219_REG_intensity = 0x0a;
byte max7219_REG_scanLimit = 0x0b;
byte max7219_REG_shutdown = 0x0c;
byte max7219_REG_displayTest = 0x0f;

// Digit Addresses
byte max7219_REG_digit0 = 0x01;
byte max7219_REG_digit1 = 0x02;
byte max7219_REG_digit2 = 0x03;
byte max7219_REG_digit3 = 0x04;
byte max7219_REG_digit4 = 0x05;
byte max7219_REG_digit5 = 0x06;
byte max7219_REG_digit6 = 0x07;
byte max7219_REG_digit7 = 0x08;

void SendByte(byte data)
{

  byte i = 8;
  byte mask;
  while (i > 0)
  {
    mask = 0x01 << (i - 1);

    digitalWrite(CLOCK_pin, LOW);
    if (data & mask)
    {
      digitalWrite(DIN_pin, HIGH);
    }
    else
    {
      digitalWrite(DIN_pin, LOW);
    }
    digitalWrite(CLOCK_pin, HIGH);
    --i;
  }
}

void init(byte reg_addr, byte reg_data)
{
  digitalWrite(LOAD_pin, LOW);
  for (int i = 0; i < 4; i++)
  {
    SendByte(reg_addr);
    SendByte(reg_data);
  }
  digitalWrite(LOAD_pin, HIGH);
}

void clear_screen()
{
  init(1, 0);
  init(2, 0);
  init(3, 0);
  init(4, 0);
  init(5, 0);
  init(6, 0);
  init(7, 0);
  init(8, 0);
}

void setup()
{
  Serial.begin(9600);
  Serial.print("Start\n");

  pinMode(DIN_pin, OUTPUT);
  pinMode(CLOCK_pin, OUTPUT);
  pinMode(LOAD_pin, OUTPUT);

  // Initialize MAX7219 registers
  init(max7219_REG_scanLimit, 0x07); // set to scan all row
  // Disable decoding
  init(max7219_REG_decodeMode, 0x00);
  // Not shutdown mode
  init(max7219_REG_shutdown, 0x01);
  // Not test mode
  init(max7219_REG_displayTest, 0x00);

  clear_screen();
}

u32 matrix_8X32[8] = {
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000};

uint32_t letter_v[8] = {
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00100100,
    0b00100100,
    0b00011000};

byte number_0[8] = {
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100};

byte number_1[8] = {
    0b0110000,
    0b11100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000};

byte number_2[8] = {
    0b01111100,
    0b10000100,
    0b10000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b0111110};

byte number_3[8] = {
    0b00000000,
    0b11110000,
    0b00001000,
    0b00001000,
    0b11110000,
    0b00001000,
    0b00001000,
    0b11110000};

byte number_4[8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000};

byte number_5[8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000};

byte number_6[8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000};

byte number_7[8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000};

byte number_8[8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000};

byte number_9[8] = {
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000};

void draw_matrix_8x32()
{
  for (int i = 1; i <= 8; i++)
  {
    digitalWrite(LOAD_pin, LOW);
    uint32_t mask;
    for (uint32_t j = 32; j > 0; j--)
    {

      if (j % 8 == 0)
      {
        SendByte(i);
      }

      mask = 0x80000000 >> (32 - j);

      digitalWrite(CLOCK_pin, LOW);
      if (mask & matrix_8X32[i - 1])
      {
        digitalWrite(DIN_pin, HIGH);
      }
      else
      {
        digitalWrite(DIN_pin, LOW);
      }
      digitalWrite(CLOCK_pin, HIGH);
    }
    digitalWrite(LOAD_pin, HIGH);
  }
}

void overlay(byte number, byte offset)
{
  uint32_t number_matrix[8];
  uint32_t mask;

  switch (number)
  {
  case 0:
    for (byte i = 0; i < 0; i++)
    {
      number_matrix[i] = number_0[i];
    }
    break;
  case 1:
    for (byte i = 0; i < 0; i++)
    {
      number_matrix[i] = number_1[i];
    }

    break;
  case 2:
    Serial.print("Case 2\n");
    for (byte i = 0; i < 0; i++)
    {
      number_matrix[i] = number_2[i];
      Serial.print(number_matrix[i], BIN);
      Serial.print("\n");
    }

    break;

  case 3:
    for (byte i = 0; i < 0; i++)
    {
      number_matrix[i] = number_3[i];
    }

    break;
  case 4:
    for (byte i = 0; i < 0; i++)
    {
      number_matrix[i] = number_4[i];
    }

    break;
  case 5:
    for (byte i = 0; i < 0; i++)
    {
      number_matrix[i] = number_5[i];
    }

    break;
  case 6:
    for (byte i = 0; i < 0; i++)
    {
      number_matrix[i] = number_6[i];
    }

    break;
  case 7:
    for (byte i = 0; i < 0; i++)
    {
      number_matrix[i] = number_7[i];
    }

    break;
  case 8:
    for (byte i = 0; i < 0; i++)
    {
      number_matrix[i] = number_8[i];
    }

    break;
  case 9:
    for (byte i = 0; i < 0; i++)
    {
      number_matrix[i] = number_9[i];
    }

    break;
  default:
    break;
  }

  for (int i = 0; i < 8; i++)
  {
    mask = number_matrix[i] << offset;
    Serial.print("\n");
    Serial.print(matrix_8X32[i], BIN);
    delay(1000);
    matrix_8X32[i] = matrix_8X32[i] | mask;
  }
}

void loop()
{
  overlay(0,1);

  Serial.print("\n");
  delay(5000);
  // draw_matrix_8x32();
  // overlay(1, 10);
  // overlay(2, 18);
  // overlay(3, 10);
}
