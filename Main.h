#ifndef _MODULE_MAIN
#define _MODULE_MAIN

#include "Motor.h"

class Main {
  private:
    static Motor motor1;
    static Motor motor2;

  public:
    static void setup() {
      motor1.setInterval(1000);
      motor2.setInterval(1000);
    }

    static void loop() {
    }
};

Motor Main::motor1(PIN_MOTOR_11, PIN_MOTOR_12, PIN_MOTOR_13, PIN_MOTOR_14);
Motor Main::motor2(PIN_MOTOR_21, PIN_MOTOR_22, PIN_MOTOR_23, PIN_MOTOR_24);

#endif