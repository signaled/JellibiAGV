/** 
 *  Copyright (c) 2019 Robot&More, jhlee@robotnmore.com(signaled@gmail.com)
 *  
 *  See the file license.txt for copying permission  
 */

#include "Drive.h"

Drive::Drive(uint8_t rightWheelPwm, 
             uint8_t rightWheelDir, 
             uint8_t leftWheelPwm, 

             uint8_t leftWheelDir,
             uint8_t pinFrontRight, 
             uint8_t pinFrontCenter, 
             uint8_t pinFrontLeft, 
             uint8_t pinBottomRight, 
             uint8_t pinBottomLeft)
: _initRightValue(0)
, _initLeftValue(0)
, _centerValue(0)
, _rightValue(0)
, _leftValue(0)
, _nLineCounter(0)
, _rightWheelPwm(rightWheelPwm)
, _rightWheelDir(rightWheelDir)
, _leftWheelPwm(leftWheelPwm)
, _leftWheelDir(leftWheelDir)
, _pinFrontRight(pinFrontRight)
, _pinFrontCenter(pinFrontCenter)
, _pinFrontLeft(pinFrontLeft)
, _pinBottomRight(pinBottomRight)
, _pinBottomLeft(pinBottomLeft)
{
}

void Drive::Init()
{
    pinMode(_rightWheelDir, OUTPUT);
    pinMode(_leftWheelDir, OUTPUT);
    pinMode(_rightWheelPwm, OUTPUT);
    pinMode(_leftWheelPwm, OUTPUT);
    pinMode(_pinFrontRight, INPUT);
    pinMode(_pinFrontCenter, INPUT);
    pinMode(_pinFrontLeft, INPUT);
    pinMode(_pinBottomRight, INPUT);
    pinMode(_pinBottomLeft, INPUT);
}

void Drive::InitSensor() 
{
    Stop();
    _initLeftValue = GetLeft();
    _initRightValue = GetRight();
    _centerValue = GetRight() - GetLeft();
}

void Drive::Sensing()
{
    _rightValue = GetRight();
    _leftValue = GetLeft();
}

// Half Pivot Turn for JellibiAGV
void Drive::PivotTurnLeft() 
{
    if (Serial) Serial.println("Enter Pivot turn Left");
    analogWrite( _leftWheelPwm, 0);
    analogWrite( _rightWheelPwm, 0);
    delay(10);
    digitalWrite(_leftWheelDir, 0);
    digitalWrite(_rightWheelDir, 0);
    delay(10);
    analogWrite( _leftWheelPwm, _turnHighSpeed);
    analogWrite( _rightWheelPwm, _turnHighSpeed);
    delay(120);
    analogWrite( _leftWheelPwm, _turnLowSpeed);
    analogWrite( _rightWheelPwm, _turnSpeed);
    delay(600);
    analogWrite( _leftWheelPwm, 0);
    analogWrite( _rightWheelPwm, 0);

    if (Serial) Serial.println("Leave Pivot turn Left");
}

void Drive::Forward()
{
    delay(10);
    digitalWrite(_leftWheelDir, 1);
    digitalWrite(_rightWheelDir, 0);
    delay(10);
    analogWrite( _leftWheelPwm, _speed);
    analogWrite( _rightWheelPwm, _speed);
}

void Drive::Backward()
{
    delay(10);
    digitalWrite(_leftWheelDir, 0);
    digitalWrite(_rightWheelDir, 1);
    delay(10);
    analogWrite( _leftWheelPwm, _speed);
    analogWrite( _rightWheelPwm, _speed);
}

// Half Pivot Turn for JellibiAGV
void Drive::PivotTurnRight()
{
    if (Serial) Serial.println("Enter Pivot turn Right");
    analogWrite( _leftWheelPwm, 0);
    analogWrite( _rightWheelPwm, 0);
    delay(10);
    digitalWrite(_leftWheelDir, 1);
    digitalWrite(_rightWheelDir, 1);
    delay(10);
    analogWrite( _rightWheelPwm, _turnHighSpeed);
    analogWrite( _leftWheelPwm, _turnHighSpeed);
    delay(120);
    analogWrite( _rightWheelPwm, _turnLowSpeed);
    analogWrite( _leftWheelPwm, _turnSpeed);
    delay(600);
    analogWrite( _leftWheelPwm, 0);
    analogWrite( _rightWheelPwm, 0);
    if (Serial) Serial.println("Leave Pivot turn Right");
}

void Drive::LineTrace() 
{
    int16_t turnSpeed = ((_rightValue-_leftValue) - _centerValue)/8;
    int16_t leftSpeed = _speed + turnSpeed;
    int16_t rightSpeed = _speed - turnSpeed;

    digitalWrite( _leftWheelDir, (leftSpeed > 0)?1:0);
    digitalWrite( _rightWheelDir, (rightSpeed > 0)?0:1);
    analogWrite( _leftWheelPwm, round(abs(leftSpeed)));
    analogWrite( _rightWheelPwm, round(abs(rightSpeed)));
}

uint16_t Drive::LineCounter() 
{
    static uint8_t bSignalHigh = 0;
    if (_rightValue > LINEDETECT_THRESHOLD_MIN && _leftValue > LINEDETECT_THRESHOLD_MIN ) {
        if (bSignalHigh==0) {
            _nLineCounter++;
            if (Serial) {
                Serial.println(String("LINE!!! :")+ String(_nLineCounter));
            }
            bSignalHigh = 1;
        }
    } else if (_rightValue < BLANKDETECT_THERSHOLD_MAX && _leftValue < BLANKDETECT_THERSHOLD_MAX) {
        if (bSignalHigh) {
            bSignalHigh = 0;
        }
    }
    return _nLineCounter;
}

void Drive::ResetLineCounter()
{
    _nLineCounter = 0;
}

void Drive::Stop() 
{
    analogWrite( _leftWheelPwm, 0);
    analogWrite( _rightWheelPwm, 0);
}

void Drive::StepForward(uint32_t timeInMs)
{
    Forward();
    delay(timeInMs);
    Stop();
}

void Drive::StepBackward(uint32_t timeInMs)
{
    Backward();
    delay(timeInMs);
    Stop();
}


int16_t Drive::GetLeft() 
{
    return analogRead(_pinBottomLeft);
}

int16_t Drive::GetRight() 
{
    return analogRead(_pinBottomRight);
}


int16_t Drive::GetFrontLeft()
{
    return analogRead(_pinFrontLeft);
}

int16_t Drive::GetFrontRight()
{
    return analogRead(_pinFrontRight);
}

int16_t Drive::GetFrontCenter()
{
    return analogRead(_pinFrontCenter);  
}
