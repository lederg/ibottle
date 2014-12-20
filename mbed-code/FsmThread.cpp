#include "mbed.h"
#include "rtos.h"
#include "FsmThread.hpp"
#include "smartbottle.hpp"

DigitalOut dbg_led3(LED3);

void FsmThread::add(message_t *msg) {
	mQueue.put(msg);
}
void FsmThread::init() {
}

void FsmThread::run(void) {
	
	while(true) {
		osEvent evt = mQueue.get();
        if (evt.status == osEventMessage) {
        	message_t *msg = (message_t*)evt.value.p;
        	this->fsm_handler(msg);
        	getMemPool()->free(msg);
        }
	}
}

void thread_stub(void const *thrd) {
	dbg_led3 = 1;
	getDebug().printf("Beginning runloop\n");
	((FsmThread *)thrd)->run();
}
