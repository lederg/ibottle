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
DigitalOut dbg_led(LED1);
DigitalOut dbg_led2(LED2);


AndroidSerial ble(p13,p14);    // This is used to connect to the android
ScannerFsm scanner(p28,p27);
// AndroidSerial ble(p28,p27);    // This is used to connect to the android
// ScannerFsm scanner(p13,p14);
ControllerFsm controller(p16);   // p16 here is the RESET interrupt
AndroidFsm android;
ServoLock lock(p21);
LoadCell loadcell(p19);
AdxlModule adxl(p5,p6,p7,p8, p9, p10);


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

AndroidSerial &getAndroidSerial() {
    return ble;
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
        if(c >= '0' && c <= '9') {
            int g = c - '0';
            debug.printf("Enrolling id %d\r\n", g);
            getScanner()->add(getMsgType(MSG_SCANNER_ENROLL,g));
        }
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
            case 'F':
                debug.printf("Sending MSG_SCN_AND_RF\n");
                getAndroid()->add(getMsgType(MSG_SCN_AND_RF));
                break;
            case 'S':
                debug.printf("Sending MSG_CTRL_AND_NEW_SAMPLE\n");
                getAndroid()->add(getMsgType(MSG_CTRL_AND_NEW_SAMPLE));
                break;
            case 'C':
                debug.printf("Sending MSG_CONTROLLER_CALIBRATE\n");
                getController()->add(getMsgType(MSG_CONTROLLER_CALIBRATE));
                break;
            case 'L':
                debug.printf("Lock\r\n");
                getLock().lock();
                break;
            case 'U':
                debug.printf("Unlock\r\n");
                getLock().unlock();
                break;
            case 'H':
                debug.printf("Faking 5 history vals\n");
                getLoadCell()->fakeHistory(5);
                break;
            case 'P':
                debug.printf("Print history\r\n");
                getLoadCell()->printHistory();
                break;
            case 'T':
                getController()->add(getMsgType(MSG_CTRL_PRINT_STATE));
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
    // controller_thread.set_priority(osPriorityAboveNormal);
    dbg_led2 = 1;
    dbg_led = 0;    
    Thread scanner_thread(thread_stub,getScanner());
    Thread and_listener_thread(thread_stub,getAndroid());
    Thread android_serial_thread(android_serial_stub,&ble);
    Thread thread(test_thread);
    debug.printf("Not setting time...\r\n");
    debug.printf("Sending first tick to controller\r\n");
    getController()->add(getMsgType(MSG_CONTROLLER_TICK));
    // set_time(1256729737); 
    time_t seconds = time(NULL);
    debug.printf("Number of seconds since 1970: %d\r\n",seconds);
    while(1) {
        Thread::wait(1000);
    }
}
