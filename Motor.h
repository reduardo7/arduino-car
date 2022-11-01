#ifndef _MODULE_MOTOR
#define _MODULE_MOTOR

#include "Config.h"
#include "App_Runnable.h"

class Motor: public Runnable {
  private:
    const uint8_t _pin1;
    const uint8_t _pin2;
    const uint8_t _pin3;
    const uint8_t _pin4;

    unsigned int i1 = 1;
    unsigned int i2 = 0;
    unsigned int i3 = 0;
    unsigned int i4 = 0;

    unsigned long _nextInterval = 0;
    unsigned int _interval = 0;

    /**
     * 0 -> stop
     * 1 -> forward
     * 2 -> backward
     */
    unsigned int _action = 0;

    void _update() {
      digitalWrite(this->_pin1, this->i1);
      digitalWrite(this->_pin2, this->i2);
      digitalWrite(this->_pin3, this->i3);
      digitalWrite(this->_pin4, this->i4);
    }

  protected:
    void onSetup() {
      pinMode(this->_pin1, OUTPUT);
      pinMode(this->_pin2, OUTPUT);
      pinMode(this->_pin3, OUTPUT);
      pinMode(this->_pin4, OUTPUT);

      this->stop();
    }

    void onLoop() {
      if (this->_action && this->_interval) {
        unsigned long m = micros();
        
        if (this->_nextInterval < m) {
          this->_nextInterval = m + this->_interval;

          if (this->_action == 1) {
            this->next();
          } else {
            this->prev();
          }
        }
      }
    }

  public:
    Motor(uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4) :
      _pin1(pin1), _pin2(pin2), _pin3(pin3), _pin4(pin4)
    {}

    void next() {
      if ( this->i1 && !this->i2 && !this->i4 ) {
        // 1
        this->i2 = 1;
      } else if ( this->i1 && this->i2 ) {
        // 2
        this->i1 = 0;
      } else if ( !this->i1 && this->i2 && !this->i3 ) {
        // 3
        this->i3 = 1;
      } else if ( this->i2 && this->i3 ) {
        // 4
        this->i2 = 0;
      } else if ( !this->i2 && this->i3 && !this->i4 ) {
        // 5
        this->i4 = 1;
      } else if ( this->i3 && this->i4 ) {
        // 6
        this->i3 = 0;
      } else if ( !this->i3 && this->i4 && !this->i1 ) {
        // 7
        this->i1 = 1;
      } else if ( this->i4 && this->i1 ) {
        // 8
        this->i4 = 0;
      }

      this->_update();
    }

    void prev() {
      if ( this->i1 && !this->i2 && !this->i4 ) {
        // 1
        this->i4 = 1;
      } else if ( this->i1 && this->i2 ) {
        // 2
        this->i2 = 0;
      } else if ( !this->i1 && this->i2 && !this->i3 ) {
        // 3
        this->i1 = 1;
      } else if ( this->i2 && this->i3 ) {
        // 4
        this->i3 = 0;
      } else if ( !this->i2 && this->i3 && !this->i4 ) {
        // 5
        this->i2 = 1;
      } else if ( this->i3 && this->i4 ) {
        // 6
        this->i4 = 0;
      } else if ( !this->i3 && this->i4 && !this->i1 ) {
        // 7
        this->i3 = 1;
      } else if ( this->i4 && this->i1 ) {
        // 8
        this->i1 = 0;
      }

      this->_update();
    }

    Motor* setInterval(int interval) {
      // this->_nextInterval = 0;
      this->_interval = interval;
      return this;
    }

    Motor* forward() {
      this->_action = 1;
      return this;
    }

    Motor* backward() {
      this->_action = 2;
      return this;
    }

    Motor* stop() {
      this->_action = 0;
      return this;
    }
};

#endif