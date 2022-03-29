

#include <AFMotor.h>
#include <CommandParser.h>
#include <SPI.h>
#include <SD.h>


typedef CommandParser<> Parser;

Parser parser;


AF_DCMotor motor1(1, MOTOR12_64KHZ);
AF_DCMotor motor2(2, MOTOR12_64KHZ);
AF_DCMotor motor3(3, MOTOR34_64KHZ);
AF_DCMotor motor4(4, MOTOR34_64KHZ);


//File commandsFile;
const int pinBranchementCS = 53 ;    // Le « 10 » indiquant ici que la broche CS (SS) de votre lecteur de carte SD est branché sur la pin D10 de votre Arduino

const int DEFAULT_MOTOR_DELAY  = 1000;
const int DEFAULT_MOTOR_SPEED = 200;


void moveForward(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY);
void moveBackward(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY);
void leftRotate(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY);
void rightRotate(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY);
void _stop();


void forwardCallback(Parser::Argument *args, char *response);
void backwardCallback(Parser::Argument *args, char *response);
void leftCallback(Parser::Argument *args, char *response);
void rightCallback(Parser::Argument *args, char *response);
void stopCallback(Parser::Argument *args, char *response);

bool stopit = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.setTimeout(5000);
  while (!Serial){}

  parser.registerCommand("f", "ii", &forwardCallback);
  parser.registerCommand("b", "ii", &backwardCallback);
  parser.registerCommand("l", "ii", &leftCallback);
  parser.registerCommand("r", "ii", &rightCallback);
  parser.registerCommand("s", "", &stopCallback);
  
  
  //SD.begin(pinBranchementCS);

//  Serial.println("Ready to read commands");
//
//  commandsFile = SD.open("commands.txt", FILE_WRITE);
//  if(commandsFile){
//    Serial.println("Writing commands ...");
//
//      commandsFile.println("f 200");
//      commandsFile.println("b 200");
//  }else{
//      Serial.println("error opening commands file");
//  }
//    commandsFile.close();


//commandsFile = SD.open("commands.txt");
// if(commandsFile){
//    Serial.println("Reading Commands ...");
//    String line;
//    while(commandsFile.available()){
//        // Read char
//        char c ;
//        if (c != '\n'){
//           // Construct current line string
//          while((c = commandsFile.read()) != ';'){
//              line = line + String(c);
//          }
//          char response[Parser::MAX_RESPONSE_SIZE];
//          char command[128];
//          line.trim();
//          line.toCharArray(command, 127);
//          command[128] = '\0';
//          parser.processCommand(command, response);
//          Serial.println(command);
//  
//          Serial.println(response);
//          
//        }
//        line = "";
//       
//    }
//    Serial.println("Done Reading");
//
//  }else{
//    Serial.println("Could't open the file");
//  }
//  commandsFile.close();

}

void loop() {
//  // put your main code here, to run repeatedly:
    if (Serial.available()) {
      char command[128];
      for(int i = 0; i < 128; i++){
        command[i] = '\0';
      }
      size_t lineLength = Serial.readBytesUntil(';', command, 128);
      command[lineLength] = '\0';
      String line = command;
      line.trim();
      line.toCharArray(command, 127);
      char response[Parser::MAX_RESPONSE_SIZE];
      parser.processCommand(command, response);
      Serial.println(line);
      Serial.println(response);
    }
    
 // moveForward(2000, 2000);
//  rightRotate(255, 3000);
//  moveForward(255, 2000);
//  _stop();
}


/* Parser callbacks */

void forwardCallback(Parser::Argument *args, char *response) {
    // FORWARD
    Serial.print("Go forward with speed "); Serial.print((int32_t)args[0].asInt64);
    Serial.print(" and for "); Serial.print((int32_t)args[1].asInt64);
    Serial.println(" seconds");
    
    strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

    moveForward((int32_t)args[0].asInt64, (int32_t)args[1].asInt64);

}

void backwardCallback(Parser::Argument *args, char *response) {
    // BACKWARD
    Serial.print("Go backward with speed "); Serial.print((int32_t)args[0].asInt64);
    Serial.print(" and for "); Serial.print((int32_t)args[1].asInt64);
    Serial.println(" seconds");
    
    strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

    moveBackward((int32_t)args[0].asInt64, (int32_t)args[1].asInt64);

}


void leftCallback(Parser::Argument *args, char *response) {
    // TURN LEFT
    Serial.print("Turning left with angle "); Serial.println((int32_t)args[0].asInt64);
    strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

    leftRotate((int32_t)args[0].asInt64, (int32_t)args[1].asInt64);
    
}
void rightCallback(Parser::Argument *args, char *response) {
    // TRUN RIGHT
    Serial.print("Turning right with angle "); Serial.println((int32_t)args[0].asInt64);
    strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);
    rightRotate((int32_t)args[0].asInt64, (int32_t)args[1].asInt64);
  
}

void stopCallback(Parser::Argument *args, char *response) {
    // STOP WHEELS
    Serial.println("stop");
    strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

    _stop();
    
}





/* Motor control functions */
void moveForward(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY){
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

void moveBackward(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY) {
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

void leftRotate(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY){
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

void rightRotate(int _speed = DEFAULT_MOTOR_SPEED, int _delay = DEFAULT_MOTOR_DELAY){
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

void _stop(){
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);

}
