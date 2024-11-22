#include "AL5DRobotArm_Header.h"

AL5DMover catchAction;
AL5DMover releaseAction;

bool isCatched = false;

void setup() {
  Serial.begin(115200);

  isCatched = false;

  InitAL5D();
  catchAction.point_2 = -20;
  catchAction.point_4 = -45;
  catchAction.time = 1000;

  releaseAction.point_2 = 20;
  releaseAction.point_4 = 0;
  releaseAction.time = 1000;
}

void loop() {
  if (millis() % 4000 == 0)  {
    if (isCatched){
      SendoutMoverData(releaseAction);
      isCatched = false;
    }
    else{
      SendoutMoverData(catchAction);
      isCatched = true;
    }
  }

  CheckRelease();
}