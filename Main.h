#include <SoftwareSerial.h>
#include "StackArray.h"
#include "Motor.h"

class Main {
  private:
    static SoftwareSerial BTSerial;
    static Motor motorLeft;
    static Motor motorRight;
    static bool btConnected;
    static unsigned int speed;
    
    static char lastMove;
    static unsigned long lastMoveTime;
    static StackArray<char> historyMoves;
    static StackArray<unsigned long> historyTimes;
    static bool isRecordingHistory;
    
  public:
    static void setup() {
      BTSerial.begin(9600);

      pinMode(PIN_LED_FRONT, OUTPUT);
      pinMode(PIN_LED_BACK, OUTPUT);
      pinMode(PIN_HORN, OUTPUT);
      pinMode(PIN_EXTRA, OUTPUT);

      // Motors speed
      motorLeft.setSpeed(speed);
      motorRight.setSpeed(speed);
    }

    static void loop() {
      char bt;
      unsigned long m = millis();

      if (BTSerial.available()) {
        if (!btConnected) {
          btConnected = true;
          Serial.println('BT connected!');
        }
        
        bt = BTSerial.read();
        Serial.println('BT: ' + bt);
      } else {
        // BT disconnected
        if (btConnected) {
          btConnected = false;
          bt = 'S';
          Serial.println('BT disconnected!');
        }
      }

      if (isRecordingHistory) {
        if (lastMove != bt) {
          // new move
          historyMoves.push(lastMove);
          historyTimes.push(m - lastMoveTime);
          lastMove = bt;
          lastMoveTime = m;

          if (historyMoves.isFull()) {
            bt = 'x'; // Stop recording            
          }
        }
      } else if (!historyTimes.isEmpty()) {
        if (lastMoveTime - m > 0) {
          lastMoveTime = m + historyTimes.pop();
          lastMove = historyMoves.pop();
        }

        bt = lastMove;
      }

      switch (bt) {
        case 'F':
          // move forward
          motorLeft.setSpeed(speed);
          motorRight.setSpeed(speed);
          motorLeft.forward();
          motorRight.forward();
          break;
        
        case 'I':
          // move forward right
          motorLeft.setSpeed(speed);
          motorRight.stop();
          motorLeft.forward();
          motorRight.forward();
          break;
      
        case 'R':
          // turn right
          motorLeft.setSpeed(speed);
          motorRight.setSpeed(speed);
          motorLeft.forward();
          motorRight.backward();
          break;
      
        case 'J':
          // move back right
          motorLeft.setSpeed(speed);
          motorRight.stop();
          motorLeft.backward();
          motorRight.backward();
          break;
      
        case 'B':
          // move back
          motorLeft.setSpeed(speed);
          motorRight.setSpeed(speed);
          motorLeft.backward();
          motorRight.backward();
          break;
      
        case 'H':
          // move back left
          motorLeft.stop();
          motorRight.setSpeed(speed);
          motorLeft.backward();
          motorRight.backward();
          break;
      
        case 'L':
          // turn left
          motorLeft.setSpeed(speed);
          motorRight.setSpeed(speed);
          motorLeft.backward();
          motorRight.forward();
          break;
        
        case 'G':
          // move forward left
          motorLeft.stop();
          motorRight.setSpeed(speed);
          motorLeft.forward();
          motorRight.forward();
          break;

        case 'W':
          // front lights on
          digitalWrite(PIN_LED_FRONT, HIGH);
          break;
        case 'w':
          // front lights off
          digitalWrite(PIN_LED_FRONT, LOW);
          break;

        case 'U':
          // back lights on
          digitalWrite(PIN_LED_BACK, HIGH);
          break;
        case 'u':
          // back lights off
          digitalWrite(PIN_LED_BACK, LOW);
          break;

        case 'V':
          // horn on
          digitalWrite(PIN_HORN, HIGH);
          break;
        case 'v':
          // horn off
          digitalWrite(PIN_HORN, LOW);
          break;

        case 'X':
          // extra on
          // Start recording
          isRecordingHistory = true;
          lastMove = bt;
          lastMoveTime = m;
          digitalWrite(PIN_EXTRA, HIGH);
          break;
        case 'x':
          // extra off
          // Finish recording, run hisotory back
          isRecordingHistory = false;
          digitalWrite(PIN_EXTRA, LOW);
          break;
      
        case 'S':
          // STOP motors
          motorLeft.stop();
          motorRight.stop();
          break;
      
        case 'D':
          // STOP ALL
          motorLeft.stop();
          motorRight.stop();
          digitalWrite(PIN_LED_FRONT, LOW);
          digitalWrite(PIN_LED_BACK, LOW);
          digitalWrite(PIN_HORN, LOW);
          digitalWrite(PIN_EXTRA, LOW);
          break;

        case 'q':
          // Speed 100
          speed = 100;
          motorLeft.setSpeed(speed);
          motorRight.setSpeed(speed);
          break;        

        default:
          // Speed 0-90

          // WARNING: Next operation can fail, and the rest of the code will be not executed
          // Following should be the last operation of the "loop"
          unsigned int i = String(bt).toInt();

          if (i >= 0 || i <= 9) {
            speed = i * 10;

            motorLeft.setSpeed(speed);
            motorRight.setSpeed(speed);
          }
      }
    }
};

static SoftwareSerial Main::BTSerial(PIN_BT_RX, PIN_BT_TX);
Motor Main::motorLeft(PIN_MOTOR_L1, PIN_MOTOR_L2, PIN_MOTOR_LE);
Motor Main::motorRight(PIN_MOTOR_R1, PIN_MOTOR_R2, PIN_MOTOR_RE);
bool Main::btConnected = false;
unsigned int Main::speed = 100;

char Main::lastMove = 'x';
unsigned long Main::lastMoveTime = 0;
StackArray<char> Main::historyMoves;
StackArray<unsigned long> Main::historyTimes;
bool Main::isRecordingHistory = false;
