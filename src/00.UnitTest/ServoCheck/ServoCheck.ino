/** 
 *  Copyright (c) 2019 jhlee@robotnmore.com(signaled@gmail.com)
 *  
 *  See the file license.txt for copying permission  
 */

/**
 * JellibiAGV 보드용 샘플 
 *   D2 : RFID Reader control : SPI SS(Slave Select)
 *   D3 : Buzzer (Piezo Speaker)
 *   D4 : RFID Reader control : Module Reset
 *   D5 : Left side DC MOTOR : PWM  : 회전속도제어
 *   D6 : Right side DC MOTOR : PWM : 회전속도제어
 *   D7 : Left side DC MOTOR : GPIO : 회전방향제어
 *   D8 : Right side DC MOTOR : GPIO : 회전방향제어
 *   D9 : Servo 1  : PWM
 *   D10 : Servo 2 : PWM
 *   D11 : RFID Reader control : SPI MOSI 
 *   D12 : RFID Reader control : SPI MISO 
 *   D13 : RFID Reader control : SPI SCL
 *   A0 : 전면 센서보드 가운데 IR 센서 : ADC
 *   A1 : 전면 센서보드 왼쪽 IR 센서 : ADC
 *   A2 : 전면 센서보드 오른쪽 IR 센서 : ADC 
 *   A6 : 바닥면 센서보드 왼쪽 IR 센서 : ADC
 *   A7 : 바닥면 센서보드 오른쪽 IR 센서 : ADC
 */

#include <Servo.h>

#define SERVO1_PIN 9 //Servo1
#define SERVO1_MIN 90
#define SERVO1_MAX 00
#define SERVO1_DEF SERVO1_MIN

#define SERVO2_PIN 10 // Servo2
#define SERVO2_MIN 80
#define SERVO2_MAX 170
#define SERVO2_DEF SERVO2_MIN 

Servo _servo1;
Servo _servo2;

void LiftUp();
void PutDown();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  _servo1.attach(SERVO1_PIN);
  _servo2.attach(SERVO2_PIN);
  delay(100);
  _servo1.write(SERVO1_DEF);
  _servo2.write(SERVO2_DEF);
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:
  LiftUp();
  PutDown();
}


void LiftUp()
{
  const int STEP=20;
  Serial.println(__FUNCTION__);
  delay(1000);
  for ( int i =0; i < STEP; i++) {
    int angle1 = map(i, 0, STEP, SERVO1_MIN, SERVO1_MAX);
    int angle2 = map(i, 0, STEP, SERVO2_MIN, SERVO2_MAX);
    _servo1.write(angle1);
    _servo2.write(angle2);
    delay(20);  
  }
}

void PutDown()
{
  const int STEP = 20;
  Serial.println(__FUNCTION__);
  delay(1000);
  for (int i=0; i<STEP;i++) {
    int angle1 = map(i,0, STEP, SERVO1_MAX, SERVO1_MIN);
    int angle2 = map(i,0, STEP, SERVO2_MAX, SERVO2_MIN);
    _servo1.write(angle1);
    _servo2.write(angle2);
    delay(20);
  }
}
