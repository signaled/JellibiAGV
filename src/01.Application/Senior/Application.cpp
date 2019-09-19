/** 
 *  Copyright (c) 2019 Robot&More, jhlee@robotnmore.com(signaled@gmail.com)
 *  
 *  See the file license.txt for copying permission  
 */

#include <Arduino.h>
#include "Application.h"
#include "Tone_note.h"

Application::Application()
: _iScenarioIdx(0)
, _state(STATE_NONE)
, _reader(s_RFIDReaderSlaveSelect, s_RFIDReaderReset)
, _lifter(s_LiftServo1, s_LiftServo2)
, _drive(s_RightWheelPWM, 
         s_RightWheelDir, 
         s_LeftWheelPWM, 
         s_LeftWheelDir,
         s_SensorFrontRight, 
         s_SensorFrontCenter, 
         s_SensorFrontLeft, 
         s_SensorBottomRight, 
         s_SensorBottomLeft)
{
}

void Application::Init()
{
    _drive.Init();
    _lifter.Init();
    _reader.Init();    
     pinMode(s_Buzzer, OUTPUT);
     pinMode(s_UserButton, INPUT);
    Serial.println("Application Initialized");
}

void Application::ResetState(APP_STATE newstate)
{
    if (_state != newstate) {
        if (Serial) {
            switch (newstate) {
            case STATE_NONE:       Serial.println("ENTER STATE_NONE");       break;
            case STATE_RFIDREAD:   Serial.println("ENTER STATE_RFIDREAD");   break;
            case STATE_CMDLIST:    Serial.println("ENTER STATE_CMDLIST");    break;
            case STATE_LINECOUNTER:Serial.println("ENTER STATE_LINECOUNTER");break;
            case STATE_LINETRACER: Serial.println("ENTER STATE_LINETRACER"); break;
           }
        }
    }
    _state = newstate;
}

Application::~Application()
{
    ResetState(STATE_NONE);
}

/* SetupScenario1

    Start Pos : A1
    PickUp Pos : C2
    Target Pos : B8
    Obstacle Pos : B4, B5, C4, C5, D4, D5
*/
void Application::SetupScenario1()
{
    AlarmBeep1();
    _cmdScenario[0] = eInit;
    _cmdScenario[1] = eTr2;
    _cmdScenario[2] = eSleep;
    _cmdScenario[3] = eTurnRight;
    _cmdScenario[4] = eSleep;
    _cmdScenario[5] = eTr2;
    _cmdScenario[6] = eStepForward;
    _cmdScenario[7] = eSVLiftUp;
    _cmdScenario[8] = eSleep;
    _cmdScenario[9] = eTr2;
    _cmdScenario[10] = eSleep;
    _cmdScenario[11] = eTurnLeft;
    _cmdScenario[12] = eSleep;
    _cmdScenario[13] = eTr5;
    _cmdScenario[14] = eSleep;
    _cmdScenario[15] = eTurnLeft;
    _cmdScenario[16] = eSleep;
    _cmdScenario[17] = eTr3;
    _cmdScenario[18] = eSleep;
    _cmdScenario[19] = eTurnRight;
    _cmdScenario[20] = eSleep;
    _cmdScenario[21] = eTr1;
    _cmdScenario[22] = eStepForward;
    _cmdScenario[23] = eSVLiftDown;
    _cmdScenario[24] = eBeep;
    _cmdScenario[25] = eNONE;
    _iScenarioIdx = 0;
}

/* SetupScenario1

    Start Pos : E1
    PickUp Pos : G7
    Target Pos : F2
    Obstacle Pos : F4 F5 F6 G4 G5 G6
*/

void Application::SetupScenario2()
{
    AlarmBeep2();
    _cmdScenario[0] = eInit;
    _cmdScenario[1] = eTr7;
    _cmdScenario[2] = eSleep;
    _cmdScenario[3] = eTurnRight;
    _cmdScenario[4] = eSleep;
    _cmdScenario[5] = eTr2;
    _cmdScenario[6] = eStepForward;
    _cmdScenario[7] = eSVLiftUp;
    _cmdScenario[8] = eSleep;
    _cmdScenario[9] = eTr1;
    _cmdScenario[10] = eSleep;
    _cmdScenario[11] = eTurnLeft;
    _cmdScenario[12] = eSleep;
    _cmdScenario[13] = eTr5;
    _cmdScenario[14] = eSleep;
    _cmdScenario[15] = eTurnRight;
    _cmdScenario[16] = eSleep;
    _cmdScenario[17] = eTr2;
    _cmdScenario[18] = eStepForward;
    _cmdScenario[19] = eSVLiftDown;
    _cmdScenario[20] = eBeep;
    _cmdScenario[21] = eNONE;
    _iScenarioIdx = 0;
}

void Application::Applinecounter()
{
    ResetState(STATE_LINECOUNTER);
    _drive.ResetLineCounter();
    _drive.Forward();
}

void Application::Applinetracer()
{
    ResetState(STATE_LINETRACER);
}

void Application::AppCommandList()
{
    ResetState(STATE_CMDLIST);
    _iScenarioIdx = 0;
}

void Application::AppRFIDReader()
{
    ResetState(STATE_RFIDREAD);
}

void Application::RunOnce() 
{
    _drive.Sensing();
    if (_state == STATE_NONE) {
    } else if (_state == STATE_LINECOUNTER) {
        LineCounter();
    } else if (_state == STATE_LINETRACER) {
        _drive.LineTrace();
    } else if (_state == STATE_CMDLIST) {
        ProcessCmdList();
    } else if (_state == STATE_RFIDREAD) {
        ProcessRFIDRead();
    }
}

void Application::AlarmBeep()
{
    tone(s_Buzzer, NOTE_C4,150);
    delay(150);
    tone(s_Buzzer, NOTE_E4, 150);
    delay(150);
    tone(s_Buzzer, NOTE_G4, 150);
    delay(150);
}

void Application::AlarmBeep1()
{
    uint8_t i = 2;
    do {
        for(int note = 200; note< 2000; note+= 1 ) {
            tone(s_Buzzer, note, 15);
        }
        for (int note = 2000; note > 200; note-= 1) {
            tone(s_Buzzer, note, 15);
        }
    }while ( i--);
}


void Application::AlarmBeep2()
{
    uint8_t i = 3;
    do {
        for(int note = 200; note< 3000; note+= 4 ) {
            tone(s_Buzzer, note, 10);
        }
        for (int note = 3000; note > 200; note-= 4) {
            tone(s_Buzzer, note, 10);
        }
    }while ( i--);
}

void Application::LineCounter() 
{
    _drive.LineTrace();
    int8_t nLine = _drive.LineCounter();
    if(Serial) {
        Serial.println(String("Line :")+ String(nLine));
    }
}

void Application::ProcessRFIDRead()
{
    if (_reader.Read()) {
        String strUid = _reader.getUid();
        Serial.println(strUid);
        if (strUid.compareTo(RFIDUID_SCENARIO1) == 0) {
            Serial.println("Scenario 1");
            SetupScenario1();
            AppCommandList();
        } else if (strUid.compareTo(RFIDUID_SCENARIO2) == 0) {
            Serial.println("Scenario 2");
            SetupScenario2();
            AppCommandList();
        } else {
            Serial.println("Unknown RFID Card");
        }
    }
}

void Application::ProcessCmdList()
{
    CMD curCmd = _cmdScenario[_iScenarioIdx];
    if (curCmd !=  eNONE) {
        if (curCmd == eInit) {
            _drive.InitSensor();
            _iScenarioIdx++;
        } else if (curCmd == eTurnLeft) {
            _drive.PivotTurnLeft();
            _iScenarioIdx++;
        } else if (curCmd == eTurnRight) {
            _drive.PivotTurnRight();
            _iScenarioIdx++;
        } else if (curCmd == eStepForward) {
            _drive.StepForward(STEP_FORWARD_DURATION);
            _iScenarioIdx++;
        } else if (curCmd == eStepBackward) {
            _drive.StepBackward(STEP_BACKWARD_DURATION);
            _iScenarioIdx++;
        } else if (curCmd == eStop) {
            _drive.Stop();
            _iScenarioIdx++;
        } else if (curCmd == eSVLiftUp) {
            _lifter.LiftUP();
            _iScenarioIdx++;
        } else if (curCmd == eSVLiftDown) {
            _lifter.LiftDown();
            _iScenarioIdx++;
        } else if (curCmd == eSleep ) {
            delay(SLEEP_DURATION);
            _iScenarioIdx++;
        } else if (curCmd == eBeep) {
            AlarmBeep();
            _iScenarioIdx++;
        } else {
            _drive.LineTrace();
            if (curCmd == _drive.LineCounter()) {
                _drive.Stop();
                _drive.ResetLineCounter();
                _iScenarioIdx++;
            }
        }
    } else {
        AppRFIDReader();
    }
}

