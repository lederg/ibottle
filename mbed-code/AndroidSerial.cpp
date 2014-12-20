#include "mbed.h"
#include "smartbottle.hpp"
#include "AndroidSerial.hpp"

void android_serial_stub(void const *obj) {
	((AndroidSerial *)obj)->run();
} 


int AndroidSerial::send(and_message_t *msg, void *buff) {
	write(msg,sizeof(*msg));
	char *bla = (char *)msg;
	for(int i=0;i<sizeof(*msg);i++) {
		getDebug().printf("%d ",bla[i]);
	}
	if(buff) {
		bla = (char *)buff;
		for(int i=0;i<msg->length;i++) {
			getDebug().printf("%d ",bla[i]);
		}

		write(buff,msg->length);
	}
	return 0;
}


void AndroidSerial::run() {
	struct {
		int type;
		int length;
	} andmsg;
    int header_count = 0;
    int mg;

    getDebug().printf("AndroidSerial: Enter\r\n");
    while(1) {
        while(1) {
            char c = getc();
		    getDebug().printf("AndroidSerial: header_count: %d, Got %d\r\n",header_count,c);
            if(c == 0xcc) {
                header_count++;
            } else {
                header_count = 0;
            }
            if(header_count > 3) break;
        }
        header_count = 0;        
        read(&andmsg,sizeof(andmsg));
        getDebug().printf("Got android message, type %d, length %d\r\n",andmsg.type,andmsg.length);
        read(serial_buf,andmsg.length);        
        
	    switch(andmsg.type) {
	        case AND_M_ENROLL:
	        getDebug().printf("Enrolling id %d\r\n",(int)serial_buf[0]);
	        getScanner()->add(getMsgType(MSG_SCANNER_ENROLL,(int)serial_buf[0]));
	        break;
	        case AND_M_CALIBRATE:
	        getController()->add(getMsgType(MSG_CONTROLLER_CALIBRATE));
	        break;
	        case AND_M_OPEN:
			getController()->add(getMsgType(MSG_SCANNER_IDENTIFIED,99));
	        break;
	        case AND_M_SET_WEIGHT:
	        mg = *((int *)serial_buf);
	        getLoadCell()->setPillWeight(mg);
	        getDebug().printf("Setting weight to %d mg",mg);
	        break;
	        case AND_M_REQ_HISTORY:	        
	        getDebug().printf("AndroidSerial: AND_M_REQ_HISTORY");
	        getAndroid()->add(getMsgType(MSG_CTRL_AND_HISTORY));
	        break;
	    }
    }  
    
}
