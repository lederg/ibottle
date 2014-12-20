#ifndef __LOADCELL_HPP
#define __LOADCELL_HPP
#include "mbed.h"


typedef struct {
    time_t timestamp;
    int val;
} lc_sample;



class LoadCell {
    int mPillWeight;
    float mADCperMg;  
    AnalogIn myADC;
    long int mLastBias;
public:
    lc_sample mHistory[256];
    int mLastHistoryIndex;
    LoadCell(PinName aIn) : myADC(aIn), mADCperMg(46.8/770.0), mLastHistoryIndex(0), mPillWeight(770) {}
    uint16_t sample();
    uint16_t numberOfPills();
    long int averageSample();
    void calibrate();   
    void setPillWeight(int mg) {mPillWeight = mg;}
    void fakeHistory(int);
    void printHistory();
    // int lastSample() {return mHistory[mLastHistoryIndex-1].val;}
    
};

#endif  //__LOADCELL_HPP
