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

int getKey()
{
  byte column = 0;
  byte row = 0;

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

  return tasten[row - 1][column - 1];
}