#ifndef __SERVOLOCK_HPP
#define __SERVOLOCK_HPP

#include "mbed.h"
#include "Servo.h"

#define UNLOCKED 0
#define LOCKED 1

class ServoLock {
    Servo mServo;
    int mState;

public:
    ServoLock(PinName pwm);
    void init(void);
    int getState() {return mState;}
    void setState(int);
    void lock() {setState(LOCKED);}
    void unlock() {setState(UNLOCKED);}
    bool isLocked() {return (bool)getState();}
};

#endif  //__SERVOLOCK_HPP
