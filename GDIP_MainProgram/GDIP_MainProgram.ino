#include "AL5DRoboticArmActionSetting_Header.h"
#include "Button_Header.h"
#include "LCD1602_Header.h"
#include <Wire.h> 

//#define rxPin    0 // 低電壓
//#define txPin    2 // 會出高電壓
#define relayPin   3
#define TrigPin    4
#define EchoPin    5
#define button_pin 7
//#define sda_pin  15
//#define scl_pin  16

enum ActionState { Init, Ready, Start, Pause, Stop, Catching };
ActionState actionState = Init;

const int targetDistance = 5;

float cm;
bool relayOn = false;

float nextCollectTime = 0;
float collectTime = 200;

int inCount  = 0;
int outCount = 0;


void OnUpCallback() {
  if (actionState == Ready)
  {
    Print_LCD("Start Program");
    actionState = Start;
  }
}

void setup()
{
  Serial.begin(115200);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(relayPin, OUTPUT);
  pinMode(button_pin, INPUT_PULLUP);

  digitalWrite(relayPin, LOW);
  SetUpOnUpCallback(0, &OnUpCallback);

  Init_LCD1602();
  Print_LCD("Initialization");

  relayOn = false;

  SetupFlowAction();
  InitAL5D();
  delay(2000);
  SendoutMoverData(idleAction);
  delay(2000);

  actionState = Ready;
  Print_LCD("Put Button Start");
}

void loop()
{
  if (actionState == Init)  return;

  CheckButtonOnClick(0, button_pin);

  if (actionState == Ready) return;

  Print_LCD("Conveyor Running");

  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  cm = pulseIn(EchoPin, HIGH) * 0.0170145;

  if(cm < 1000){
    //Serial.print("The Distance :");
    //Serial.print(cm);
    //Serial.println("cm");
    cm < targetDistance ? inCount++ : outCount++;
  } else {
    Serial.println("Error Detect...");
  }
  
  if (millis() > nextCollectTime)
  {
    Serial.println(inCount > outCount ? "In Target Distance !" : "Out Target Distance !");

    if (inCount > outCount){
      if(relayOn){
        digitalWrite(relayPin, LOW);
        relayOn = false;
        Print_LCD("Stop Conveyor", "Start Catching");
        Serial.println("Stop Conveyor");
        Serial.println("Start Catching");
        delay(1000);

        Print_LCD("Robotic Arm", "Start Moving");
        SendoutMoverData(catchAction_1);
        delay(2000);

        SendoutMoverData(catchAction_2);
        delay(2000);

        SendoutMoverData(catchAction_3);
        delay(2000);

        Print_LCD("Robotic Arm", "Start Catch");

        SendoutMoverData(catchAction_4);
        delay(2000);

        SendoutMoverData(release_1_Action_1);
        delay(2000);

        Print_LCD("Robotic Arm", "Move To Box 1");

        SendoutMoverData(release_1_Action_2);
        delay(2000);

        Print_LCD("Robotic Arm", "End Catching");

        SendoutMoverData(idleAction);
        delay(2000);
      }
    } else {
      if(!relayOn){
        digitalWrite(relayPin, HIGH);
        relayOn = true;

        Print_LCD("Conveyor Running");
        Serial.println("Start Conveyor");
      }
    }
    inCount  = 0;
    outCount = 0;
    nextCollectTime = millis() + collectTime;
  }


  delay(10);
}
