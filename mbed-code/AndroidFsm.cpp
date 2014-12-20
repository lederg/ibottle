#include "mbed.h"
#include "AndroidFsm.hpp"
#include "smartbottle.hpp"


void AndroidFsm::init(void) {

}

int AndroidFsm::fsm_handler(message_t *msg) {
	and_message_t andmsg;
	andmsg.length = 0;

	switch(msg->type) {
		case MSG_SCN_AND_RF:
			andmsg.type = M_AND_RF;
			getAndroidSerial().send(&andmsg);
			getDebug().printf("Sending MSG_SCN_AND_RF\n");
			break;
		case MSG_SCN_AND_ENROLL:
			andmsg.type = M_AND_ENROLL;
			getDebug().printf("Sending MSG_SCN_AND_ENROLL\n");
			break;			
		case MSG_SCN_AND_ENROLL_ACK:
			andmsg.type = M_AND_ACK;
			getAndroidSerial().send(&andmsg);
			getDebug().printf("Sending M_AND_ACK\n");
			break;			
		case MSG_CTRL_AND_NEW_SAMPLE:
			andmsg.type = M_AND_NEW_SAMPLE;
			andmsg.length = 4;	
			getAndroidSerial().send(&andmsg,(void *)&(msg->data1));
			getDebug().printf("Sending M_AND_NEW_SAMPLE\n");
			break;			
		case MSG_CTRL_AND_HISTORY:
			andmsg.type = M_AND_HISTORY;
			andmsg.length = sizeof(lc_sample)*(getLoadCell()->mLastHistoryIndex);
			getAndroidSerial().send(&andmsg,&getLoadCell()->mHistory);
			getDebug().printf("Sending M_AND_HISTORY\n");
			break;			
	}
	return 0;
}