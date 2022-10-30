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

  public:
    static void setup() {
      BTSerial.begin(9600);

      pinMode(PIN_LED, OUTPUT);

      // Motors speed
      motorLeft.setInterval(1000);
      motorRight.setInterval(1000);
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
          // move forward(all motors rotate in forward direction)
          motorLeft.forward();
          motorRight.forward();
          break;
      
        case 'B':
          // move reverse (all motors rotate in reverse direction)
          motorLeft.backward();
          motorRight.backward();
          break;
      
        case 'L':
          // turn right (left side motors rotate in forward direction, right side motors doesn't rotate)
          motorLeft.backward();
          motorRight.forward();
          break;
      
        case 'R':
          // turn left (right side motors rotate in forward direction, left side motors doesn't rotate)
          motorLeft.forward();
          motorRight.backward();
          break;

        // case 'W':
        //   // turn led on
        //   digitalWrite(9,HIGH);
        //   break
        // case 'w':
        //   // turn led off
        //   digitalWrite(9,LOW);
        //   break;
      
        case 'S':
          // STOP (all motors stop)
          motorLeft.stop();
          motorRight.stop();
          break;
      }
    }
};

static SoftwareSerial Main::BTSerial(0, 1); // RX | TX
Motor Main::motorLeft(PIN_MOTOR_11, PIN_MOTOR_12, PIN_MOTOR_13, PIN_MOTOR_14);
Motor Main::motorRight(PIN_MOTOR_21, PIN_MOTOR_22, PIN_MOTOR_23, PIN_MOTOR_24);
bool Main::btConnected = false;

#endif