#include "AL5DRoboticArmActionSetting_Header.h"

//#define rxPin 0 // 低電壓
//#define txPin 2 // 會出高電壓
const int relayPin = 3;
const int TrigPin = 4;
const int EchoPin = 5;

const int targetDistance = 5;

float cm;
bool relayOn = false;

float nextCollectTime = 0;
float collectTime = 200;

int inCount  = 0;
int outCount = 0;

void setup()
{
  Serial.begin(115200);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(relayPin, OUTPUT);

  digitalWrite(relayPin, LOW);
  relayOn = false;

  SetupFlowAction();
  InitAL5D();
  delay(2000);
  SendoutMoverData(idleAction);
  delay(2000);
}

void loop()
{
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
        Serial.println("Stop Conveyor Belt.");
        Serial.println("Start Catching Flow.");

        SendoutMoverData(catchAction_1);
        delay(2000);

        SendoutMoverData(catchAction_2);
        delay(2000);

        SendoutMoverData(catchAction_3);
        delay(2000);

        SendoutMoverData(catchAction_4);
        delay(2000);

        SendoutMoverData(release_1_Action_1);
        delay(2000);

        SendoutMoverData(release_1_Action_2);
        delay(2000);

        SendoutMoverData(idleAction);
        delay(2000);
      }
    } else {
      if(!relayOn){
        digitalWrite(relayPin, HIGH);
        relayOn = true;
        Serial.println("Start Conveyor Belt.");
      }
    }
    inCount  = 0;
    outCount = 0;
    nextCollectTime = millis() + collectTime;
  }


  delay(10);
}
