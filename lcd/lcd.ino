
#include <LiquidCrystal.h> // includes the LiquidCrystal Library 
#include <Wire.h>

LiquidCrystal lcd(1, 2, 4, 5, 6, 7); // Creates an LCD object. Parameters: (rs, enable, d4, d5, d6, d7) 
void setup() { 
Serial.begin(9600);
 //Wire.setClock(10000);
 lcd.begin(16,2); // Initializes the interface to the LCD screen, and specifies the dimensions (width and height) of the display } 

lcd.print("Verouille");
}

void loop() { 
// lcd.print("Arduino"); // Prints "Arduino" on the LCD 
// Serial.print("Arduino");
// delay(5000); // 3 seconds delay 
// lcd.setCursor(2,1); // Sets the location at which subsequent text written to the LCD will be displayed 
// lcd.print("LCD Tutorial"); 
// Serial.print("LCD Tutorial \n"); 
// delay(5000); 
// lcd.clear(); // Clears the LCD screen 
}
