#ifndef __SCANNERFSM_HPP
#define __SCANNERFSM_HPP
#include "mbed.h"
#include "GT511C3.hpp"
#include "FsmThread.hpp"


class ScannerFsm : public FsmThread {
	GT511C3 mScanner;
	bool bOpen;
public:
	ScannerFsm(PinName _tx, PinName _rx) : mScanner(_tx, _rx) , bOpen(false), FsmThread() {}
	void init();
	void do_scan();
	void open();
	void close();
	void do_enroll(int id);
	int fsm_handler(message_t *);
};


#endif  //__SCANNERFSM_HPP

