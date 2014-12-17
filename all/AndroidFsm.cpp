#include "mbed.h"
#include "smartbottle.hpp"
#include "AndroidFsm.hpp"


void AndroidFsm::init(void) {

}

int AndroidFsm::fsm_handler(message_t *msg) {
	switch(msg->type) {
		case MSG_SCN_AND_RF:
			getDebug().printf("MSG_SCN_AND_RF\n");
			break;
		case MSG_SCN_AND_ENROLL:
			getDebug().printf("MSG_SCN_AND_ENROLL\n");
			break;			
	}
	return 0;
}