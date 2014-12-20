#include "mbed.h"
#include "LoadCell.hpp"
#include "smartbottle.hpp"

#define LC_bufsize 10000
#define LC_iterations 5 
#define LC_delaytime 0.2
#define ADC_BIAS 20000

uint16_t LoadCell::sample() {
    return myADC.read_u16() - 20000;
}


void LoadCell::fakeHistory(int n) {
    int i=0;
    for(i=0;i<n;i++) {
        mHistory[i].val = i;
        mHistory[i].timestamp = time(NULL);
    }

    mLastHistoryIndex = n;
}

void LoadCell::printHistory() {
    int i;
    for(i=0;i<mLastHistoryIndex;i++) {        
        getDebug().printf("time: %d, number of pills: %d\r\n",mHistory[i].timestamp,mHistory[i].val);
    }
}

long int LoadCell::averageSample(){
    long int sum = 0; 
    long int avg= 0; 
    for (int j = 0; j < LC_iterations; j++){
        sum = 0;   
        for(int i = 0; i < LC_bufsize; i++)
            sum += sample(); 
        avg += sum/LC_bufsize; 
        wait(LC_delaytime); 
    } 

    getDebug().printf("averageSample: Measured %lu\r\n",avg/LC_iterations);
    return avg/LC_iterations; 
}

void LoadCell::calibrate() {
    mLastBias = averageSample();
    // mLastBias = averageSample() + 15;
    getDebug().printf("Calibrated! Bias is %lu\r\n",mLastBias);
}

uint16_t LoadCell::numberOfPills() {
    long int res = averageSample(); 
    float mg = (float)(res-mLastBias)/(float)mADCperMg;
    getDebug().printf("numberOfPills: res is %lu, mg are: %f, weight of one pill: %d\r\n",res,mg,mPillWeight);    
    uint16_t rc = (uint16_t)((mg/(float)mPillWeight) + 0.5);
    mHistory[mLastHistoryIndex].val = (int)rc;
    mHistory[mLastHistoryIndex].timestamp = time(NULL);
    mLastHistoryIndex++;
    return rc;
}




