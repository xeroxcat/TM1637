//  Author: avishorp@gmail.com
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Lesser General Public
//  License as published by the Free Software Foundation; either
//  version 2.1 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with this library; if not, write to the Free Software
//  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA


// modified for use with Mozzi Sound Library 1.0.2
extern "C" {
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
}

#include <TM1637Display.h>
#include <Arduino.h>

#define TM1637_I2C_COMM1    0x40
#define TM1637_I2C_COMM2    0xC0
#define TM1637_I2C_COMM3    0x80
//#define SET_STEPS   100
#define DIGIT_STEPS 28

//
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const uint8_t digitToSegment[] = {
  // XGFEDCBA
  0b00111111,    // 0
  0b00000110,    // 1
  0b01011011,    // 2
  0b01001111,    // 3
  0b01100110,    // 4
  0b01101101,    // 5
  0b01111101,    // 6
  0b00000111,    // 7
  0b01111111,    // 8
  0b01101111,    // 9
  0b01110111,    // A
  0b01111100,    // b
  0b00111001,    // C
  0b01011110,    // d
  0b01111001,    // E
  0b01110001     // F
};

static const uint8_t minusSegments = 0b01000000;

TM1637Display::TM1637Display(uint8_t pinClk, uint8_t pinDIO, unsigned int bitDelay)
{
  m_counter = 0;
  m_datacounter = 0;
  //memcpy(m_segments,{0,0,0,0},4);
  m_pos = 0;
  m_updatable = false;

  // Copy the pin numbers
  m_pinClk = pinClk;
  m_pinDIO = pinDIO;
  m_bitDelay = bitDelay;

  // Set the pin direction and default value.
  // Both pins are set as inputs, allowing the pull-up resistors to pull them up
  pinMode(m_pinClk, INPUT);
  pinMode(m_pinDIO, INPUT);
  digitalWrite(m_pinClk, LOW);
  digitalWrite(m_pinDIO, LOW);
  //Serial.begin(9600);
}

void TM1637Display::setBrightness(uint8_t brightness, bool on)
{
  m_brightness = (brightness & 0x7) | (on? 0x08 : 0x00);
}

void TM1637Display::setSegments(const uint8_t segments[], uint8_t length, uint8_t pos)
{
  if (length < 4){
    uint8_t zero[] = { 0, 0, 0, 0 };
    memcpy(m_segments, zero, 4);
  }
  memcpy(m_segments, segments, length);
  m_pos = pos;
  m_counter = 0;
  m_datacounter = (DIGIT_STEPS * length) - 1;
  m_updatable = true;
  m_segidx = 0;

  uint8_t data;
  uint8_t ack;
  // Write COMM1
  //start();
  //case0
  /*
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  //case5
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();
  //case10
  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinDIO, INPUT);
  bitDelay();
  //case15
  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinDIO, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();

  pinMode(m_pinClk, OUTPUT);
  pinMode(m_pinDIO, INPUT);
  bitDelay();
  //case20
  pinMode(m_pinClk, INPUT);
  bitDelay();
  
  if (digitalRead(m_pinDIO) == 0)
    pinMode(m_pinDIO, OUTPUT);

  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();
  //END writeByte(TM1637_I2C_COMM1);

  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();
  //case25
  pinMode(m_pinDIO, INPUT);
  bitDelay();

  // Write COMM2 + first digit address
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  if (m_pos & 0x01) {
    pinMode(m_pinDIO, INPUT);
  } else {
    pinMode(m_pinDIO, OUTPUT);
  }
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  //case30
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  if (m_pos & 0x02) {
    pinMode(m_pinDIO, INPUT);
  } else {
    pinMode(m_pinDIO, OUTPUT);
  }
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
  //case35
  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  //case40
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinDIO, INPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  //case45
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();

  if (digitalRead(m_pinDIO) == 0)
    pinMode(m_pinDIO, OUTPUT);

  bitDelay();
  //END writeByte(TM1637_I2C_COMM2 + (pos & 0x03));

  // Write the data bytes
  for (m_segidx=0; m_segidx < length; m_segidx++) {
    //data = m_segments[k];

    // 8 Data Bits
    for(uint8_t i = 0; i < 8; i++) {
      // CLK low
      pinMode(m_pinClk, OUTPUT);
      bitDelay();

      // Set data bit
      if (m_segments[m_segidx] & 0x01)
        pinMode(m_pinDIO, INPUT);
      else
        pinMode(m_pinDIO, OUTPUT);

      bitDelay();

      // CLK high
      pinMode(m_pinClk, INPUT);
      bitDelay();
      m_segments[m_segidx] = m_segments[m_segidx] >> 1;
    }

    pinMode(m_pinClk, OUTPUT);
    pinMode(m_pinDIO, INPUT);
    bitDelay();

    pinMode(m_pinClk, INPUT);
    bitDelay();

    ack = digitalRead(m_pinDIO);
    if (ack == 0)
      pinMode(m_pinDIO, OUTPUT);

    bitDelay();
    pinMode(m_pinClk, OUTPUT);
    bitDelay();
  }

  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinDIO, INPUT);
  bitDelay();

  // Write COMM3 + brightness
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, OUTPUT);
  bitDelay();
  
  if (m_brightness & 0x01)
    pinMode(m_pinDIO, INPUT);
  else
    pinMode(m_pinDIO, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();

  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  if (m_brightness & 0x02)
    pinMode(m_pinDIO, INPUT);
  else
    pinMode(m_pinDIO, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();

  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  if (m_brightness & 0x04)
    pinMode(m_pinDIO, INPUT);
  else
    pinMode(m_pinDIO, OUTPUT);
  bitDelay();

  pinMode(m_pinClk, INPUT);
  bitDelay();

  pinMode(m_pinClk, OUTPUT);
  bitDelay();
  if (m_brightness & 0x08)
    pinMode(m_pinDIO, INPUT);
  else
    pinMode(m_pinDIO, OUTPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();

  pinMode(m_pinClk, OUTPUT);
  bitDelay();
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();

  pinMode(m_pinClk, OUTPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();

  pinMode(m_pinClk, OUTPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();

  pinMode(m_pinClk, OUTPUT);
  bitDelay();
  pinMode(m_pinDIO, INPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();

  //START writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
  data = TM1637_I2C_COMM3 + (m_brightness & 0x0f);

  pinMode(m_pinClk, OUTPUT);
  pinMode(m_pinDIO, INPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();

  if (digitalRead(m_pinDIO) == 0)
    pinMode(m_pinDIO, OUTPUT);

  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinDIO, INPUT);
  bitDelay();
  */
}

bool TM1637Display::updateTM1637()
{
  Serial.print("\tm_counter:");
  Serial.print(m_counter);
  Serial.print("\tm_datacounter:");
  Serial.print(m_datacounter);
  Serial.print("   \tm_updatable:");
  Serial.print(m_updatable);
  if (!m_updatable)
    return true;
  switch (m_counter) {
    case 0:
      //Serial.print("----");
      pinMode(m_pinDIO, OUTPUT);
      break;
    case 1:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 2:
      pinMode(m_pinClk, INPUT);
      break;
    case 3:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 4:
      pinMode(m_pinClk, INPUT);
      break;
    case 5:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 6:
      pinMode(m_pinClk, INPUT);
      break;
    case 7:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 8:
      pinMode(m_pinClk, INPUT);
      break;
    case 9:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 10:
      pinMode(m_pinClk, INPUT);
      break;
    case 11:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 12:
      pinMode(m_pinClk, INPUT);
      break;
    case 13:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 14:
      pinMode(m_pinDIO, INPUT);
      break;
    case 15:
      pinMode(m_pinClk, INPUT);
      break;
    case 16:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 17:
      pinMode(m_pinDIO, OUTPUT);
      break;
    case 18:
      pinMode(m_pinClk, INPUT);
      break;
    case 19:
      pinMode(m_pinClk, OUTPUT);
      pinMode(m_pinDIO, INPUT);
      break;
    case 20:
      pinMode(m_pinClk, INPUT);
      break;
    case 21:
      if (digitalRead(m_pinDIO) == 0)
        pinMode(m_pinDIO, OUTPUT);
      break;
    case 22:
      pinMode(m_pinClk, OUTPUT);
      break;
      //END writeByte(TM1637_I2C_COMM1);
    case 23:
      pinMode(m_pinDIO, OUTPUT);
      break;
    case 24:
      pinMode(m_pinClk, INPUT);
      break;
    case 25:
      pinMode(m_pinDIO, INPUT);
      break;
    case 26:
      //write COMM2 + first digit address
      pinMode(m_pinDIO, OUTPUT);
      break;
    case 27:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 28:
      if (m_pos & 0x01) {
        pinMode(m_pinDIO, INPUT);
      } else {
        pinMode(m_pinDIO, OUTPUT);
      }
      break;
    case 29:
      pinMode(m_pinClk, INPUT);
      break;
    case 30:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 31:
      if (m_pos & 0x02) {
        pinMode(m_pinDIO, INPUT);
      } else {
        pinMode(m_pinDIO, OUTPUT);
      }
      break;
    case 32:
      pinMode(m_pinClk, INPUT);
      break;
    case 33:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 34:
      pinMode(m_pinDIO, OUTPUT);
      break;
    case 35:
      pinMode(m_pinClk, INPUT);
      break;
    case 36:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 37:
      pinMode(m_pinClk, INPUT);
      break;
    case 38:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 39:
      pinMode(m_pinClk, INPUT);
      break;
    case 40:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 41:
      pinMode(m_pinClk, INPUT);
      break;
    case 42:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 43:
      pinMode(m_pinDIO, INPUT);
      break;
    case 44:
      pinMode(m_pinClk, INPUT);
      break;
    case 45:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 46:
      pinMode(m_pinClk, INPUT);
      break;
    case 47:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 48:
      pinMode(m_pinClk, INPUT);
      break;
    case 49:
      if (digitalRead(m_pinDIO) == 0)
        pinMode(m_pinDIO, OUTPUT);
      break;
    //END writeByte(TM1637_I2C_COMM2 + (pos & 0x03));
    case 50:
      switch (m_datacounter % 28) {
        case 27:
          pinMode(m_pinClk, OUTPUT);
          Serial.print("a");
          break;
        case 26:
          if (m_segments[m_segidx] & 0x01)
            pinMode(m_pinDIO, INPUT);
          else
            pinMode(m_pinDIO, OUTPUT);
          Serial.print("b");
          break;
        case 25:
          pinMode(m_pinClk, INPUT);
          Serial.print("c");
          break;
        case 24:
          pinMode(m_pinClk, OUTPUT);
          Serial.print("d");
          break;
        case 23:
          if (m_segments[m_segidx] & 0x02)
            pinMode(m_pinDIO, INPUT);
          else
            pinMode(m_pinDIO, OUTPUT);
          Serial.print("e");
          break;
        case 22:
          pinMode(m_pinClk, INPUT);
          Serial.print("f");
          break;
        case 21:
          pinMode(m_pinClk, OUTPUT);
          Serial.print("g");
          break;
        case 20:
          if (m_segments[m_segidx] & 0x04)
            pinMode(m_pinDIO, INPUT);
          else
            pinMode(m_pinDIO, OUTPUT);
          Serial.print("h");
          break;
        case 19:
          pinMode(m_pinClk, INPUT);
          Serial.print("i");
          break;
        case 18:
          pinMode(m_pinClk, OUTPUT);
          Serial.print("j");
          break;
        case 17:
          if (m_segments[m_segidx] & 0x08)
            pinMode(m_pinDIO, INPUT);
          else
            pinMode(m_pinDIO, OUTPUT);
          Serial.print("k");
          break;
        case 16:
          pinMode(m_pinClk, INPUT);
          Serial.print("l");
          break;
        case 15:
          pinMode(m_pinClk, OUTPUT);
          Serial.print("m");
          break;
        case 14:
          if (m_segments[m_segidx] & 0x10)
            pinMode(m_pinDIO, INPUT);
          else
            pinMode(m_pinDIO, OUTPUT);
          Serial.print("n");
          break;
        case 13:
          pinMode(m_pinClk, INPUT);
          Serial.print("o");
          break;
        case 12:
          pinMode(m_pinClk, OUTPUT);
          Serial.print("p");
          break;
        case 11:
          if (m_segments[m_segidx] & 0x20)
            pinMode(m_pinDIO, INPUT);
          else
            pinMode(m_pinDIO, OUTPUT);
          Serial.print("q");
          break;
        case 10:
          pinMode(m_pinClk, INPUT);
          Serial.print("r");
          break;
        case 9:
          pinMode(m_pinClk, OUTPUT);
          Serial.print("s");
          break;
        case 8:
          if (m_segments[m_segidx] & 0x40)
            pinMode(m_pinDIO, INPUT);
          else
            pinMode(m_pinDIO, OUTPUT);
          Serial.print("t");
          break;
        case 7:
          pinMode(m_pinClk, INPUT);
          Serial.print("u");
          break;
        case 6:
          pinMode(m_pinClk, OUTPUT);
          Serial.print("v");
          break;
        case 5:
          if (m_segments[m_segidx] & 0x80)
            pinMode(m_pinDIO, INPUT);
          else
            pinMode(m_pinDIO, OUTPUT);
          Serial.print("w");
          break;
        case 4:
          pinMode(m_pinClk, INPUT);
          Serial.print("x");
          break;
        case 3:
          pinMode(m_pinClk, OUTPUT);
          pinMode(m_pinDIO, INPUT);
          Serial.print("y");
          break;
        case 2:
          pinMode(m_pinClk, INPUT);
          Serial.print("z");
          break;
        case 1:
          if(digitalRead(m_pinDIO) == 0)
            pinMode(m_pinDIO, OUTPUT);
          break;
        case 0:
          pinMode(m_pinClk, OUTPUT);
          m_segidx++;
          break;
      }
      //Serial.print("-+-+");
      if (m_datacounter == 0)
        m_counter++;
      else
        m_datacounter--;
      return false;
      //skip below m_counter incremen
    case 51:
      pinMode(m_pinDIO, OUTPUT);
      break;
    case 52:
      pinMode(m_pinClk, INPUT);
      break;
    case 53:
      pinMode(m_pinDIO, INPUT);
      break;
    case 54:
      // Write COMM3 + brightness
      pinMode(m_pinDIO, OUTPUT);
      break;
    case 55:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 56:
      if (m_brightness & 0x01)
        pinMode(m_pinDIO, INPUT);
      else
        pinMode(m_pinDIO, OUTPUT);
      break;
    case 57:
      pinMode(m_pinClk, INPUT);
      break;
    case 58:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 59:
      if (m_brightness & 0x02)
        pinMode(m_pinDIO, INPUT);
      else
        pinMode(m_pinDIO, OUTPUT);
      break;
    case 60:
      pinMode(m_pinClk, INPUT);
      break;
    case 61:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 62:
      if (m_brightness & 0x04)
        pinMode(m_pinDIO, INPUT);
      else
        pinMode(m_pinDIO, OUTPUT);
      break;
    case 63:
      pinMode(m_pinClk, INPUT);
      break;
    case 64:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 65:
      if (m_brightness & 0x08)
        pinMode(m_pinDIO, INPUT);
      else
        pinMode(m_pinDIO, OUTPUT);
      break;
    case 66:
      pinMode(m_pinClk, INPUT);
      break;
    case 67:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 68:
      pinMode(m_pinDIO, OUTPUT);
      break;
    case 69:
      pinMode(m_pinClk, INPUT);
      break;
    case 70:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 71:
      pinMode(m_pinClk, INPUT);
      break;
    case 72:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 73:
      pinMode(m_pinClk, INPUT);
      break;
    case 74:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 75:
      pinMode(m_pinDIO, INPUT);
      break;
    case 76:
      pinMode(m_pinClk, INPUT);
      break;
    case 77:
      //START writeByte(TM1637_I2C_COMM3 + (m_brightness & 0x0f));
      pinMode(m_pinClk, OUTPUT);
      pinMode(m_pinDIO, INPUT);
      break;
    case 78:
      pinMode(m_pinClk, INPUT);
      break;
    case 79:
      if (digitalRead(m_pinDIO) == 0)
        pinMode(m_pinDIO, OUTPUT);
      break;
    case 80:
      pinMode(m_pinClk, OUTPUT);
      break;
    case 81:
      pinMode(m_pinDIO, OUTPUT);
      break;
    case 82:
      pinMode(m_pinClk, INPUT);
      break;
    case 83:
      pinMode(m_pinDIO, INPUT);
      m_updatable = false;
      break;
  }
  Serial.print("++++");
  m_counter++;
  return false; 
}

void TM1637Display::clear()
{
  uint8_t data[] = { 0, 0, 0, 0 };
  setSegments(data);
}

void TM1637Display::showNumberDec(int num, bool leading_zero, uint8_t length, 
    uint8_t pos)
{
  showNumberDecEx(num, 0, leading_zero, length, pos);
}

void TM1637Display::showNumberDecEx(int num, uint8_t dots, bool leading_zero,
    uint8_t length, uint8_t pos)
{
  showNumberBaseEx(num < 0? -10 : 10, num < 0? -num : num, dots, leading_zero, length, pos);
}

void TM1637Display::showNumberHexEx(uint16_t num, uint8_t dots, bool leading_zero,
    uint8_t length, uint8_t pos)
{
  showNumberBaseEx(16, num, dots, leading_zero, length, pos);
}

void TM1637Display::showNumberBaseEx(int8_t base, uint16_t num, uint8_t dots,
    bool leading_zero, uint8_t length, uint8_t pos)
{
  bool negative = false;
  if (base < 0) {
    base = -base;
    negative = true;
  }


  uint8_t digits[4];

  if (num == 0 && !leading_zero) {
    // Singular case - take care separately
    for(uint8_t i = 0; i < (length-1); i++)
      digits[i] = 0;
    digits[length-1] = encodeDigit(0);
  }
  else {
    //uint8_t i = length-1;
    //if (negative) {
    //	// Negative number, show the minus sign
    //    digits[i] = minusSegments;
    //	i--;
    //}

    for(int i = length-1; i >= 0; --i)
    {
      uint8_t digit = num % base;

      if (digit == 0 && num == 0 && leading_zero == false)
        // Leading zero is blank
        digits[i] = 0;
      else
        digits[i] = encodeDigit(digit);

      if (digit == 0 && num == 0 && negative) {
        digits[i] = minusSegments;
        negative = false;
      }

      num /= base;
    }

    if(dots != 0)
    {
      showDots(dots, digits);
    }
  }
  setSegments(digits, length, pos);
}

void TM1637Display::bitDelay()
{
  delayMicroseconds(m_bitDelay);
}

/*
void TM1637Display::start()
{
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
}

void TM1637Display::stop()
{
  pinMode(m_pinDIO, OUTPUT);
  bitDelay();
  pinMode(m_pinClk, INPUT);
  bitDelay();
  pinMode(m_pinDIO, INPUT);
  bitDelay();
}

bool TM1637Display::writeByte(uint8_t b)
{
  uint8_t data = b;

  // 8 Data Bits
  for(uint8_t i = 0; i < 8; i++) {
    // CLK low
    pinMode(m_pinClk, OUTPUT);
    bitDelay();

    // Set data bit
    if (data & 0x01)
      pinMode(m_pinDIO, INPUT);
    else
      pinMode(m_pinDIO, OUTPUT);

    bitDelay();

    // CLK high
    pinMode(m_pinClk, INPUT);
    bitDelay();
    data = data >> 1;
  }

  // Wait for acknowledge
  // CLK to zero
  pinMode(m_pinClk, OUTPUT);
  pinMode(m_pinDIO, INPUT);
  bitDelay();

  // CLK to high
  pinMode(m_pinClk, INPUT);
  bitDelay();
  uint8_t ack = digitalRead(m_pinDIO);
  if (ack == 0)
    pinMode(m_pinDIO, OUTPUT);


  bitDelay();
  pinMode(m_pinClk, OUTPUT);
  bitDelay();

  return ack;
}
*/

void TM1637Display::showDots(uint8_t dots, uint8_t* digits)
{
  for(int i = 0; i < 4; ++i)
  {
    digits[i] |= (dots & 0x80);
    dots <<= 1;
  }
}

uint8_t TM1637Display::encodeDigit(uint8_t digit)
{
  return digitToSegment[digit & 0x0f];
}
