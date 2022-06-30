#include <Wire.h>
#include <QMC5883LCompass.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <AFMotor.h>

float latc,logc;
float latd=36.71365 ,logd=3.17105;
float bearing;
float heading;

const int rp = 10;
const int rn = 11;
const int lp = 8;
const int ln = 9;

AF_DCMotor rightBack(2);                          //Create an object to control each motor
AF_DCMotor rightFront(3);
AF_DCMotor leftFront(4);
AF_DCMotor leftBack(1); 

SoftwareSerial GPS_SoftSerial(50,51);
TinyGPSPlus gps;
QMC5883LCompass qmc;

byte motorSpeed =240;                             //The maximum motor speed
int motorOffset = 0;                             //Factor to account for one side being more powerful
int turnSpeed = 50;  
void setup() 
{
 pinMode(rp,OUTPUT);
 pinMode(rn,OUTPUT);
 pinMode(ln,OUTPUT);
 pinMode(lp,OUTPUT);
  rightBack.setSpeed(motorSpeed);                 //Set the motors to the motor speed
  rightFront.setSpeed(motorSpeed);
  leftFront.setSpeed(motorSpeed+motorOffset);
  leftBack.setSpeed(motorSpeed+motorOffset);
   rightBack.run(RELEASE);                         //Ensure all motors are stopped
  rightFront.run(RELEASE);
  leftFront.run(RELEASE);
  leftBack.run(RELEASE);
 Wire.begin();
 Serial.begin(9600);
 GPS_SoftSerial.begin(9600); 
 qmc.init();
}

void loop() 
{
  headingcal();
   delay(200);
  gpsdata();
  delay(200);
  gpsheading();
   delay(200);
  steering();
   delay(200);
}

//GPS:-
void gpsdata()
{
  smartDelay(1000);
  unsigned long start;
  double lat_val, lng_val, alt_m_val;
  bool loc_valid, alt_valid;
  lat_val = gps.location.lat(); 
  loc_valid = gps.location.isValid(); 
  lng_val = gps.location.lng();
  alt_m_val = gps.altitude.meters(); 
  alt_valid = gps.altitude.isValid(); 
  
  if (!loc_valid)
  {
    Serial.print("Latitude : ");
    Serial.println("*****");
    Serial.print("Longitude : ");
    Serial.println("*****");
    delay(100);
  }
  else
  {
   // Serial.println("GPS READING: ");
   // DegMinSec(lat_val);
  //  Serial.print("Latitude in Decimal Degrees : ");
   // Serial.println(lat_val, 6);

    //DegMinSec(lng_val); 
    //Serial.print("Longitude in Decimal Degrees : ");
    //Serial.println(lng_val, 6);
    delay(100);
  }

  latc=lat_val;
  logc=lng_val;  
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do
  {
    while (GPS_SoftSerial.available()) 
    gps.encode(GPS_SoftSerial.read());
  } while (millis() - start < ms);
}

//GPS Heading:-
void gpsheading()
{
  float x,y,deltalog,deltalat;
  deltalog= logd-logc;
  deltalat=latd-latc;

  x=cos(latd)*sin(deltalog);
  y=(cos(latc)*sin(latd))-(sin(latc)*cos(latd)*cos(deltalog));
  
  bearing=(atan2(x,y))*(180/3.14);
  Serial.print("bearing");
  Serial.println(bearing);

  float a,d,c;
  a=(((sin(deltalat/2)))*(sin(deltalat/2))) + ((cos(latc))*(cos(latd))* (((sin(deltalog/2)))*(sin(deltalog/2)))  );
  c=2*(atan2(sqrt(a),sqrt(1-a)));
  d=6371*c; 
//Serial.println(d);
}



//Heading:-
void headingcal()
{
  int x,y,z;
qmc.read();

  // Return XYZ readings
  x = qmc.getX();
  y = qmc.getY();
  z = qmc.getZ();  delay(100);

  heading=atan2(x, y)/0.0174532925;
  if(heading < 0) 
  {
  heading+=360;
  }
  
  heading=360-heading; // N=0/360, E=90, S=180, W=270
  Serial.print("heading: ");
   Serial.println(heading);  
}

//Motor:-

void steering()
{

float finalv;
finalv=heading/bearing;
Serial.print("finalv: ");
Serial.println(finalv);
  
if(finalv>=0&&finalv<=1)
{
  forward();
}

else if(finalv >1 && finalv <=8)
{
  left();
}

else if(finalv <=13 && finalv >=8)
{
  right();
}

else if(logd==logc && latc==latd)
{
  stop1();
}
}

void forward()
{
  rightBack.run(BACKWARD);
  rightFront.run(BACKWARD);
  leftFront.run(BACKWARD);
  leftBack.run(BACKWARD);
}

void right()
{
  rightBack.setSpeed(motorSpeed+turnSpeed);                 //Set the motors to the motor speed
  rightFront.setSpeed(motorSpeed+turnSpeed);
  leftFront.setSpeed(motorSpeed+motorOffset+turnSpeed);
  leftBack.setSpeed(motorSpeed+motorOffset+turnSpeed);
  rightBack.run(BACKWARD);
  rightFront.run(BACKWARD);
  leftFront.run(FORWARD);
  leftBack.run(FORWARD);
  rightBack.setSpeed(motorSpeed);                           //Set the motors to the motor speed
  rightFront.setSpeed(motorSpeed);
  leftFront.setSpeed(motorSpeed+motorOffset);
  leftBack.setSpeed(motorSpeed+motorOffset);
  rightBack.run(RELEASE);
  rightFront.run(RELEASE);
  leftFront.run(RELEASE);
  leftBack.run(RELEASE);
}

void left()
{
  rightBack.setSpeed(motorSpeed+turnSpeed);                 //Set the motors to the motor speed
  rightFront.setSpeed(motorSpeed+turnSpeed);
  leftFront.setSpeed(motorSpeed+motorOffset+turnSpeed);
  leftBack.setSpeed(motorSpeed+motorOffset+turnSpeed);
  rightBack.run(FORWARD);
  rightFront.run(FORWARD);
  leftFront.run(BACKWARD);
  leftBack.run(BACKWARD);
  rightBack.setSpeed(motorSpeed);                           //Set the motors to the motor speed
  rightFront.setSpeed(motorSpeed);
  leftFront.setSpeed(motorSpeed+motorOffset);
  leftBack.setSpeed(motorSpeed+motorOffset);
  rightBack.run(RELEASE);
  rightFront.run(RELEASE);
  leftFront.run(RELEASE);
  leftBack.run(RELEASE);
  
}

void back()
{
  rightBack.run(FORWARD);
  rightFront.run(FORWARD);
  leftFront.run(FORWARD);
  leftBack.run(FORWARD);
}
void stop1()
{
rightBack.run(RELEASE);                         //Ensure all motors are stopped
  rightFront.run(RELEASE);
  leftFront.run(RELEASE);
  leftBack.run(RELEASE);
}
