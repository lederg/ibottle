#ifndef __CONTROLLERFSM_HPP
#define __CONTROLLERFSM_HPP
#include "mbed.h"
#include "rtos.h"
#include "FsmThread.hpp"

#define DEFAULT_SCANNER_ID 0

class ControllerFsm : public FsmThread {
	RtosTimer *do_scan_timer;
	RtosTimer *timeup_timer;
	int mScanCounter;

	void identified(message_t *msg);

public:
	ControllerFsm();
	void init();
	void start_scan();
	void do_scan();
	void deactivate(message_t *);
	int fsm_handler(message_t *);
};


#endif  //__CONTROLLERFSM_HPP
