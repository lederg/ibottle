#ifndef __ADXL_MODULE_HPP
#define __ADXL_MODULE_HPP
#include "mbed.h"
#include "ADXL345.h"
class AdxlModule {
	ADXL345 mAdxl;
public:
    AdxlModule(PinName pin1, PinName pin2, PinName pin3, PinName pin4) : mAdxl(pin1, pin2, pin3, pin4) {}
    bool isTilted();
};


#endif  //__ADXL_MODULE_HPP
