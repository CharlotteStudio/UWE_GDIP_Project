#include "AL5DRoboticArmActionSetting_Header.h"
#include "Button_Header.h"
#include "Buzzer_Header.h"
#include "LCD1602_Header.h"
#include <Wire.h> 

//#define rxPin    0 // 低電壓
//#define txPin    2 // 會出高電壓
#define relayPin   3
#define TrigPin    4
#define EchoPin    5
#define button_pin 7
#define buzzerPin  9
//#define sda_pin  15
//#define scl_pin  16

enum ActionState { Init, Ready, Start, Pause, Stop, StartCatch, Catched };
ActionState actionState = Init;

const int targetDistance = 5;

float cm;
bool relayOn = false;

float nextCollectTime              = 0;
const float collectTime            = 100;
float conveyorTimeout              = 0;
const float conveyorTimeoutWaiting = 10000;
float onHoldButtonTime             = 0;
const float onHoldButtonWaiting    = 5000;


int inCount  = 0;
int outCount = 0;

bool IsItemReceived() { return inCount > outCount; }

void OnUpCallback() {
  if (actionState == Ready && millis() < onHoldButtonTime)
  {
    Print_LCD("Start Program", "Conveyor Running");
    StartConveyor();
  }
  onHoldButtonTime = 0;
}

void OnDownCallback() {
  onHoldButtonTime = millis() + onHoldButtonWaiting;
}

void OnHoldCallback() {
  if (actionState == Init)  return;
  if (actionState == Ready) return;

  if (millis() > onHoldButtonTime){
    StartBuzzerLoop(2, 200, 50, 5);
    Print_LCD("Stop Action","Reset System");
    digitalWrite(relayPin, LOW);
    relayOn = false;
    actionState = Ready;
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
  SetUpOnDownCallback(0, &OnDownCallback);
  SetUpOnHoldCallback(0, &OnHoldCallback);

  InitBuzzer(buzzerPin);
  Init_LCD1602();
  Print_LCD("Initialization");

  relayOn = false;

  SetupFlowAction();
  InitAL5D();
  RecoverArmPosition();

  actionState = Ready;
  Print_LCD("Put Button Start");
}

void loop()
{
  delay(5);

  if (actionState == Init)        return;

  UpdateBuzzer();
  CheckButtonOnClick(0, button_pin);

  if (actionState == Ready)       return;

  if (actionState == Start || actionState == Catched) CheckUltrasound();

  if (actionState == Start)       CheckConveyorTimeout();

  if (actionState == StartCatch)  StartArmMovement();

  if (actionState == Catched)     StartArmMoveToBox();
}

void CheckUltrasound(){
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
    Serial.println(IsItemReceived() ? "In Target Distance !" : "Out Target Distance !");

    if (IsItemReceived()) {
      if(relayOn) { StartCatchAction(); }
      else {
        if (actionState == Start){
          StartCatchAction();
        }
        if (actionState == Catched) {
          CatchFailAction();
        }
      }
    } else {
      if(!relayOn)
      {
        digitalWrite(relayPin, HIGH);
        Print_LCD("Conveyor Running");
        relayOn = true;
      }
    }
    
    inCount  = 0;
    outCount = 0;
    nextCollectTime = millis() + collectTime;
  }
}

void StartConveyor(){
  conveyorTimeout = millis() + conveyorTimeoutWaiting;
  actionState = Start;
}

void StartCatchAction(){
  digitalWrite(relayPin, LOW);
  Print_LCD("Stop Conveyor", "Start Catching");
  relayOn = false;
  actionState = StartCatch;
}

void StartArmMovement(){
  Print_LCD("Robotic Arm", "Start Moving");
  SendoutMoverData(catchAction_1);
  delay(2000);

  SendoutMoverData(catchAction_2);
  delay(2000);

  Print_LCD("Robotic Arm", "Start Catch");

  SendoutMoverData(catchAction_3);
  delay(2000);

  SendoutMoverData(catchAction_4);
  delay(2000);

  actionState = Catched;
  conveyorTimeout = 0;
}

void StartArmMoveToBox(){
  Print_LCD("Robotic Arm", "Move To Box 1");
  SendoutMoverData(release_1_Action_1);
  delay(2000);

  SendoutMoverData(release_1_Action_2);
  delay(2000);

  Print_LCD("Robotic Arm", "End Catching");

  SendoutMoverData(idleAction);
  delay(2000);

  StartConveyor();
}

void CatchFailAction(){
  Print_LCD("Error", "Catch Fail !");
  StartBuzzerLoop(2, 100, 50, 2);
  digitalWrite(relayPin, HIGH);
  relayOn = true;
  StartConveyor();
  RecoverArmPosition();
  StartConveyor();
}

void RecoverArmPosition(){
  delay(2000);
  SendoutMoverData(idleAction);
  delay(2000);
}

void CheckConveyorTimeout(){
  if (millis() > conveyorTimeout)
    ConveyorTimeoutAction();
}

void ConveyorTimeoutAction(){
  StartBuzzerLoop(2, 100, 50, 3);
  Print_LCD("Error !", "No Item !");
  StartConveyor();
}