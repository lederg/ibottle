#include "smartbottle.hpp"
#include "mbed.h"
#include "ControllerFsm.hpp"
#include "AdxlModule.hpp"
#include "LoadCell.hpp"

#define STATE_WAIT_IDENTIFICATION 0
#define STATE_OPEN_ACTIVE 1
#define STATE_CLOSED 2
#define STATE_CONTROLLER_IDLE 3
#define STATE_OPEN_INACTIVE 4


void send_scan(void const *obj) {
	((ControllerFsm*)obj)->do_scan();
}

void timeup(void const *obj) {	
	((ControllerFsm*)obj)->add(getMsgType(MSG_TIMEUP));
}

void tick(void const *obj) {	
	((ControllerFsm*)obj)->add(getMsgType(MSG_CONTROLLER_TICK));
}

ControllerFsm::ControllerFsm(PinName _reset) : mReset(_reset), FsmThread() {
	init();
}

void ControllerFsm::reset_int() {
    getController()->add(getMsgType(MSG_CONTROLLER_RESET));
}

void ControllerFsm::init() {
	mState = STATE_CONTROLLER_IDLE;
	timeup_timer = do_scan_timer = NULL;
    // mReset.rise(this,&ControllerFsm::reset_int);
}

void ControllerFsm::move_to_open_inactive() {
	timeup_timer->start(20000);
	mState = STATE_OPEN_INACTIVE;	
}

void ControllerFsm::move_to_open_active() {
	timeup_timer->stop();
	mState = STATE_OPEN_ACTIVE;	
}


void ControllerFsm::deactivate(message_t *) {
	if(!getLock().isLocked()) {
		getLock().lock();
	}
	getDebug().printf("deactivate: Stopping timers\r\n");
	do_scan_timer->stop();
	timeup_timer->stop();
	getDebug().printf("deactivate: sending CLOSE to scanner\r\n");
	getScanner()->add(getMsgType(MSG_SCANNER_CLOSE));


	// Here we want to take a measurement from the load cell if the tilt is ok.
	Thread::wait(500);
	getDebug().printf("deactivate: After wait. Measuring.\r\n");	
	uint16_t sample = getLoadCell()->numberOfPills();
	getDebug().printf("New measurement taken, number of pills are %d\r\n",sample);	
	getAndroid()->add(getMsgType(MSG_CTRL_AND_NEW_SAMPLE,(int)sample));
	mState = STATE_CONTROLLER_IDLE;

}

void ControllerFsm::identified(message_t *msg) {
	if(msg->data1 == -1) {   // this is a MISIDENTIFICATION. Stop the scanning, zero the counter, start again.
		do_scan_timer->stop();
		start_scan();
	} else {   // This is IDENTIFICATION. Close the scanner, Open the servo and move to STATE_OPEN
		do_scan_timer->stop();
		getScanner()->add(getMsgType(MSG_SCANNER_CLOSE));
		getLock().unlock();
		mState = STATE_OPEN_ACTIVE;
	}
}

void ControllerFsm::start_scan() {
	mScanCounter = 0;
	getDebug().printf("Starting scanner\n");
	getScanner()->add(getMsgType(MSG_SCANNER_OPEN));
	do_scan_timer->stop();
	do_scan_timer->start(350);
}

void ControllerFsm::do_scan() {
	printf("Inside do_scan, mScanCounter is %d\n", mScanCounter);
	if(mScanCounter++ < 60) {    //20s		
		getScanner()->add(getMsgType(MSG_SCANNER_SCAN));
	} else {
		printf("Done with do_scan, mScanCounter is %d\n", mScanCounter);
		do_scan_timer->stop();
		getScanner()->add(getMsgType(MSG_SCANNER_CLOSE));
		mScanCounter = 0;
		mState = STATE_CLOSED;

	}	
}


int ControllerFsm::fsm_handler(message_t *msg) {
	getDebug().printf("Got message of type %d\n",msg->type);
	if(!do_scan_timer) {
		do_scan_timer = new RtosTimer(send_scan, osTimerPeriodic, (void *)this);
		timeup_timer = new RtosTimer(timeup, osTimerOnce, (void *)this);	
		tick_timer = new RtosTimer(tick, osTimerPeriodic, (void *)this);
		tick_timer->start(600);
	}
	switch(msg->type) {
		case MSG_TIMEUP:
			switch(mState) {
				case STATE_OPEN_INACTIVE:
				timeup_timer->stop();
				getLock().lock();
				mState = STATE_CLOSED;
				break;

			}
			break;

		case MSG_SCANNER_IDENTIFIED:
			if(mState == STATE_WAIT_IDENTIFICATION || msg->data1 == 99) {				
				identified(msg);
			} else {
				getDebug().printf("Got MSG_SCANNER_IDENTIFIED in state %d and id %d, what the fuck?\r\n",mState, msg->data1);				
			}
			break;

		case MSG_CONTROLLER_RESET:   // Whatever the state is, we start scanning and wait identification.
			start_scan();
			mState = STATE_WAIT_IDENTIFICATION;
			break;

		case MSG_CONTROLLER_ACTIVE:
			getDebug().printf("Got MSG_CONTROLLER_ACTIVE, what the fuck?\r\n");
			switch(mState) {
				case STATE_CONTROLLER_IDLE:
				start_scan();
				mState = STATE_WAIT_IDENTIFICATION;
				break;
			}
			break;
		case MSG_CONTROLLER_INACTIVE:    // Go to inactive state
			getDebug().printf("Got MSG_CONTROLLER_ACTIVE, what the fuck?\r\n");
			deactivate(msg);
			break;
		case MSG_CONTROLLER_CALIBRATE:
			if(mState == STATE_CONTROLLER_IDLE) {
				getLoadCell()->calibrate();
			} else {
				getDebug().printf("ControllerFsm: Error, MSG_CONTROLLER_CALIBRATE received in state %d\r\n",mState);
			}
			break;
		case MSG_CTRL_PRINT_STATE:
			getDebug().printf("Controller state is %d\r\n",mState);
			break;
		case MSG_CONTROLLER_TICK:
			getDebug().printf("Tick! isActive: %d, count is %d\r\n",getAdxl().isActive(), getAdxl().mCount);
			switch(mState) {
				case STATE_CONTROLLER_IDLE:
					if(getAdxl().isActive()) {
						start_scan();
						mState = STATE_WAIT_IDENTIFICATION;
					}
					break;
				case STATE_CLOSED:
					if(!getAdxl().isActive()) {
						deactivate(msg);
					}
					break;
				case STATE_OPEN_ACTIVE:
					if(!getAdxl().isActive()) {
						move_to_open_inactive();
					}
					break;
				case STATE_OPEN_INACTIVE:
					if(getAdxl().isActive()) {
						move_to_open_active();
					}
					break;


			}
			break;
	}

	return 0;
}


