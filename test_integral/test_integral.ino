#include "DHT.h"
#include "TinyGPS++.h"
#include "SoftwareSerial.h"
#include <QMC5883LCompass.h>
#include "LiquidCrystal_I2C.h"
#include "Wire.h"
#include <ArduinoJson.h>
#define DHTPIN 40     
#define DHTTYPE DHT22   
#define echoPin 30 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 31 //attach pin D3 Arduino to pin Trig of HC-SR04


//initialize the liquid crystal library
//the first parameter is the I2C address
//the second parameter is how many rows are on your screen
//the third parameter is how many columns are on your screen
LiquidCrystal_I2C lcd(0x27, 16, 2);
// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement


QMC5883LCompass compass;

SoftwareSerial serial_connection(50,51); //RX=pin 51, TX=pin 50  (50?51) (TX,RX)
TinyGPSPlus gps;//This is the GPS object that will pretty much do all the grunt work with the NMEA data
DHT dht(DHTPIN, DHTTYPE);
StaticJsonDocument<200> doc;

void setup() {
  Serial.begin(9600);
  serial_connection.begin(9600);//This opens up communications to the GPS
  Serial.println("GPS Start");
  Serial.println("DHTxx test!");
  compass.init();
  dht.begin();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.println("Ultrasonic Sensor HC-SR04 Test"); // print some text in Serial Monitor
  Serial.println("with Arduino UNO R3");
   lcd.init();
  // turn on the backlight
  lcd.backlight();
}

void loop() {

// create an object
doc["temperature"] = NULL;
doc["Speed"] = NULL;
doc["charge"] = 70;
doc["lat"] = NULL;
doc["long"] = NULL;


// serialize the object and send the result to Serial
//serializeJson(doc, Serial);
  delay(2000);
gpsfun();
//tempfun();
//  
//   compass.read();
//  
//  byte a = compass.getAzimuth();
//
//  char myArray[3];
//  compass.getDirection(myArray, a);
//  
//  Serial.print(myArray[0]);
//  Serial.print(myArray[1]);
//  Serial.print(myArray[2]);
//  Serial.println();
//  
//  delay(250);
// long duration, inches, cm;
//   pinMode(trigPin, OUTPUT);
//   digitalWrite(trigPin, LOW);
//   delayMicroseconds(5);
//   digitalWrite(trigPin, HIGH);
//   delayMicroseconds(10);
//   digitalWrite(trigPin, LOW);
//   pinMode(echoPin, INPUT);
//   duration = pulseIn(echoPin, HIGH);
//   inches = microsecondsToInches(duration);
//   cm = microsecondsToCentimeters(duration);
//   Serial.print(inches);
//   Serial.print("in, ");
//   Serial.print(cm);
//   Serial.print("cm");
//   Serial.println();
//   delay(100);
//
//   //wait for a second
//  delay(1000);
//  // tell the screen to write on the top row
//  lcd.setCursor(0,0);
//  // tell the screen to write “hello, from” on the top row
//  lcd.print("Hello, From odb");
//  // tell the screen to write on the bottom row
//  lcd.setCursor(0,1);
//  // tell the screen to write “Arduino_uno_guy” on the bottom row
//  // you can change whats in the quotes to be what you want it to be!
//  lcd.print("Integral Test");
//  
//  doc["chage"] = 70 ;
//
//  serializeJson(doc, Serial);
//
//  // Start a new line
//  Serial.println();
serializeJson(doc, Serial);
Serial.println();
}
long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}

void tempfun(){
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);

  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

//  Serial.print("Humidity: ");
//  Serial.print(h);
//  Serial.print(" %\t");
//  Serial.print("Temperature: ");
//  Serial.print(t);
//  Serial.print(" *C ");
//  Serial.print(f);
//  Serial.print(" *F\t");
//  Serial.print("Heat index: ");
//  Serial.print(hic);
//  Serial.print(" *C ");
//  Serial.print(hif);
//  Serial.println(" *F");
  doc["temperature"] = t;
  }
void gpsfun(){
   while(serial_connection.available())//While there are characters to come from the GPS
  {
    gps.encode(serial_connection.read());//This feeds the serial NMEA data into the library one char at a time
  }
  if(gps.location.isUpdated())//This will pretty much be fired all the time anyway but will at least reduce it to only after a package of NMEA data comes in
  {
    //Get the latest info from the gps object which it derived from the data sent by the GPS unit
    Serial.println("Satellite Count:");
    Serial.println(gps.satellites.value());
    Serial.print("Latitude:");
    Serial.print(gps.location.lat(), 6);
    Serial.print("   Longitude:");
    Serial.print(gps.location.lng(), 6);
    Serial.print("   Speed MPH:");
    Serial.print(gps.speed.mph());
    Serial.print("   Altitude Feet:");
    Serial.print(gps.altitude.feet());
    Serial.println();
    
      doc["speed"] = gps.speed.mph();
      doc["lat"] = gps.location.lat();
      doc["long"] = gps.location.lng();
    }    
     
  }

/*
 * $GPRMC,183729,A,3907.356,N,12102.482,W,000.0,360.0,080301,015.5,E*6F
$GPRMB,A,,,,,,,,,,,,V*71
$GPGGA,183730,3907.356,N,12102.482,W,1,05,1.6,646.4,M,-24.1,M,,*75
$GPGSA,A,3,02,,,07,,09,24,26,,,,,1.6,1.6,1.0*3D
$GPGSV,2,1,08,02,43,088,38,04,42,145,00,05,11,291,00,07,60,043,35*71
$GPGSV,2,2,08,08,02,145,00,09,46,303,47,24,16,178,32,26,18,231,43*77
$PGRME,22.0,M,52.9,M,51.0,M*14
$GPGLL,3907.360,N,12102.481,W,183730,A*33
$PGRMZ,2062,f,3*2D
$PGRMM,WGS 84*06
$GPBOD,,T,,M,,*47
$GPRTE,1,1,c,0*07
$GPRMC,183731,A,3907.482,N,12102.436,W,000.0,360.0,080301,015.5,E*67
$GPRMB,A,,,,,,,,,,,,V*71
*/
