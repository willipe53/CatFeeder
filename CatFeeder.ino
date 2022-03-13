#include <Preferences.h>
#include <ESP32Servo.h>
#include "esp_adc_cal.h"
#include "secrets.h"
#include <WiFiClientSecure.h>
#include <MQTTClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

#define LED      14
#define BUTTON   32
#define MAGNET   21
#define SHOULDER  0
#define ELBOW     1
#define WRIST     2

#define AWS_IOT_PUBLISH_TOPIC   "cat/status"
#define AWS_IOT_SUBSCRIBE_TOPIC "cat/commands"

WiFiClientSecure wifi_client = WiFiClientSecure();
MQTTClient mqtt_client = MQTTClient(256);
uint32_t t1;

Preferences prefs;
Servo shoulderServo;
Servo elbowServo;
Servo wristServo;
Servo servoMotor[] = {shoulderServo, elbowServo, wristServo};

int servoPins[]    = {A0, A5, A1};
String servoNames[] = { "Shoulder", "Elbow", "Wrist"};
bool isLocked = false;
int buttonState;
bool servosOn = false;

//default servo positions
int servoTop         = 2500;
int servoBottom      = 1400;
int servoMiddle      = 1400;
int servoSide        =  500;
int servoElbowOffset =  250;
int jiggleTimes      =    3;

String inputChars = "faojlhsewkibpx";
String outputNames[] = {"Full open with jiggle",                   //f
                        "Jiggles (times to ring the bell)",        //a
                        "Open the lid",                            //o
                        "Jiggle (ring the bell)",                  //j
                        "Lock or unlock magnet (1 or 0)",          //l
                        "Hand (move elbow and wrist to wrist=x)",  //h
                        "Move shoulder to x",                      //s
                        "Move elbow to x",                         //e
                        "Move wrist to x",                         //w
                        "Kill all servos",                         //k
                        "Initialize servos",                       //i
                        "Save as new open",                        //b
                        "Print all values",                        //p
                        "Purge all saved values"};                 //x

void processCommand(String cmd) {
  //commands come from AWS IoT or serial terminal
  Serial.printf("Command received: %s\n", cmd.c_str());
  String key = cmd.substring(0,1);
  int valu = cmd.substring(1,cmd.length()-1).toInt();
  if (key.equals("f"))      openLid(true);   
  else if (key.equals("o")) openLid(false);   
  else if (key.equals("a")) saveJiggleTimes(valu);   
  else if (key.equals("j")) jiggleShoulder();    
  else if (key.equals("h")) moveElbowWrist(valu);    
  else if (key.equals("s")) moveServo(SHOULDER, valu); 
  else if (key.equals("e")) moveServo(ELBOW, valu); 
  else if (key.equals("w")) moveServo(WRIST, valu); 
  else if (key.equals("l")) toggleMagnet(valu);            
  else if (key.equals("k")) killServos();            
  else if (key.equals("i")) initServos();            
  else if (key.equals("b")) saveNewOpenPosition();            
  else if (key.equals("p")) printValues();            
  else if (key.equals("x")) loadPreferences(false);  
  else Serial.printf("Unrecognized input: no values will be changed\n");
}

void toggleMagnet(int valu) {
  digitalWrite(LED, valu);
  digitalWrite(MAGNET, valu);
  isLocked = valu;
  Serial.printf("Reset magnet to %d\n", isLocked);
}

void setup() {
  Serial.begin(115200);
  connectAWS();
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(1);
  initServos();
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(MAGNET, OUTPUT);
  toggleMagnet(0);
  loadPreferences(true);
  delay(500);
  printValues();
}

void loop() {
  //send status to AWS IoT
  //publishStatus();
  //delay(4000);
  
  //receive commands from AWS IoT
  mqtt_client.loop();

  //check button
  buttonState = digitalRead(BUTTON);
  if (buttonState == 0) {
    if (isLocked) openLid(true);
    else toggleMagnet(1);
    delay(500); //debounce
  }
   
  //manual testing
  if (Serial.available() > 0) processCommand(Serial.readString());

}
