#ifndef _MODULE_MAIN
#define _MODULE_MAIN

#include <SoftwareSerial.h>
#include "Motor.h"

class Main {
  private:
    static SoftwareSerial BTSerial;
    static Motor motorLeft;
    static Motor motorRight;
    static bool btConnected;
    static unsigned int speed;
    
  public:
    static void setup() {
      BTSerial.begin(9600);

      pinMode(PIN_LED_FRONT, OUTPUT);
      pinMode(PIN_LED_BACK, OUTPUT);
      pinMode(PIN_HORN, OUTPUT);
      pinMode(PIN_EXTRA, OUTPUT);

      // Motors speed
      motorLeft.setInterval(speed);
      motorRight.setInterval(speed);
    }

    static void loop() {
      char bt;

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

      switch (bt) {
        case 'F':
          // move forward
          motorLeft.setInterval(speed);
          motorRight.setInterval(speed);
          motorLeft.forward();
          motorRight.forward();
          break;
        
        case 'I':
          // move forward right
          motorLeft.setInterval(speed);
          motorRight.setInterval(speed + SPEED_TURN);
          motorLeft.forward();
          motorRight.forward();
          break;
      
        case 'R':
          // turn right
          motorLeft.setInterval(speed);
          motorRight.setInterval(speed);
          motorLeft.forward();
          motorRight.backward();
          break;
      
        case 'J':
          // move back right
          motorLeft.setInterval(speed);
          motorRight.setInterval(speed + SPEED_TURN);
          motorLeft.backward();
          motorRight.backward();
          break;
      
        case 'B':
          // move back
          motorLeft.setInterval(speed);
          motorRight.setInterval(speed);
          motorLeft.backward();
          motorRight.backward();
          break;
      
        case 'H':
          // move back left
          motorLeft.setInterval(speed + SPEED_TURN);
          motorRight.setInterval(speed);
          motorLeft.backward();
          motorRight.backward();
          break;
      
        case 'L':
          // turn left
          motorLeft.setInterval(speed);
          motorRight.setInterval(speed);
          motorLeft.backward();
          motorRight.forward();
          break;
        
        case 'G':
          // move forward left
          motorLeft.setInterval(speed + SPEED_TURN);
          motorRight.setInterval(speed);
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
          digitalWrite(PIN_EXTRA, HIGH);
          break;
        case 'x':
          // extra off
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
          speed = SPEED_MIN - 100;
          motorLeft.setInterval(speed);
          motorRight.setInterval(speed);
          break;        

        default:
          // Speed 0-90

          // WARNING: Next operation can fail, and the rest of the code will be not executed
          // Following should be the last operation of the "loop"
          unsigned int i = String(bt).toInt();

          if (i >= 0 || i <= 9) {
            speed = SPEED_MIN - (i * 10);

            motorLeft.setInterval(speed);
            motorRight.setInterval(speed);
          }
      }
    }
};

static SoftwareSerial Main::BTSerial(PIN_BT_RX, PIN_BT_TX);
Motor Main::motorLeft(PIN_MOTOR_11, PIN_MOTOR_12, PIN_MOTOR_13, PIN_MOTOR_14);
Motor Main::motorRight(PIN_MOTOR_21, PIN_MOTOR_22, PIN_MOTOR_23, PIN_MOTOR_24);
bool Main::btConnected = false;
unsigned int Main::speed = SPEED_MIN;

#endif