#include <Arduino.h>
#include <Characters_8x8.h>

// set pin definition
const int LOAD_pin = 10;
const int DIN_pin = 11;
const int CLOCK_pin = 13;
const int modules = 4;

// MAX7219 register address
const byte max7219_REG_noop = 0x00;
const byte max7219_REG_decodeMode = 0x09;
const byte max7219_REG_intensity = 0x0a;
const byte max7219_REG_scanLimit = 0x0b;
const byte max7219_REG_shutdown = 0x0c;
const byte max7219_REG_displayTest = 0x0f;

byte matrix_8X32[8][modules];

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

  for (byte i = 0; i < 8; i++)
  {
    for (byte j = 0; j < 4; j++)
    {
      matrix_8X32[i][j] = 0x0;
    }
  }
}

// Set display intensity
void set_display_intensity()
{
  byte intensity_value = (round((16 * analogRead(A0)) / 1024) + 0.4);
  init(max7219_REG_intensity, intensity_value);
}

void setup()
{
  Serial.begin(9600);

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
  // Set display

  clear_screen();
}

void draw_matrix_8x32()
{

  byte mask = 0;

  for (byte i = 0; i < 8; i++)
  {
    // Send digets address
    digitalWrite(LOAD_pin, LOW);

    // Send leds
    for (byte j = 0; j < modules; j++)
    {
      SendByte(8 - i);
      for (byte k = 0; k < 8; k++)
      {
        mask = 0x01 << k;
        digitalWrite(CLOCK_pin, LOW);

        if (mask & matrix_8X32[i][modules - 1 - j])
        {
          digitalWrite(DIN_pin, HIGH);
        }
        else
        {
          digitalWrite(DIN_pin, LOW);
        }

        digitalWrite(CLOCK_pin, HIGH);
      }
    }
    digitalWrite(LOAD_pin, HIGH);
  }
}

void overlay(byte *matrix, byte offset)
{
  byte sector = 0;
  byte sector_offset = 0;

  for (byte i = 0; i < 8; i++)
  {
    sector_offset = offset % 8;
    sector = offset / 8;

    if (sector_offset == 0)
    {
      matrix_8X32[i][sector] |= matrix[i];
    }
    else
    {
      matrix_8X32[i][sector] |= (matrix[i] << sector_offset);
      matrix_8X32[i][sector + 1] |= (matrix[i] >> (8 - sector_offset));
    }
  }
}

void loop()
{
  set_display_intensity();
  overlay(number_1, 0);
  overlay(number_2, 8);
  overlay(number_3, 16);
  overlay(number_4, 24);
  draw_matrix_8x32();
  delay(10);
}
