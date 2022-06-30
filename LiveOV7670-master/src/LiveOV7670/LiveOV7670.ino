// change setup.h to switch between buffered and pixel-by-pixel processing
#include "setup.h"



void setup() {
  
  Serial.begin(2000000);
    Serial.println("Camera begin() BEFORE");

  // This is not necessary and has no effect for ATMEGA based Arduinos.
  // WAVGAT Nano has slower clock rate by default. We want to reset it to maximum speed
  CLKPR = 0x80; // enter clock rate change mode
  CLKPR = 0; // set prescaler to 0. WAVGAT MCU has it 3 by default.
      Serial.println("Camera begin() BEFORE INIT");

  initializeScreenAndCamera();
      Serial.println("Camera begin() BQAFTER INIT");

}


void loop() {
      Serial.println("Camera begin() BZFORE LOOP");

  processFrame();
      Serial.println("Camera begin() after LOOP");

}
