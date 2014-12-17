#ifndef __FSMTHREAD_HPP
#define __FSMTHREAD_HPP
#include "mbed.h"
#include "rtos.h"
#include "FsmMessage.hpp"

#define STATE_ZERO 0

class FsmThread {
	Queue<message_t, 16> mQueue;
protected:
	int mState;
public:
	void add(message_t *);
	FsmThread() {}
	FsmThread(int s) : mState(s) {}
	void init();
	virtual int fsm_handler(message_t *) {return 0;}
	void run();	
};

void thread_stub(void const *thrd);

#endif  //__FSMTHREAD_HPP
