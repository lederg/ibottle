#include "mbed.h"
#include "smartbottle.hpp"
#include "ServoLock.hpp"

#define LOCK_VAL 0.5
#define UNLOCK_VAL 0

void ServoLock::init(void) {

}

ServoLock::ServoLock(PinName pwm) : mServo(pwm), mState(LOCKED) {	
	mServo.calibrate(0.0009,90);
}

void ServoLock::setState(int s) {
	getDebug().printf("Servo state: %d => %d\n",mState,s);
	if(mState == s) return;
	if(s==LOCKED) {
		mServo = LOCK_VAL;		
	} else if(s==UNLOCKED) {
		mServo = UNLOCK_VAL;
	}
	mState = s;
}
