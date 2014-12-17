#include "mbed.h"
#include "rtos.h"
#include "smartbottle.hpp"
#include "LoadCell.hpp"
#include "ServoLock.hpp"
#include "FsmThread.hpp"
#include "ControllerFsm.hpp"
#include "ScannerFsm.hpp"
#include "AndroidFsm.hpp"
#include "AndroidMessage.hpp"
#include "AndroidSerial.hpp"
#include "AdxlModule.hpp"

#define SIG_SB_WAKEUP 0x01

Serial debug(USBTX,USBRX);
//DigitalOut myled(LED1);
DigitalOut dbg_led(LED1);
DigitalOut dbg_led2(LED2);
//GT511C3 finger(p28,p27);
// GT511C3 finger(PTC4,PTC3);


AndroidSerial ble(p28,p27);    // This is used to connect to the android
ScannerFsm scanner(p13,p14);
ControllerFsm controller;
AndroidFsm android;
ServoLock lock(p22);
LoadCell loadcell(p15);
AdxlModule adxl(p5,p6,p7,p8);


MemoryPool<message_t, MSG_MEMPOOL_SIZE> mpool;

ServoLock &getLock() {
    return lock;
}

FsmThread *getController() {
    return (FsmThread *)&controller;
}
FsmThread *getScanner() {
    return (FsmThread *)&scanner;
}
FsmThread *getAndroid() {
    return (FsmThread *)&android;
}

AdxlModule &getAdxl() {return adxl;}

LoadCell *getLoadCell() {return &loadcell;}


Serial &getDebug() {
    return debug;
}

MemoryPool<message_t, MSG_MEMPOOL_SIZE> *getMemPool() {
    return &mpool;
}

message_t *getMsgType(int type, int data) {
    message_t *msg = getMemPool()->alloc();
    msg->type = type;
    msg->data1 = data;
    return msg;
}



void test_thread(void const *args) {
    debug.printf("Click anything to send commands\n");
    while(true) {
        char c = debug.getc();
        debug.printf("Got %c!\n",c);
        switch(c) {
            case 'A':
                debug.printf("Sending MSG_CONTROLLER_ACTIVE to controller\n");
                getController()->add(getMsgType(MSG_CONTROLLER_ACTIVE));             
                break;
            case 'B':
                debug.printf("Sending MSG_CONTROLLER_INACTIVE\n");
                getController()->add(getMsgType(MSG_CONTROLLER_INACTIVE));
                break;
            case 'R':
                debug.printf("Sending MSG_CONTROLLER_RESET\n");
                getController()->add(getMsgType(MSG_CONTROLLER_RESET));
                break;
            case 'E':
                debug.printf("Sending MSG_CONTROLLER_ENROLL\n");
                getScanner()->add(getMsgType(MSG_SCANNER_ENROLL,DEFAULT_SCANNER_ID));
                break;

        }
        Thread::wait(0.1);
    }
}


int main() {
    int sts = 0;
    int ID = 0;
    dbg_led = 0;
    debug.format(8,Serial::None,1);
    debug.baud(115200);
    dbg_led = 1;
    Thread controller_thread(thread_stub,getController());
    controller_thread.set_priority(osPriorityAboveNormal);
    dbg_led2 = 1;
    dbg_led = 0;    
    Thread scanner_thread(thread_stub,getScanner());
    Thread and_listener_thread(thread_stub,getAndroid());
    Thread android_serial_thread(android_serial_stub,&ble);
    Thread thread(test_thread);
    debug.printf("Here 0");
    // RtosTimer *led_1_timer;
    // led_1_timer = new RtosTimer(blink, osTimerPeriodic, (void *)0);
    // led_1_timer->start(5000);
    // sts = finger.Open();
    while(1) {
        Thread::wait(1);
    }
    // debug.printf("sts = %d\n",sts);
    // if(sts == 0){
    //     int i;
    //     debug.printf("FirmwareVersion = %lx\n",finger.FirmwareVersion);
    //     debug.printf("IsoAreaMaxSize = %ld\n",finger.IsoAreaMaxSize);
    //     debug.printf("DeviceSerialNumber = ");
    //     for(i = 0; i < sizeof(finger.DeviceSerialNumber);i++){
    //         debug.printf("%02X",finger.DeviceSerialNumber[i]);
    //     }
    //     debug.printf("\n");
    // }
//
//    if(0){
//        int EnrollID = 12;
//        if(finger.CheckEnrolled(EnrollID) == 0){
//            debug.printf("EnrollID(%d) is already enrolled.Delete!\n",EnrollID);
//            if(finger.DeleteID(EnrollID) == 0){
//                debug.printf("Delete OK!\n");
//            }
//        }
//        finger.Enroll(EnrollID,progress);
//    }
//
    // finger.CmosLed(1);
    //while(1) {
//        debug.printf("Press finger for Identify\n");
//        finger.WaitPress(1);
//        if(finger.Capture(1) != 0)
//            continue;
//        ID = finger.Identify();
//        debug.printf("ID = %d\n",ID); 
//        debug.printf("Remove finger\n");
//        finger.WaitPress(0);
//    }
}
