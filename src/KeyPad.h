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

boolean released = true;

int getKey()
{
  byte column = 0;
  byte row = 0;

  // Look for connections - column wise
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
    }
    pinMode(i, OUTPUT);
  }

  //Check if any connection was present
  if (column == 0)
  {
    released = true;
    return -1;
  }

  // Look for connections - row wise
  for (int i = 2; i <= 5; i++)
  {
    digitalWrite(i, 0);
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

  // return array matrix mapped to keypad
  if (released)
  {
    released = false;
    return tasten[row - 1][column - 1];
  }
  else
  {
    return -1;
  }
}