#include "mbed.h"
#include "smartbottle.hpp"
#include "ServoLock.hpp"

#define LOCK_VAL 1.0
#define UNLOCK_VAL 0.1

void ServoLock::init(void) {

}

ServoLock::ServoLock(PinName pwm) : mServo(pwm), mState(LOCKED) {	
	mServo.calibrate(0.0010, 45.0);
	lock();
}

void ServoLock::setState(int s) {
	getDebug().printf("Servo state: %d => %d\n",mState,s);
	if(s==LOCKED) {
		mServo = LOCK_VAL;		
	} else if(s==UNLOCKED) {
		mServo = UNLOCK_VAL;
	}
	mState = s;
}
