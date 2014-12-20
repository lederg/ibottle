#ifndef __ADXL_MODULE_HPP
#define __ADXL_MODULE_HPP
#include "mbed.h"
#include "ADXL345.h"
class AdxlModule {
	InterruptIn mActive;
	InterruptIn mInactive;
	ADXL345 mAdxl;
	bool mIsActive;

	void init();
public:
	int mCount;
    AdxlModule(PinName pin1, PinName pin2, PinName pin3, PinName pin4, PinName _active, PinName _inactive);
    void active_int();
    void inactive_int();    
    bool isActive() {return mIsActive;}
};


#endif  //__ADXL_MODULE_HPP
