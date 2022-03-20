/**
 * Pins:
 * D2 - D9 (Arduino) -> 1 - 8 (4x4 Keypad)
 * PORT_D D7,D6,D5,D4,D3,D2,D1,D0
 * PORT_B X,X,D13,D12,D11,D10,D9,D8
 * PORT_C X,X,A5,A4,A3,A2,A1,A0
 *
 * 2 - 5 Output High 6-9 Input
 *
 */

#include <Arduino.h>

int tasten[4][4] = {
    {1, 2, 3, 10},
    {4, 5, 6, 11},
    {7, 8, 9, 12},
    {13, 0, 14, 15}};

// Flag: 1 -> Button released; 0 -> Button pressed
uint8_t released = 1;

void initKeys()
{
  // D2 - D5 auf Input und Pullup-Widerstand
  DDRD &= 0b11000011;
  PORTD |= 0b00111100;

  // D6 - D9 auf Output und Low
  DDRB |= 0b00000011;
  PORTB &= 0b11111100;
  DDRD |= 0b11000000;
  PORTD &= 0b00111111;
}

int getKey()
{
  // Pins lesen
  delayMicroseconds(3);
  uint16_t input = ~PINB;
  input <<= 8;
  input |= ~PIND;

  // (oder) D2 bis D5 auf Low
  if ((input >> 2) & 0b1111)
  {

    uint16_t input_mask = 0;
    uint16_t DDRB_DDRD = 0;

    // 8 bit Data Direction Register zu 16 bit zusammenfassen
    DDRB_DDRD = DDRB;
    DDRB_DDRD <<= 8;
    DDRB_DDRD |= DDRD;

    int8_t zeile = -1;
    int8_t spalte = -1;
    for (int i = 6; i <= 9; i++)
    {

      // Mask um bestimmten Pin von Output auf Input zu invertieren
      input_mask = 0x1 << i;

      // Einzelnes Bit invertieren
      DDRB_DDRD ^= input_mask;

      // 16 bit zu zwei 8 bit DDR Register
      DDRD = DDRB_DDRD & 0xFF;
      DDRB = (DDRB_DDRD >> 8) & 0xF;

      // 8 bit Input Register in 16 bit zusammenfassen
      delayMicroseconds(3);
      input = PINB;
      input <<= 8;
      input |= PIND;

      // Kontrolle der Pins
      uint8_t mask = 0b0000 | (input >> 2);
      if ((mask & 0b1111) == 0b1111)
      {
        spalte = i - 6;
      }
      else
      {
        for (int j = 0; j < 4; j++)
        {
          if ((~input >> 2) & (0b1 << j))
          {
            zeile = j;
            break;
          }
        }
      }

      // Rücksetzen
      DDRB_DDRD ^= input_mask;
      DDRD = DDRB_DDRD & 0xFF;
      DDRB = (DDRB_DDRD >> 8) & 0xF;

      if (zeile != -1 && spalte != -1 && released == 1)
      {
        released = 0;
        return tasten[zeile][spalte];
      }
    }
  }
  else
  {
    released = 1;
    return -1;
  }
}

void setup()
{
  Serial.begin(9600);
  initKeys();
}

void loop()
{

  int key = getKey();
  if (key != -1)
  {
    Serial.println(key);
  }
}
