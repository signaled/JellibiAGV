/** 
 *  Copyright (c) 2019 Robot&More, jhlee@robotnmore.com(signaled@gmail.com)
 *  
 *  See the file license.txt for copying permission  
 */

#ifndef __DEFINE_H__
#define __DEFINE_H__
#include <Arduino.h>

/* 
     Playground 


      1       2       3       4       5       6       7       8
      ┬       ┬       ┬       ┬       ┬       ┬       ┬       ┬
 A  ├─┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼─┤
      │       │       │       │       │       │       │       │
      │       │       │       │       │       │       │       │
 B  ├─┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼─┤
      │       │       │       │       │       │       │       │
      │       │       │       │       │       │       │       │
 C  ├─┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼─┤
      │       │       │       │       │       │       │       │
      │       │       │       │       │       │       │       │
 D  ├─┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼─┤
      │       │       │       │       │       │       │       │
      │       │       │       │       │       │       │       │
 E  ├─┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼─┤
      │       │       │       │       │       │       │       │
      │       │       │       │       │       │       │       │
 F  ├─┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼─┤
      │       │       │       │       │       │       │       │
      │       │       │       │       │       │       │       │
 G  ├─┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼─┤
      │       │       │       │       │       │       │       │
      │       │       │       │       │       │       │       │
 H  ├─┼───────┼───────┼───────┼───────┼───────┼───────┼───────┼─┤
      ┴       ┴       ┴       ┴       ┴       ┴       ┴       ┴
    Seoul  InCheon SeJong  Daejeon  Daegu  KwangJu ChunCeon Jeju

*/

#define MAX_CMD 32 

enum CMD {
    eNONE = 0,          // None 
    eTr1 = 1,           // {{{ LineTrace 
    eTr2 = 2,
    eTr3 = 3, 
    eTr4 = 4,
    eTr5 = 5, 
    eTr6 = 6, 
    eTr7 = 7, 
    eTr8 = 8,
    eTr9 = 9, 
    eTr10 = 10, 
    eTr11 = 11, 
    eTr12 = 12, 
    eTr13 = 13, 
    eTr14 = 14, 
    eTr15 = 15,         // }}} 
    eInit = 100,    // INIT LineTracer
    eTurnRight,     // Turn Right 
    eTurnLeft,      // Turn Left
    eStepForward,   // STEP
    eStepBackward,
    eStop,            // STOP
    eSVLiftUp,      // Servo Lift
    eSVLiftDown,    // Servo Down
    eRFID,          // Start Card detect
    eSleep,         // Sleep 1sec 
    eBeep           // Beep
};


#define RFIDUID_SCENARIO1 "09D0047F" 
#define RFIDUID_SCENARIO2 "86E634EF" 

#define STEP_FORWARD_DURATION       250
#define STEP_BACKWARD_DURATION      400
#define SLEEP_DURATION             1000

#define NORMAL_DRIVE_SPEED    50 

#define TURN_INITIAL_WHEEL_SPEED 90 
#define TURN_INSIDE_WHEEL_SPEED  30 
#define TURN_OUTSIDE_WHEEL_SPEED 70 

#define LINEDETECT_THRESHOLD_MIN 700 
#define BLANKDETECT_THERSHOLD_MAX 500 


#endif//__DEFINE_H__
