
/** 
 *  Copyright (c) 2019 Robot&More, jhlee@robotnmore.com(signaled@gmail.com)
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
 *   A3 : 측면 사용자 버튼 : GPI
 *   A0 : 전면 센서보드 가운데 IR 센서 : ADC
 *   A1 : 전면 센서보드 왼쪽 IR 센서 : ADC
 *   A2 : 전면 센서보드 오른쪽 IR 센서 : ADC 
 *   A6 : 바닥면 센서보드 왼쪽 IR 센서 : ADC
 *   A7 : 바닥면 센서보드 오른쪽 IR 센서 : ADC
 */
#include <JellibiButton.h>

// {{{ User Button
JellibiButton _button;
#define USER_BUTTON A3
// }}}

// {{{ Drive Function 
#define WHEEL_RIGHT_PWM   6
#define WHEEL_RIGHT_DIR   8
#define WHEEL_LEFT_PWM    5
#define WHEEL_LEFT_DIR    7

void InitWheel();
void TestDrive();
void TestTurnRight();
void TestTurnLeft();
// }}} 

// {{{ RFID Reader
// Reference : https://randomnerdtutorials.com/security-access-using-mfrc522-rfid-reader-with-arduino/
#include <deprecated.h>
#include <MFRC522.h>
#include <MFRC522Extended.h>
#include <require_cpp11.h>
#include <SPI.h>

#define RFID_SS_PIN     2
#define RFID_RESET_PIN  4 // MFRC522::UNUSED_PIN
MFRC522 _mfrc522(RFID_SS_PIN, RFID_RESET_PIN);
String _strCardUID;
void InitRFIDReader();
bool ReadRFIDReader();
// }}}

// {{{ IR (A0,A1, A2, A6, A7) 
#define IR_FRONT_LEFT_PIN     A1
#define IR_FRONT_CENTER_PIN   A0
#define IR_FRONT_RIGHT_PIN    A2
#define IR_BOTTOM_RIGHT_PIN   A7
#define IR_BOTTOM_LEFT_PIN    A6
void SensingIR();
// }}}

// {{{ Buzzer(D3) 
#include "Tone_note.h"
#define BUZZER_PIN 3 // PIEZO SPEAKER
void PlayTone();
// }}}

// {{{ Servo (D9, D10) 
#include <Servo.h>

#define SERVO1_PIN 9 //Servo1
#define SERVO1_MIN 90
#define SERVO1_MAX 00
#define SERVO1_DEF SERVO1_MIN

#define SERVO2_PIN 10 // Servo2
#define SERVO2_MIN 00
#define SERVO2_MAX 90
#define SERVO2_DEF SERVO2_MIN 

Servo _servo1;
Servo _servo2;

void InitServo();
void LiftUp();
void PutDown();

// }}}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  InitRFIDReader();
  InitServo();
  delay(1000);
  //PlayTone();
  InitWheel();
  _button.Init(USER_BUTTON, false);
}

void loop() {
  
  // put your main code here, to run repeatedly:
  if (!_button.Check())
  {
    return;
  }
  TestDrive();
  delay(1000);
  LiftUp();
  delay(1000);
  PutDown();
  delay(1000);
  while(!ReadRFIDReader()){
    delay(500);
  }
  PlayTone();
  SensingIR();
}


void InitServo()
{
  _servo1.attach(SERVO1_PIN);
  _servo2.attach(SERVO2_PIN);
  delay(100);
  _servo1.write(SERVO1_DEF);
  _servo2.write(SERVO2_DEF);
}

void LiftUp()
{
  const int STEP=20;
  Serial.println(__FUNCTION__);
 
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
  for (int i=0; i<STEP;i++) {
    int angle1 = map(i,0, STEP, SERVO1_MAX, SERVO1_MIN);
    int angle2 = map(i,0, STEP, SERVO2_MAX, SERVO2_MIN);
    _servo1.write(angle1);
    _servo2.write(angle2);
    delay(20);
  }
}

void PlayTone()
{
  // notes in the melody:
  int melody[] = {  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4};
  
  // note durations: 4 = quarter note, 8 = eighth note, etc.:
  int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 8; thisNote++) {

    // to calculate the note duration, take one second divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(BUZZER_PIN, melody[thisNote], noteDuration);

    // to distinguish the notes, set a minimum time between them.
    // the note's duration + 30% seems to work well:
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(BUZZER_PIN);
  }

}

void SensingIR()
{
  char debugMessage[64] = {0,};
  int frontRight = analogRead(IR_FRONT_RIGHT_PIN);
  int frontLeft = analogRead(IR_FRONT_LEFT_PIN);
  int frontCenter = analogRead(IR_FRONT_CENTER_PIN);
  int bottomRight = analogRead(IR_BOTTOM_RIGHT_PIN);
  int bottomLeft = analogRead(IR_BOTTOM_LEFT_PIN);
  sprintf(debugMessage, "F %04d-%04d-%04d / B %04d-%04d", frontLeft,frontCenter,  frontRight, bottomLeft,bottomRight); 
  Serial.println(debugMessage);
}

void InitRFIDReader()
{
  SPI.begin();
  _mfrc522.PCD_Init();
}

bool ReadRFIDReader()
{
  Serial.println("read");
  if (!_mfrc522.PICC_IsNewCardPresent()) {
    Serial.println("New card false");
    return false;
  }
  if (!_mfrc522.PICC_ReadCardSerial()) {
    Serial.println("card Serial false");
    return false;
  }
  Serial.println("detect");
  _strCardUID = "";
  for (byte i = 0; i < _mfrc522.uid.size; i++) {
     _strCardUID.concat(String(_mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     _strCardUID.concat(String(_mfrc522.uid.uidByte[i], HEX));
  }
  Serial.print("CARD UID : ");
  _strCardUID.toUpperCase();
  Serial.println(_strCardUID);
  return true;
}

void InitWheel()
{ 
  pinMode(WHEEL_RIGHT_PWM, OUTPUT);
  pinMode(WHEEL_RIGHT_DIR, OUTPUT);
  pinMode(WHEEL_LEFT_PWM, OUTPUT);
  pinMode(WHEEL_LEFT_DIR, OUTPUT);
}

void TestDrive()
{
  analogWrite(WHEEL_RIGHT_PWM, 0);
  analogWrite(WHEEL_LEFT_PWM, 0);

  digitalWrite(WHEEL_RIGHT_DIR, LOW);
  digitalWrite(WHEEL_LEFT_DIR, HIGH);
  delay(10);
  analogWrite(WHEEL_RIGHT_PWM, 60);
  analogWrite(WHEEL_LEFT_PWM, 60);
  delay(1500);
  analogWrite(WHEEL_RIGHT_PWM, 0);
  analogWrite(WHEEL_LEFT_PWM, 0);

  digitalWrite(WHEEL_RIGHT_DIR, HIGH);
  digitalWrite(WHEEL_LEFT_DIR, LOW);
  delay(100);
  analogWrite(WHEEL_RIGHT_PWM, 60);
  analogWrite(WHEEL_LEFT_PWM, 60);
  delay(1500);
  analogWrite(WHEEL_RIGHT_PWM, 0);
  analogWrite(WHEEL_LEFT_PWM, 0);    
}

void TestTurnRight()
{
  analogWrite(WHEEL_RIGHT_PWM, 0);
  analogWrite(WHEEL_LEFT_PWM, 0);

  digitalWrite(WHEEL_RIGHT_DIR, LOW);
  digitalWrite(WHEEL_LEFT_DIR, HIGH);
  delay(10);
  analogWrite(WHEEL_RIGHT_PWM, 0);
  analogWrite(WHEEL_LEFT_PWM, 60);
  delay(1000);
  analogWrite(WHEEL_RIGHT_PWM, 0);
  analogWrite(WHEEL_LEFT_PWM, 0);
}

void TestTurnLeft()
{
  analogWrite(WHEEL_RIGHT_PWM, 0);
  analogWrite(WHEEL_LEFT_PWM, 0);

  digitalWrite(WHEEL_RIGHT_DIR, LOW);
  digitalWrite(WHEEL_LEFT_DIR, HIGH);
  delay(10);
  analogWrite(WHEEL_RIGHT_PWM, 60);
  analogWrite(WHEEL_LEFT_PWM, 0);
  delay(1000);
  analogWrite(WHEEL_RIGHT_PWM, 0);
  analogWrite(WHEEL_LEFT_PWM, 0);
}
