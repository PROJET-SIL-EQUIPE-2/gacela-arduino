/*
 * This source code is protected under the terms of the MIT License and is copyright (c) 2013 by David Bird and permission is hereby granted, free of charge, to
 * any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software
 * without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, but not to sub-license and/or 
 * to sell copies of the Software or to permit persons to whom the Software is furnished to do so, subject to the following conditions:
 *   The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
 *   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 *   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 *   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE. 
 *   
 *   See more at http://dsbird.org.uk 
 */   

/*(c) D BIRD 2013
An Arduino code example for interfacing with the HMC5883
Uses:
Analog input 4 I2C SDA or equivlanet for MEGA
Analog input 5 I2C SCL
*/
#include <SPI.h>
#include <Wire.h> //I2C Arduino Library

const int centreX  = 160;
const int centreY  = 120;
const int diameter = 100; 

#include <Adafruit_GFX.h>    // Core graphics library

// The control pins for the LCD can be assigned to any digital or
// analog pins...but we'll use the analog pins as this allows us to
// double up the pins with the touch screen (see the TFT paint example).
#define LCD_CS A3 // Chip Select goes to Analogue 3
#define LCD_CD A2 // Command/Data goes to Analogue 2
#define LCD_WR A1 // LCD Write goes to Analogue 1
#define LCD_RD A0 // LCD Read goes to Analogue 0

#define LCD_RESET A4 // Can alternately just connect to Arduino reset pin

// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7
//   Assign human-readable names to some common 16-bit colour values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

#define address 0x1E //0011110b, I2C 7bit address of HMC5883
const int x_offset = 30;
const int y_offset = 128;
const int z_offset = 0;
int last_dx, last_dy, dx, dy;

// Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
// If using the shield, all control and data lines are fixed, and
// a simpler declaration can optionally be used:

void setup(){
Serial.begin(9600);
Serial.println("debut setup");
  //Initialize I2C communications
  Wire.begin();
  //Put the HMC5883 IC into the correct operating mode
  Wire.beginTransmission(address); //open communication with HMC5883
  Wire.write(0x02); //select mode register
  Wire.write(0x00); //continuous measurement mode
  Wire.endTransmission();
  delay(300); // Slight delay for screen to start
  last_dx = centreX;
  last_dy = centreY;
}

void loop(){
  double angle;
  int x,y,z; //triple axis data
  //Tell the HMC5883 where to begin reading data
  Serial.println("debut setup");
  Wire.beginTransmission(address);
  Wire.write(0x03); //select register 3, X MSB register
  Wire.endTransmission();
 
 //Read data from each axis, 2 registers per axis
  Wire.requestFrom(address, 6);
  if(6<=Wire.available()){
    x = Wire.read() << 8 | Wire.read();
    z = Wire.read() << 8 | Wire.read();
    y = Wire.read() << 8 | Wire.read();
     Serial.println(x);
    Serial.println(y);
    Serial.println(z);
  }
}
 
