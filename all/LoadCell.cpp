#include "mbed.h"
#include "LoadCell.hpp"

#define LC_bufsize 10000
#define LC_iterations 5 
#define LC_delaytime 0.2
#define ADC_BIAS 20000

uint16_t LoadCell::sample() {
    return myADC.read_u16() - 20000;
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
    return avg/LC_iterations; 
}

void LoadCell::calibrate() {
    mLastBias = averageSample();
}

uint16_t LoadCell::numberOfPills() {
    long int res = averageSample(); 
    float mg = (float)(res-mLastBias)/(float)mADCperMg;
    return (uint16_t)((mg/(float)mPillWeight) + 0.5);
}




