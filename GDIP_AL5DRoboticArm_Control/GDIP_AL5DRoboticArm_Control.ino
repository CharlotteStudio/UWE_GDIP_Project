#include "AL5DRobotArm_Header.h"

String str = "";
int size = 7;

void setup() {
  Serial.begin(115200);

  InitAL5D();

  Serial.println("You can input command now (such as 0,20,0,0,0,0,1000)");
}

void loop() {
  str = SerialToString();

  if (str != "") {
    Serial.print("Get string : ");
    Serial.println(str);

    String* strings = SplitString(str, ',', size);

    AL5DMover action;
    action.point_0 = StringToInt(strings[0]);
    action.point_1 = StringToInt(strings[1]);
    action.point_2 = StringToInt(strings[2]);
    action.point_3 = StringToInt(strings[3]);
    action.point_4 = StringToInt(strings[4]);
    action.point_5 = StringToInt(strings[5]);
    action.time    = StringToInt(strings[6]);

    SendoutMoverData(action);
  }

  CheckRelease();
}

String SerialToString() {
  while (Serial.available()) {
    return Serial.readString();
  }
  return "";
}

int StringToInt(String s)       { return s.toInt();      } 

String* SplitString(String str, char c, int size){
  int count = 0;
  String* strings = new String[size];

  while (str.length() > 0)
  {
    int index = str.indexOf(c);
    if (index == -1)
    {
      strings[count++] = str;
      break;
    }
    else
    {
      strings[count++] = str.substring(0, index);
      str = str.substring(index + 1);
    }
  }
  return strings;
}
