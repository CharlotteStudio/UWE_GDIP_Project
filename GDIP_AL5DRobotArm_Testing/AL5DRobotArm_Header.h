  /*  
 * Compatible ESP32 & ESP8266
 *
 * Need Download Library :
 *   EspSoftwareSerial by Dirk Kaar Peter Lerup (https://github.com/plerup/espsoftwareserial/)
 * 
 *    AL5DMover is a struct for sendout movement 
 * 
 * Reference :
 *  FlowArm PLTW, which is for preview AL5D
 *  https://wiki.lynxmotion.com/info/wiki/lynxmotion/view/ses-software/flowarm-pltw/
 *  
 *  The board for AL5D Robot Arm : lynxmotion ssc-32u usb servo controller
 *
 */
#include <SoftwareSerial.h>

#define rxPin 0 // 低電壓
#define txPin 2 // 會出高電壓

SoftwareSerial Serial_AL5D(rxPin, txPin);

char receivedata[10];
char *p=&receivedata[0];
unsigned long nextTime;

// Default time is 1 second
typedef struct {
  int point_0;    // #0 底座        數值少向左轉, 數值大向右轉
  int point_1;    // #1 底座上      數值少向下彎(向前), 數值大向上彎(向後)
  int point_2;    // #2 底座上上方  數值少向上彎, 數值大向下彎
  int point_3;    // #3 夾腕上下轉  數值少向下彎, 數值大向上彎
  int point_4;    // #4 夾          數值少向內夾, 數值大向外移
  int point_5;    // #5 夾轉動      數值少向右轉, 數值大向左彎
  int point_6;    // #6 無
  int point_7;    // #7 無
  int time;
  int delayTime;
} AL5DMover;

int ConvertToP(int value)       { return (value * 11) + 1500; }
int CheckRotateValue(int value) { return (value > 90) ? 90 : (value < -90) ? -90 : value; }
int CheckCatchValue(int value)  { return (value > 0)  ? 0  : (value < -90) ? -90 : value; }
int CheckTime(int value)        { return (value <= 0) ? 1000 : value; }
int CheckTimeDelay(int value)   { return (value < 0) ? 0 : value; }

void InitAL5D(){
  Serial_AL5D.begin(9600);
  Serial_AL5D.println("\r\nAL BL CL DL EL FL ");
}

void SendoutMoverData(AL5DMover mover){
  if (millis() < nextTime) return;

  mover.point_0   = CheckRotateValue(mover.point_0);
  mover.point_1   = CheckRotateValue(mover.point_1);
  mover.point_2   = CheckRotateValue(mover.point_2);
  mover.point_3   = CheckRotateValue(mover.point_3);
  mover.point_4   = CheckCatchValue(mover.point_4);
  mover.point_5   = CheckRotateValue(mover.point_5);
  mover.point_6   = CheckRotateValue(mover.point_6);
  mover.point_7   = CheckRotateValue(mover.point_7);

  mover.time      = CheckTime(mover.time);
  mover.delayTime = CheckTimeDelay(mover.delayTime);

  String command = "\r\#0 P";
  command += ConvertToP(mover.point_0);
  command += " #1 P";
  command += ConvertToP(mover.point_1);
  command += " #2 P";
  command += ConvertToP(mover.point_2);
  command += " #3 P";
  command += ConvertToP(mover.point_3);
  command += " #4 P";
  command += ConvertToP(mover.point_4);
  command += " #5 P";
  command += ConvertToP(mover.point_5);
  command += " #6 P";
  command += ConvertToP(mover.point_6);
  command += " #7 P";
  command += ConvertToP(mover.point_7);
  command += " T";
  command += mover.time;
  command += "\r\n";

  Serial_AL5D.println(command);
  Serial_AL5D.println("#1:0\r\nAL BL CL DL EL FL VG VH\r\n");

  Serial.print("Sendout command : ");
  Serial.println(command);
  Serial.println("#1:0\r\nAL BL CL DL EL FL VG VH\r\n");

  nextTime = millis() + mover.time + mover.delayTime;
}

void CheckRelease(){
  if (!Serial_AL5D.available()) return;
    
  *p=Serial_AL5D.read();

  if (p++>=(&receivedata[0]+sizeof(receivedata)))
  {
    p=&receivedata[0];
    Serial_AL5D.flush();
    memset(receivedata, 0, sizeof(receivedata));
  }

  if ((receivedata[6]==0xFD) && (receivedata[7]==0xFA))
  {
    Serial_AL5D.println(receivedata);
    p=&receivedata[0];
    memset(receivedata, 0, sizeof(receivedata));
  }
}