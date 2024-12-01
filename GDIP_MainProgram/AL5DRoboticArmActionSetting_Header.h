#include "AL5DRoboticArm_Header.h"

const int actionTime = 1500;

AL5DMover idleAction;
AL5DMover catchAction_1;
AL5DMover catchAction_2;
AL5DMover catchAction_3;
AL5DMover catchAction_4;
AL5DMover release_1_Action_1;
AL5DMover release_1_Action_2;

void SetupFlowAction(){
    idleAction.point_0 = 4;
    idleAction.point_3 = -90;
    idleAction.point_4 = -90;
    idleAction.point_5 = -80;
    idleAction.time = actionTime;

    catchAction_1.point_0 = -66;
    catchAction_1.point_1 = 30;
    catchAction_1.point_2 = 20;
    catchAction_1.point_3 = -86;
    catchAction_1.point_4 = -90;
    catchAction_1.point_5 = -65;
    catchAction_1.time = actionTime;

    catchAction_2.point_0 = -66;
    catchAction_2.point_1 = 30;
    catchAction_2.point_2 = 30;
    catchAction_2.point_3 = -79;
    catchAction_2.point_4 = -90;
    catchAction_2.point_5 = -65;
    catchAction_2.time = actionTime;

    catchAction_3.point_0 = -66;
    catchAction_3.point_1 = 30;
    catchAction_3.point_2 = 30;
    catchAction_3.point_3 = -79;
    catchAction_3.point_4 = -20;
    catchAction_3.point_5 = -65;
    catchAction_3.time = actionTime;

    catchAction_4.point_0 = -66;
    catchAction_4.point_1 = 30;
    catchAction_4.point_2 = 20;
    catchAction_4.point_3 = -86;
    catchAction_4.point_4 = -20;
    catchAction_4.point_5 = -65;
    catchAction_4.time = actionTime;

    release_1_Action_1.point_0 = 4;
    release_1_Action_1.point_1 = -18;
    release_1_Action_1.point_2 = -22;
    release_1_Action_1.point_3 = -90;
    release_1_Action_1.point_4 = -20;
    release_1_Action_1.point_5 = -80;
    release_1_Action_1.time = actionTime;

    release_1_Action_2.point_0 = 4;
    release_1_Action_2.point_1 = -18;
    release_1_Action_2.point_2 = -22;
    release_1_Action_2.point_3 = -90;
    release_1_Action_2.point_4 = -90;
    release_1_Action_2.point_5 = -80;
    release_1_Action_2.time = actionTime;
}