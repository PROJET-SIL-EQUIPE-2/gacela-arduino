

#include <AFMotor.h>
#include <CommandParser.h>
#include <SPI.h>
#include <SD.h>

typedef CommandParser<> Parser;

Parser parser;

AF_DCMotor motor1(1, MOTOR12_64KHZ);
AF_DCMotor motor2(2, MOTOR12_64KHZ);
AF_DCMotor motor3(3, MOTOR12_64KHZ);
AF_DCMotor motor4(4, MOTOR12_64KHZ);

File commandsFile;

const int DEFAULT_MOTOR_DELAY = 1000;
const int DEFAULT_MOTOR_SPEED = 200;
//
//void moveForward(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY);
//void moveBackward(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY);
//void leftRotate(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY);
//void rightRotate(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY);
//void _stop();

void forwardCallback(Parser::Argument *args, char *response);
void backwardCallback(Parser::Argument *args, char *response);
void leftCallback(Parser::Argument *args, char *response);
void rightCallback(Parser::Argument *args, char *response);
void stopCallback(Parser::Argument *args, char *response);

bool stopit = false;

const int pinCS = 53;
bool _read = false;

SdFile root;
void setup()
{
  // put your setup code here, to run once:
    Serial.begin(9600);
  //  Serial.setTimeout(5000);
    while (!Serial){}
//
//
    initSD();
  
    parser.registerCommand("FORWARD", "ii", &forwardCallback);
    parser.registerCommand("BACKWARD", "ii", &backwardCallback);
    
    parser.registerCommand("LEFT", "ii", &leftCallback);
    parser.registerCommand("RIGHT", "ii", &rightCallback);
    
    parser.registerCommand("SLEFT", "iii", &steadyLeftCallback);
    parser.registerCommand("SRIGHT", "iii", &steadyRightCallback);
    
    parser.registerCommand("STOP", "", &stopCallback);

  
    commandsFile = SD.open("commands.txt");
    if(commandsFile){
      String line;
      while(commandsFile.available()){
          // Read char
          char c ;
          // Construct current line string

              while((c = commandsFile.read()) != ';'){
                if (c != '\n' && c != '\r'){
                  line = line + String(c);
                }
                
              }
          
          char command[128];
          line.toCharArray(command, 128);
          Serial.print(command);
          char response[Parser::MAX_RESPONSE_SIZE];
          parser.processCommand(command, response);
          
          
         Serial.flush();
  
        line = "";
      }
    if (root.isOpen()) root.close();
      commandsFile.close();
  
    }else{
      Serial.println("Could't open the file");
    }
    
}

void loop()
{
  //  // put your main code here, to run repeatedly:
   

}

/* Motor control functions */
void moveForward(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY)
{
  stopit = false;
  motor1.setSpeed(_speed);
  motor2.setSpeed(_speed);
  motor3.setSpeed(_speed);
  motor4.setSpeed(_speed);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
  delay(_delay);
  _stop();
}

void moveBackward(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY)
{
  stopit = false;
  motor1.setSpeed(_speed);
  motor2.setSpeed(_speed);
  motor3.setSpeed(_speed);
  motor4.setSpeed(_speed);
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(_delay);
  _stop();
}

void leftRotate(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY)
{
  stopit = false;
  motor1.setSpeed(_speed);
  motor2.setSpeed(_speed);
  motor3.setSpeed(_speed);
  motor4.setSpeed(_speed);

  motor2.run(FORWARD);
  motor3.run(FORWARD);

  motor1.run(BACKWARD);
  motor4.run(BACKWARD);
  delay(_delay);
  _stop();
}

void rightRotate(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY)
{
  stopit = false;
  motor1.setSpeed(_speed);
  motor2.setSpeed(_speed);
  motor3.setSpeed(_speed);
  motor4.setSpeed(_speed);

  motor1.run(FORWARD);
  motor4.run(FORWARD);

  motor2.run(BACKWARD);
  motor3.run(BACKWARD);

  delay(_delay);
  _stop();
}


void steadyRightRotate(int rightSpeed, int leftSpeed, int _delay){
  stopit = false;
  motor1.setSpeed(leftSpeed);
  motor2.setSpeed(rightSpeed);
  motor3.setSpeed(rightSpeed);
  motor4.setSpeed(leftSpeed);

  motor1.run(FORWARD);
  motor4.run(FORWARD);

  motor2.run(FORWARD);
  motor3.run(FORWARD);

  delay(_delay);
  _stop();
}

void steadyLeftRotate(int rightSpeed, int leftSpeed, int _delay){
  stopit = false;
  motor1.setSpeed(leftSpeed);
  motor2.setSpeed(rightSpeed);
  motor3.setSpeed(rightSpeed);
  motor4.setSpeed(leftSpeed);

  motor2.run(FORWARD);
  motor3.run(FORWARD);

  motor1.run(FORWARD);
  motor4.run(FORWARD);
  delay(_delay);
  _stop();
}



void _stop()
{
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
  stopit = true;
}

/* Parser callbacks */

void forwardCallback(Parser::Argument *args, char *response)
{
  // FORWARD


  strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

  moveForward((int32_t)args[0].asInt64, (int32_t)args[1].asInt64);
}

void backwardCallback(Parser::Argument *args, char *response)
{

  strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

  moveBackward((int32_t)args[0].asInt64, (int32_t)args[1].asInt64);
}

void leftCallback(Parser::Argument *args, char *response)
{
  // TURN LEFT

  strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

  leftRotate((int32_t)args[0].asInt64, (int32_t)args[1].asInt64);
}
void rightCallback(Parser::Argument *args, char *response)
{
  // TRUN RIGHT
  strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);
  rightRotate((int32_t)args[0].asInt64, (int32_t)args[1].asInt64);
}



void steadyLeftCallback(Parser::Argument *args, char *response)
{
  // TURN LEFT
  //    Serial.print("Turning left with angle "); Serial.println((int32_t)args[0].asInt64);
  strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

  steadyLeftRotate((int32_t)args[0].asInt64, (int32_t)args[1].asInt64, (int32_t)args[2].asInt64);
}


void steadyRightCallback(Parser::Argument *args, char *response)
{
  // TRUN RIGHT
  //    Serial.print("Turning right with angle "); Serial.println((int32_t)args[0].asInt64);
  strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);
  steadyRightRotate((int32_t)args[0].asInt64, (int32_t)args[1].asInt64, (int32_t)args[2].asInt64);
}



void stopCallback(Parser::Argument *args, char *response)
{
  // STOP WHEELS
  //    Serial.println("stop");
  strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

  _stop();
}


void initSD(){
//   Serial.println("Initializing SD card...");

  if (!SD.begin(pinCS)) {
//    Serial.println("initialization failed!");
    while (1);
  }
//  Serial.println("initialization done.");
}
