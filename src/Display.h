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
      matrix_8X32[i][sector] |= (matrix[i] >> sector_offset);
      matrix_8X32[i][sector + 1] |= (matrix[i] << (8 - sector_offset));
    }
  }
}

void ShowTemperature(float number)
{
  char temp[8] = {0, 0, 0, 0};
  char temp2[8] = {0, 0, 0};

  String output = String(number, 1);
  output.toCharArray(temp, 8);
  int position[4] = {0, 7, 17, 21};
  uint8_t point_pos[3] = {24, 16, 8};
  uint8_t index = 0;
  for (uint8_t i = 0; i < strlen(temp); i++)
  {
    if (temp[i] == '.')
    {
      overlay(point, point_pos[i]);
    }
    else
    {
      temp2[index] = temp[i];
      index++;
    }
  }
  for (uint8_t i = 0; i < strlen(temp2); i++)
  {
    if (temp2[i] == '2')
    {
      overlay(number_2, position[i]);
    }
    else if (temp2[i] == '3')
    {
      overlay(number_3, position[i]);
    }
    else if (temp2[i] == '1')
    {
      overlay(number_1, position[i]);
    }
    else if (temp2[i] == '0')
    {
      overlay(number_0, position[i]);
    }
    else if (temp2[i] == '4')
    {

      overlay(number_4, position[i]);
    }
    else if (temp2[i] == '5')
    {

      overlay(number_5, position[i]);
    }
    else if (temp2[i] == '6')
    {

      overlay(number_6, position[i]);
    }
    else if (temp2[i] == '7')
    {

      overlay(number_7, position[i]);
    }
    else if (temp2[i] == '8')
    {

      overlay(number_8, position[i]);
    }
    else if (temp2[i] == '9')
    {
      overlay(number_9, position[i]);
    }
    else
    {
      continue;
    }
  }
  overlay(deegree_celcius, 24);
}

void setTemperature(uint8_t input, uint8_t position_input)
{
  byte position[3] = {6, 14, 24};

  if (input == 2)
  {
    overlay(number_2, position[position_input]);
  }
  else if (input == 3)
  {
    overlay(number_3, position[position_input]);
  }
  else if (input == 1)
  {
    overlay(number_1, position[position_input]);
  }
  else if (input == 0)
  {
    overlay(number_0, position[position_input]);
  }
  else if (input == 4)
  {

    overlay(number_4, position[position_input]);
  }
  else if (input == 5)
  {

    overlay(number_5, position[position_input]);
  }
  else if (input == 6)
  {

    overlay(number_6, position[position_input]);
  }
  else if (input == 7)
  {

    overlay(number_7, position[position_input]);
  }
  else if (input == 8)
  {

    overlay(number_8, position[position_input]);
  }
  else if (input == 9)
  {
    overlay(number_9, position[position_input]);
  }

  if (position_input == 2)
  {
    overlay(point, 15);
  }
}