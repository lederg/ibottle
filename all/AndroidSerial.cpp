#include "mbed.h"
#include "smartbottle.hpp"
#include "AndroidSerial.hpp"

void android_serial_stub(void const *obj) {
	((AndroidSerial *)obj)->run();
} 


void AndroidSerial::run() {
	and_message_t andmsg;
    int header_count = 0;

    while(1) {
        while(1) {
            char c = getc();
            if(c == 0xcc) {
                header_count++;
            } else {
                header_count = 0;
            }
            if(header_count > 3) break;
        }

        read(&andmsg,4);
        read(serial_buf,andmsg.length);        
        
	    switch(andmsg.type) {
	        case AND_M_ENROLL:
	        getScanner()->add(getMsgType(MSG_SCANNER_ENROLL));
	        break;
	        case AND_M_CALIBRATE:
	        getController()->add(getMsgType(MSG_CONTROLLER_CALIBRATE));
	        break;
	        case AND_M_SET_WEIGHT:
	        int mg = *((int *)serial_buf)        ;
	        getLoadCell()->setPillWeight(mg);
	        getDebug().printf("Setting weight to %d mg",mg);
	        break;

	    }
    }  
    
}
