#include <Arduino.h>
#include <Characters_8x8.h>

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

uint64_t matrix_8X32[8] = {
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000,
    0b00000000000000000000000000000000};

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
    matrix_8X32[i] = 0;
  }
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
  // Set display
  init(max7219_REG_intensity, 0x0F);

  clear_screen();
}

void draw_matrix_8x32()
{

  uint32_t mask;

  for (byte i = 1; i <= 8; i++)
  {

    digitalWrite(LOAD_pin, LOW);

    for (uint16_t j = 32; j > 0; j--)
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
  uint64_t mask;

  switch (number)
  {
  case 0:
    for (byte i = 0; i < 8; i++)
    {
      number_matrix[i] = number_0[i];
    }
    break;
  case 1:
    for (byte i = 0; i < 8; i++)
    {
      number_matrix[i] = number_1[i];
    }
    break;
  case 2:
    for (byte i = 0; i < 8; i++)
    {
      number_matrix[i] = number_2[i];
    }
    break;
  case 3:
    for (byte i = 0; i < 8; i++)
    {
      number_matrix[i] = number_3[i];
    }
    break;
  case 4:
    for (byte i = 0; i < 8; i++)
    {
      number_matrix[i] = number_4[i];
    }
    break;
  case 5:
    for (byte i = 0; i < 8; i++)
    {
      number_matrix[i] = number_5[i];
    }
    break;
  case 6:
    for (byte i = 0; i < 8; i++)
    {
      number_matrix[i] = number_6[i];
    }
    break;
  case 7:
    for (byte i = 0; i < 8; i++)
    {
      number_matrix[i] = number_7[i];
    }
    break;
  case 8:
    for (byte i = 0; i < 8; i++)
    {
      number_matrix[i] = number_8[i];
    }
    break;
  case 9:
    for (byte i = 0; i < 8; i++)
    {
      number_matrix[i] = number_9[i];
    }
    break;
  default:
    break;
  }
  for (byte i = 0; i < 8; i++)
  {
    mask = number_matrix[i] << offset;
    matrix_8X32[i] = matrix_8X32[i] | mask;
  }
}

void loop()
{
  for ( int i = 0; i < 64; i++ ){
  
  int pos_0 = 24 + i;
  int pos_1 = 16 + i;
  int pos_2 = 8 + i;
  int pos_3 = 0 + i;
  overlay(0, pos_0);
  overlay(1, pos_1);
  overlay(2, pos_2);
  overlay(3, pos_3);
  
  if(i > 8){
    int pos_4 = 24 + i - 32;
    overlay(4, pos_4);
    
    if(i > 16){
      int pos_5 = 16 + i - 32;
      overlay(5, pos_5);

      if(i > 24){
        int pos_6 = 8 + i - 32;
        overlay(6, pos_6);

        if(i > 32){
          int pos_7 = 0 + i - 32;
          overlay(7, pos_7);
        }
      }
    }
  }

  draw_matrix_8x32();
  delay(100);
  clear_screen();
  }

  // overlay(3, 24);
  // overlay(2, 16);
  // overlay(1, 8);
  // overlay(0, 0);
  // draw_matrix_8x32();
  // delay(1000);
  // clear_screen();
}
