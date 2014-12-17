#ifndef __ANDROID_LISTENER_FSM_HPP
#define __ANDROID_LISTENER_FSM_HPP
#include "mbed.h"
#include "GT511C3.hpp"
#include "FsmThread.hpp"


class AndroidFsm : public FsmThread {
public:
	AndroidFsm() : FsmThread() {}
	AndroidFsm(PinName _tx, PinName _rx) : FsmThread() {}
	void init();
	int fsm_handler(message_t *);
};


#endif  //__ANDROID_LISTENER_FSM_HPP

