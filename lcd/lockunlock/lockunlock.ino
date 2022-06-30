#include "LiquidCrystal.h"

//int locked = 12;
//int unlocked = LED_BUILTIN;


#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7) 

void setup() { 
 lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 
 Serial.begin(9600);
}

void loop() { 

  if (Serial.available()) {
      char line[128];
      for(int i = 0; i < 128; i++){
        line[i] = '\0';
      }
      size_t lineLength = Serial.readBytesUntil('\n', line, 128);
      line[lineLength] = '\0';
      String s = line;
      lcd.clear();
//      if (s == "locked"){
//        digitalWrite(locked, HIGH);
//        digitalWrite(unlocked, LOW);
//      }else{
//        digitalWrite(unlocked, HIGH);
//        digitalWrite(locked, LOW);
//      }
      lcd.print(s);
    }
}
