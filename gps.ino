
#include <ArduinoMqttClient.h>
#include <AFMotor.h>
#include <CommandParser.h>
#include <SPI.h>
#include <WiFi.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>


typedef CommandParser<> Parser;

Parser parser;

AF_DCMotor motor1(1, MOTOR12_64KHZ);
AF_DCMotor motor2(2, MOTOR12_64KHZ);
AF_DCMotor motor3(3, MOTOR12_64KHZ);
AF_DCMotor motor4(4, MOTOR12_64KHZ);


const int DEFAULT_MOTOR_DELAY = 1000;
const int DEFAULT_MOTOR_SPEED = 200;





char ssid[] = "yourNetwork";     //  your network SSID (name)
char pass[] = "secretPassword";  // your network password


const char broker[] = "test.mosquitto.org";
int        port     = 1883;

const char topic[]  = "commands_topic";

float latitude,longitude; 
 
SoftwareSerial gpSerial(2, 3); 
TinyGPS gps; 


IPAddress server(74,125,115,105); // server example
WiFiClient client;

//MqttClient mqttClient(client);

// For connection use: mqttClient.connect(broker)
// for sending data use: 
                      // mqttClient.beginMessage(topic);
                      
                      // mqttClient.print(value);
                      // mqttClient.endMessage();

// Fr receving data:
// mqttClient.subscribe(topic)
// mqttClient.available()
// mqttClient.read()




void forwardCallback(Parser::Argument *args, char *response);
void backwardCallback(Parser::Argument *args, char *response);
void leftCallback(Parser::Argument *args, char *response);
void rightCallback(Parser::Argument *args, char *response);
void stopCallback(Parser::Argument *args, char *response);

bool stopit = false;

void setup() {
  Serial.begin(9600);


  parser.registerCommand("FORWARD", "ii", &forwardCallback);
  parser.registerCommand("BACKWARD", "ii", &backwardCallback);
    
  parser.registerCommand("LEFT", "ii", &leftCallback);
  parser.registerCommand("RIGHT", "ii", &rightCallback);
    
  parser.registerCommand("SLEFT", "iii", &steadyLeftCallback);
  parser.registerCommand("SRIGHT", "iii", &steadyRightCallback);
    
  parser.registerCommand("STOP", "", &stopCallback);

  
  
  Serial.println("Attempting to connect to WPA network...");
  Serial.print("SSID: ");
  Serial.println(ssid);

  
  
  gpSerial.begin(9600); // port de comunication avec le capteur GPS

  int status = WiFi.begin(ssid, pass);
  
  if ( status != WL_CONNECTED) {
    
    Serial.println("Couldn't get a wifi connection");
    
    // don't do anything else:
    while(true);
  }
  
  else {
    Serial.println("Connected to wifi");
    
    Serial.println("\nStarting connection...");
    
    // if you get a connection, report back via serial:
    
      Serial.print("Attempting to connect to the MQTT broker: ");
      Serial.println(broker);

      if (!mqttClient.connect(broker, port)) {
        Serial.print("MQTT connection failed! Error code = ");
        Serial.println(mqttClient.connectError());

        while (1);
      }
      Serial.println("You're connected to the MQTT broker!");
      Serial.println();

      mqttClient.onMessage(onMqttMessage);
      
      // Subscribe to commands topic
      Serial.print("Subscribing to topic: ");
      Serial.println(topic);
      
      mqttClient.subscribe(topic);
  }}

void loop() {
  
  mqttClient.poll();
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  // if the server's disconnected, stop the client:
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    for(;;)
      ;
  }
//
//  while(gpSerial.available()){
//     
//    
//    if(gps.encode(gpSerial.read())){ 
//      
//      gps.f_get_position(&latitude,&longitude); // get latitude and longitude
//    
//      Serial.print(latitude);
//      Serial.print(",");
//      Serial.println(longitude);
//     }
//  }

}




// onMqttMessage
void onMqttMessage(int messageSize) {
  // we received a message, print out the topic and contents
//  Serial.println("Received a message with topic '");
//  Serial.print(mqttClient.messageTopic());
//  Serial.print("', length ");
//  Serial.print(messageSize);
//  Serial.println(" bytes:");

  // use the Stream interface to print the contents
  while (mqttClient.available()) {
      char c;
      char line[128];
      int i;
      for(i = 0; i < 128; i++){
        line[i] = '\0';
      }
      i = 0;
      while ((c = mqttClient.read()) != '\n' && i <= 128){
        line[i] = c;
      }

      line[i] = '\0';
      String command = line;
//      
//      char response[Parser::MAX_RESPONSE_SIZE];
//      parser.processCommand(line, response);
//      
//      Serial.println(response);
     
  }
//  Serial.println();
//  Serial.println();
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
