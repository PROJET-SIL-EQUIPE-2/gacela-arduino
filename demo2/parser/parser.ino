

#include <AFMotor.h>
#include <CommandParser.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
SoftwareSerial hc06(17,18);

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
// void moveForward(int _speed = DEFAULT_MOTOR_SPEED);
// void moveBackward(int _speed = DEFAULT_MOTOR_SPEED);
// void leftRotate(int _speed = DEFAULT_MOTOR_SPEED);
// void rightRotate(int _speed = DEFAULT_MOTOR_SPEED);
// void _stop();

void forwardCallback(Parser::Argument *args, char *response);
void backwardCallback(Parser::Argument *args, char *response);
void leftCallback(Parser::Argument *args, char *response);
void rightCallback(Parser::Argument *args, char *response);
void stopCallback(Parser::Argument *args, char *response);

bool stopit = false;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);

   //Initialize Bluetooth Serial Port
hc06.begin(9600);
Serial.println("hc began") ;
    while (!Serial)
    {
    }

    parser.registerCommand("f", "i", &forwardCallback);
    parser.registerCommand("b", "i", &backwardCallback);

    parser.registerCommand("l", "i", &leftCallback);
    parser.registerCommand("r", "i", &rightCallback);

    parser.registerCommand("sl", "ii", &steadyLeftCallback);
    parser.registerCommand("sr", "ii", &steadyRightCallback);

    parser.registerCommand("st", "", &stopCallback);
}

void loop()
{
    // read commands from serial
    // Can be changed to read from network
    if (hc06.available()){
Serial.write(hc06.read());
}
    if (Serial.available())
    {
        char line[128];
        for (int i = 0; i < 128; i++)
        {
            line[i] = '\0';
        }
        size_t lineLength = Serial.readBytesUntil(';', line, 128);
        line[lineLength] = '\0';
        String s = line;
        char response[Parser::MAX_RESPONSE_SIZE];
        parser.processCommand(line, response);
        Serial.println(response);
    }
}

/* Motor control functions */
void moveForward(int _speed = DEFAULT_MOTOR_SPEED)
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
}

void moveBackward(int _speed = DEFAULT_MOTOR_SPEED)
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
}

void leftRotate(int _speed = DEFAULT_MOTOR_SPEED)
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
}

void rightRotate(int _speed = DEFAULT_MOTOR_SPEED)
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
}

void steadyRightRotate(int rightSpeed, int leftSpeed)
{
    stopit = false;
    motor1.setSpeed(leftSpeed);
    motor2.setSpeed(rightSpeed);
    motor3.setSpeed(rightSpeed);
    motor4.setSpeed(leftSpeed);

    motor1.run(FORWARD);
    motor4.run(FORWARD);

    motor2.run(FORWARD);
    motor3.run(FORWARD);
}

void steadyLeftRotate(int rightSpeed, int leftSpeed)
{
    stopit = false;
    motor1.setSpeed(leftSpeed);
    motor2.setSpeed(rightSpeed);
    motor3.setSpeed(rightSpeed);
    motor4.setSpeed(leftSpeed);

    motor2.run(FORWARD);
    motor3.run(FORWARD);

    motor1.run(FORWARD);
    motor4.run(FORWARD);
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

    moveForward((int32_t)args[0].asInt64);
}

void backwardCallback(Parser::Argument *args, char *response)
{

    strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

    moveBackward((int32_t)args[0].asInt64);
}

void leftCallback(Parser::Argument *args, char *response)
{
    // TURN LEFT

    strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

    leftRotate((int32_t)args[0].asInt64);
}
void rightCallback(Parser::Argument *args, char *response)
{
    // TRUN RIGHT
    strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);
    rightRotate((int32_t)args[0].asInt64);
}

void steadyLeftCallback(Parser::Argument *args, char *response)
{
    // TURN LEFT
    //    Serial.print("Turning left with angle "); Serial.println((int32_t)args[0].asInt64);
    strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

    steadyLeftRotate((int32_t)args[0].asInt64, (int32_t)args[1].asInt64);
}

void steadyRightCallback(Parser::Argument *args, char *response)
{
    // TRUN RIGHT
    //    Serial.print("Turning right with angle "); Serial.println((int32_t)args[0].asInt64);
    strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);
    steadyRightRotate((int32_t)args[0].asInt64, (int32_t)args[1].asInt64);
}

void stopCallback(Parser::Argument *args, char *response)
{
    // STOP WHEELS
    //    Serial.println("stop");
    strlcpy(response, "success", Parser::MAX_RESPONSE_SIZE);

    _stop();
}
