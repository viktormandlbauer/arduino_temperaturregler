/**
 * Pins:
 * D2 - D9 (Arduino) -> 1 - 8 (4x4 Keypad)
 * PORT_D D7,D6,D5,D4,D3,D2,D1,D0
 * PORT_B X,X,D13,D12,D11,D10,D9,D8
 * PORT_C X,X,A5,A4,A3,A2,A1,A0
 */

#include <Arduino.h>

/**
 * Pins:
 * D2 - D9 (Arduino) -> 1 - 8 (4x4 Keypad)
 */

int tasten[4][4] = {
    {1, 2, 3, 10},
    {4, 5, 6, 11},
    {7, 8, 9, 12},
    {13, 0, 14, 15}};

uint8_t released = 1;

int getKey()
{
  int8_t column = -1;
  int8_t row = -1;

  for (int i = 2; i <= 5; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, 1);
  }

  for (int i = 6; i <= 9; i++)
  {
    pinMode(i, INPUT);
    if (digitalRead(i) == 1)
    {
      column = i - 5;
      break;
    }
    pinMode(i, OUTPUT);
  }

  for (int i = 2; i <= 5; i++)
  {
    digitalWrite(i, 0);
  }

  if (column == 0)
  {
    return 0;
  }

  for (int i = 6; i <= 9; i++)
  {
    pinMode(i, OUTPUT);
    digitalWrite(i, 1);
  }

  for (int i = 2; i <= 5; i++)
  {
    pinMode(i, INPUT);
    if (digitalRead(i) == 1)
    {
      row = i - 1;
      break;
    }
    pinMode(i, OUTPUT);
  }

  for (int i = 6; i <= 9; i++)
  {
    digitalWrite(i, 0);
  }

  if (column != -1 && row != -1)
  {
    if (released == 1)
    {
      released = 0;
      return tasten[row - 1][column - 1];
    }
    else
    {
      released = 0;
      return -1;
    }
  }
  released = 1;
  return -1;
}

/** Version ohne Digital Read/Write, funktioniert aber momentan nicht mit dem Display

#include <Arduino.h>

int tasten[4][4] = {
    {1, 2, 3, 10},
    {4, 5, 6, 11},
    {7, 8, 9, 12},
    {13, 0, 14, 15}};

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
  // Pins der Input Register PINB und PIND lesen und zu 16 bit zusammenfassen
  uint16_t input = PINB;
  input <<= 8;
  input |= PIND;

  // Abfrage ob einer der Eingabe-Pins D2 bis D5 LOW ist (invertiert und UND verknüpft)
  // Rechts shift, da D0 und D1 die ersten zwei Bits darstellen und hierfür nicht geprüft werden.
  if ((~input >> 2) & 0b1111)
  {
    // Taste betätigt
    int8_t zeile = -1;
    int8_t spalte = -1;
    uint16_t mask = 0;
    uint16_t DDRB_DDRD = 0;

    // Gedrückte Taste -> Low Signal auf Input-Pins
    for (int j = 0; j < 4; j++)
    {
      // Input Register wird invertiert um nach einer 1 zu suchen (LOW)
      if ((~input >> 2) & (0b1 << j))
      {
        // Gefunde Zeile setzen
        zeile = j;

        //Um die Spalte zu finden wir jeweils ein PIN (D6 - D9) von Output auf Input gesetzt und geprueft ob dadurch wieder alle Input Pins auf High sind
        // 8 bit Data Direction Register zu 16 bit zusammengefasst
        DDRB_DDRD = DDRB;
        DDRB_DDRD <<= 8;
        DDRB_DDRD |= DDRD;

        for (int i = 6; i <= 9; i++)
        {
          // Maske um einzelnes Bit zu intervierten
          mask = 0x1 << i;

          // Einzelnes Bit invertieren
          DDRB_DDRD ^= mask;

          // 16 bit Zusammenfassung auf die zwei 8 bit DDR Register übertragen
          DDRD = DDRB_DDRD & 0xFF;
          DDRB = (DDRB_DDRD >> 8) & 0xF;

          // Input Register lesen und zu 16 bit zusammenfassen
          delayMicroseconds(3);
          input = PINB;
          input <<= 8;
          input |= PIND;

          // Prüfung ob Bit von 0 auf 1 umgesprungen ist
          mask = 0b0000 | (input >> 2);
          if ((mask & 0b1111) == 0b1111)
          {

            // Spalte setzen
            spalte = i - 6;

            // Invertierung zurücksetzen
            DDRB |= 0b00000011;
            DDRD |= 0b11000000;

            // Schleife wird unterbrochen
            break;
          }
          else
          {
            // Falls nichts gefunden, Invertierung zurücksetzen für den nächsten Durchlauf
            DDRB |= 0b00000011;
            DDRD |= 0b11000000;
          }
        }

        // Wenn Taste zuvor auf Zustand "nicht gedrückt", dann gültigen Tastenwert zurückgeben und Tastenzustand auf "gedrückt" setzen
        if (released == 1)
        {
          released = 0;
          return tasten[zeile][spalte];
        }
        // Wenn Taste zuvor auf Zustand "gedrückt", ungültigen Wert zurückgeben
        else
        {
          return -1;
        }
      }
    }
  }
  // Wenn nichts gedrückt, Tastenzustand auf "nicht gedrückt" setzen und ungültigen Wert zurückgeben
  released = 1;
  return -1;
}

**/