#include "smartbottle.hpp"
#include "mbed.h"
#include "ControllerFsm.hpp"
#include "AdxlModule.hpp"
#include "LoadCell.hpp"

#define STATE_WAIT_IDENTIFICATION 0
#define STATE_OPEN 1
#define STATE_CLOSED 2
#define STATE_CONTROLLER_IDLE 3


void send_scan(void const *obj) {
	((ControllerFsm*)obj)->do_scan();
}

void timeup(void const *obj) {	
	((ControllerFsm*)obj)->add(getMsgType(MSG_TIMEUP));
}

ControllerFsm::ControllerFsm() : FsmThread() {
	init();
}
void ControllerFsm::init() {
	mState = STATE_CONTROLLER_IDLE;
	timeup_timer = do_scan_timer = NULL;
	// do_scan_timer = new RtosTimer(blink, osTimerPeriodic, (void *)this);
	// timeup_timer = new RtosTimer(timeup, osTimerOnce, (void *)this);	
}

void ControllerFsm::deactivate(message_t *) {
	if(!getLock().isLocked()) {
		getLock().lock();
	}
	getScanner()->add(getMsgType(MSG_SCANNER_CLOSE));
	do_scan_timer->stop();
	timeup_timer->stop();


	// Here we want to take a measurement from the load cell if the tilt is ok.


	if(!getAdxl().isTilted()) {
		uint16_t sample = getLoadCell()->numberOfPills();
	}	
}

void ControllerFsm::identified(message_t *msg) {
	if(msg->data1 != DEFAULT_SCANNER_ID) {   // this is a MISIDENTIFICATION. Stop the scanning, zero the counter, start again.
		do_scan_timer->stop();
		start_scan();
	} else {   // This is IDENTIFICATION. Close the scanner, Open the servo and move to STATE_OPEN
		do_scan_timer->stop();
		getScanner()->add(getMsgType(MSG_SCANNER_CLOSE));
		getLock().unlock();
		mState = STATE_OPEN;
		timeup_timer->start(20000);
	}
}

void ControllerFsm::start_scan() {
	mScanCounter = 0;
	getDebug().printf("Starting scanner\n");
	getScanner()->add(getMsgType(MSG_SCANNER_OPEN));
	do_scan_timer->start(200);
}

void ControllerFsm::do_scan() {
	printf("Inside do_scan, mScanCounter is %d\n", mScanCounter);
	if(mScanCounter++ < 100) {    //20s		
		getScanner()->add(getMsgType(MSG_SCANNER_SCAN));
	} else {
		printf("Done with do_scan, mScanCounter is %d\n", mScanCounter);
		do_scan_timer->stop();
		getScanner()->add(getMsgType(MSG_SCANNER_CLOSE));
		mScanCounter = 0;

	}	
}

int ControllerFsm::fsm_handler(message_t *msg) {
	getDebug().printf("Got message of type %d\n",msg->type);
	if(!do_scan_timer) {
		do_scan_timer = new RtosTimer(send_scan, osTimerPeriodic, (void *)this);
		timeup_timer = new RtosTimer(timeup, osTimerOnce, (void *)this);	
	}
	switch(msg->type) {
		case MSG_TIMEUP:
			switch(mState) {
				case STATE_OPEN:
				timeup_timer->stop();
				getLock().lock();
				mState = STATE_CLOSED;
				break;

			}
			break;

		case MSG_SCANNER_IDENTIFIED:
			switch(mState) {
				case STATE_WAIT_IDENTIFICATION:
				identified(msg);
				break;

			}
			break;

		case MSG_CONTROLLER_ACTIVE:
			switch(mState) {
				case STATE_CONTROLLER_IDLE:
				start_scan();
				mState = STATE_WAIT_IDENTIFICATION;
				break;
			}
			break;
		case MSG_CONTROLLER_INACTIVE:    // Go to inactive state
			deactivate(msg);
			break;
	}

	return 0;
}


