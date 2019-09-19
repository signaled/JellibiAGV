# 2019 WCRC 시니어부 샘플 
**Jellibi** **A**utomated **G**uided **V**ehicle

### 구현 내용

Senior 프로젝트에서는 두개의 빠레뜨를 이동시키는 내용을 구현합니다. 
프로젝트를 구성하는 파일별로 구현된 내용은 아래 테이블과 같습니다. 

| 클래스 명(파일명)                | 동작 내용                                                    |
| -------------------------------- | ------------------------------------------------------------ |
| (Senior.ino)                     | Application 클래스의 오브젝트를 만들어 초기화합니다.<br />아두이노에서는 Senior.ino 파일의 setup() 함수와 loop() 함수를 실행시간에 호출합니다. |
| Application (Application.h/.cpp) | 모든 H/W 를 제어하는 클래스들을 만들고 초기화 하고 필요한 제어를 실행합니다.<br />Senior 샘플에서는 Application 클래스의 구현 중 RFID 를 읽는 동작 (STATE_RFID 상태)과 미리 정해둔 명령들을 실행하는 동작 (STATE_CMDLIST 상태) 을 사용합니다. |
| RFIDReader(RFIDReader.h/.cpp)    | RFID 를 읽어들이고 TAG 를 읽었을 경우 읽어 들인 TAG 의 UID 를 저장합니다. Application 클래스에서 이 클래스를 이용하여 RFID TAG 를 구별에 필요한 UID 를 알아냅니다. |
| LiftControl(LiftControl.h/.cpp)  | 빠레뜨를 들고 내릴 때 사용하는 네개의 Servo 모터를 제어하는데 필요한 모든 코드를 구현하고 있습니다. Application 클래스에서 이 클래스를 이용하여 빠레뜨를 올리고 / 내릴 수 있습니다. |
| Drive (Drive.h/.cpp)             | JellibiAGV 에 장착되는 다섯개의 IR 센서 와 두개의 DC 모터를 제어합니다. 기본적인 전/후진과 라인트레이서 기능과 라인카운트 기능 |
| (Define.h)                       | Application 은 RFID 를 읽어 들여(STATE_RFID)  움직임을 명령리스트로 만들어 동작합니다. 이 명령리스트에 들어갈 명령어 목록이 이 파일에 정의 되어 있습니다.<br /> 또 "시작 RFID TAG" 의 UID 도 정의하고 있으며 몇 개의 움직임에 사용되는 시간값등의 상수가 정의되어 있습니다. |

### STATE_RFID, STATE_CMDLIST

```Senior.ino``` 의 ```setup()``` 함수에서는 ```Application::AppRFIDReader()``` 함수를 호출하여 ```Application``` 을 RFID 를 무한정 읽어들이는 상태로 만듭니다.  이 상태를 ```STATE_RFID``` 상태로 코드에서는 정의하고 있습니다. 

```Senior.ino``` 의 ```loop()``` 함수에서는 매번 ```Application::RunOnce()``` 함수를 호출하는데 현재 ```Application``` 의 상태에 따라 각각 ```Application``` 의 각기 다른 함수를 실행합니다. 

```STATE_RFID``` 상태에서는 ```Application::ProcessRFIDRead``` 함수를 호출 합니다. 

~~~c++
void Application::ProcessRFIDRead()
{
    if (_reader.Read()) {
        String strUid = _reader.getUid();
        Serial.println(strUid);
        if (strUid.compareTo(RFIDUID_SCENARIO1) == 0) {
            Serial.println("Scenario 1");
            SetupScenario1(); // A1 에서 C2 로 이동하여 빠레뜨 획득, 이후 B8 로 배송 
            AppCommandList();
        } else if (strUid.compareTo(RFIDUID_SCENARIO2) == 0) {
            Serial.println("Scenario 2");
            SetupScenario2(); // E1 에서 G7 로 이동하여 빠레뜨 획득, 이후 F2 로 배송
            AppCommandList();
        } else {
            Serial.println("Unknown RFID Card");
        }
    }
} 
~~~

코드에서는 RFID 에서 읽어들인 UID 를 ```Define.h``` 에서 선언한 두개의 RFID TAG 와 비교합니다. 
```Application::SetupScenario1()``` 함수와 ```Application::SetupScenario2()``` 함수는 주석에 적힌 바와 같은 동작을 하는 명령어 목록을 ```Application::_cmdScenario``` 에 순서대로 저장합니다. 

```Application::AppCommandList()``` 함수는 ```Application``` 을 앞서 저장한 명령을 순서대로 동작시키는 ```STATE_CMDLIST``` 상태로 변경시킵니다. 

이제 다음번 ```Senior.ino``` 의 ```loop()``` 펑션이 호출 될 때부터 ```Application::ProcessCmdList()``` 함수가 호출됩니다. 

### Application::ProcessCmdList()

```Applicaton::SetupScenario1()```  과  ```Applicaton::SetupScenario1()``` 함수에서는 인식된 태그에 따라 다른 동작목록을 ```Application::_cmdScenario[]``` 배열에 저장합니다. 

이 명령어 목록을 순차적으로 처리하는 함수가 ```Application::ProcessCmdList()``` 입니다. 

### ```CMD```

```Define.h``` 에 정의된 명령어 목록 및 동작 내용은 아래 표와 같습니다. 

| 명령어 (값)        | 동작                                                         |
| ------------------ | ------------------------------------------------------------ |
| eNONE(0)           | 명령목록의 끝을 구분하는데 사용함 ```Application::ProcessCmdList()``` 에서 이 명령을 만나면 ```Application``` 의 상태를 다시 RFID TAG 를 읽어들이는 상태로 변경함 |
| eTr1(1)            | 라인트레이서를 동작하며 이동 중 만나는 첫 번째 라인까지 이동함 |
| eTr2(2)            | 라인트레이서를 동작하며 이동 중 만나는 두 번째 라인까지 이동함 |
| ....               |                                                              |
| eTr15(15)          | 라인트레이서를 동작하며 이동 중 만나는 15 번째 라인까지 이동함 |
| eInit(100)         | 라인트레이서 동작을 위한 기준값을 센서에서 읽어들여 저장함 <br />라인트레이서 동작 전에 반드시 이 명령이 실행되어 야 함 |
| eTurnRight(101)    | 우회전 실행 후 정지 (위 eTr(1~15) 명령으로 정지된 위치에서 우회전) |
| eTurnLeft(102)     | 좌회전 실행 후 정지                                          |
| eStepForward(103)  | 빠레뜨를 들기위해 정지위치에서 전진, 빠레뜨의 중심점과 JellibiAGV 의 빠레뜨를 올리는 지지대의 중심점과의 차이로 인해 라인트레이서로 이동 후 이 명령을 통해 대략적으로 중심점 맞춤 |
| eStepBackward(104) | eStepForward 이동 후 다시 라인트레이싱으로 이동가능한 위치로 후진 |
| eStop(105)         | 정지                                                         |
| eSVLiftUp(106)     | Servo 모터를 제어하여 빠레뜨를 들어올림                      |
| eSVLiftDown(107)   | Servo 모터를 제어하여 빠레뜨를 내려놓음                      |
| eRFID(108)         | 사용하지 않음                                                |
| eSleep(109)        | 1초간 대기, 빠레뜨를 들어올린 후, 좌/우회전 하기 전 빠레뜨가 흔들리거나 떨어지지 않도록 명령을 사용 |
| eBeep(110)         | 부져 알람                                                    |

끝.