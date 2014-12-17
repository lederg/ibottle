#ifndef __LOADCELL_HPP
#define __LOADCELL_HPP
#include "mbed.h"

class LoadCell {
	int mPillWeight;
    float mADCperMg;  
    AnalogIn myADC;
    int mLastBias;
public:
    LoadCell(PinName aIn) : myADC(aIn), mADCperMg(45.0/770.0) {}
    uint16_t sample();
    uint16_t numberOfPills();
    long int averageSample();
    void calibrate();   
    void setPillWeight(int mg) {mPillWeight = mg;}
    
};

#endif  //__LOADCELL_HPP
