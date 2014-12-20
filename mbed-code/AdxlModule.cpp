#include "mbed.h"
#include "smartbottle.hpp"
#include "AdxlModule.hpp"


void AdxlModule::active_int() {
	mAdxl.getInterruptSource();
	mIsActive = true;
	mCount++;
	// getController()->add(getMsgType(MSG_CONTROLLER_ACTIVE));
}

void AdxlModule::inactive_int() {
	mAdxl.getInterruptSource();
	mIsActive = false;
	mCount++;
	// getController()->add(getMsgType(MSG_CONTROLLER_INACTIVE));
}


AdxlModule::AdxlModule(PinName pin1, PinName pin2, PinName pin3, PinName pin4, PinName _active, PinName _inactive) : mAdxl(pin1, pin2, pin3, pin4) , 
mActive(_active), mInactive(_inactive), mIsActive(false), mCount(0) {
	 //Go into standby mode to configure the device.
    mAdxl.setPowerControl(0x00);

    //Full resolution, +/-16g, 4mg/LSB.
    mAdxl.setDataFormatControl(0x0B);

    //3.2kHz data rate.
    mAdxl.setDataRate(ADXL345_3200HZ);

    //Interrupt commands.
    mAdxl.setInterruptEnableControl(0x00); //disable all interrupts
    mAdxl.setInterruptMappingControl(0x08); //sets activity to int1 and inactivity to int2
    mAdxl.setActivityThreshold(0x06); //sets activity threshold
    mAdxl.setInactivityThreshold(0x5); //sets inactivity threshold
    mAdxl.setTimeInactivity(0x04); //sets time for inactivity
    mAdxl.setActivityInactivityControl(0xFF); //sets activity inactivity threshold AC and to react to xyz
    // mAdxl.setActivityInactivityControl(0x96); //sets activity inactivity threshold AC and to react to xyz
    //mAdxl.setInterruptEnableControl(0x80); //enable DATA_READY only
    mAdxl.setInterruptEnableControl(0x18); //enable ACTIVITY and INACTIVITY

    //Measurement mode.
    mAdxl.setPowerControl(0x38); //enable link and auto-sleep function 0x30

    mActive.rise(this,&AdxlModule::active_int);
    mInactive.rise(this,&AdxlModule::inactive_int);

}
