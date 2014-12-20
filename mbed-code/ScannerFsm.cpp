#include "smartbottle.hpp"
#include "ScannerFsm.hpp"
#include "GT511C3.hpp"

#define STATE_SCN_IDLE 0
#define STATE_SCN_ENROLL1 1
#define STATE_SCN_ENROLL2 2
#define STATE_SCN_ENROLL3 3

void ScannerFsm::init(void) {

}

void ScannerFsm::open() {
	if(!bOpen) {
		mScanner.Open();
		mScanner.CmosLed(1);
		bOpen = true;
	}
}

void ScannerFsm::do_scan() {
	if(!bOpen) {
		getDebug().printf("Error! scanner should be open in do_scan\n");
	} else if(mScanner.IsPress() && mScanner.Capture(1)) {
		int id = mScanner.Identify();		
		getDebug().printf("Scanner identified id: %d\n",id);
		getController()->add(getMsgType(MSG_SCANNER_IDENTIFIED,id));
	}
}

void ScannerFsm::do_enroll(int id) {
	open();
	getDebug().printf("Starting enroll for id %d\n",id);
	// if(mScanner.CheckEnrolled(id)) {
		getDebug().printf("Deleting id %d...\n",id);
		if(mScanner.DeleteID(id) == 0){
               getDebug().printf("Delete OK!\n");
        }
	// }

    unsigned long Parameter = 0;
    unsigned short Response = 0;
    int sts = 0;

    while(1){
        Parameter = id;
        sts = mScanner.SendRecv(mScanner.CMD_EnrollStart,&Parameter,&Response);
        if(sts != 0) {
            getAndroid()->add(getMsgType(MSG_SCN_AND_ENROLL_ACK,sts));
            return;
        }
        if(Response != mScanner.CMD_Ack) {
            getAndroid()->add(getMsgType(MSG_SCN_AND_ENROLL_ACK,100));
            return;
        }
        
        mScanner.WaitPress(0);

        while(1){
            getAndroid()->add(getMsgType(MSG_SCN_AND_ENROLL,1));
            mScanner.WaitPress(1);
            if(mScanner.Capture(1) == 0)
                break;
        }

        getAndroid()->add(getMsgType(MSG_SCN_AND_RF));
        if(mScanner.Enroll_N(1) != 0)
            continue;
        mScanner.WaitPress(0);

        while(1){
            getAndroid()->add(getMsgType(MSG_SCN_AND_ENROLL,2));
            mScanner.WaitPress(1);
            if(mScanner.Capture(1) == 0)
                break;
        }

        getAndroid()->add(getMsgType(MSG_SCN_AND_RF));
        if(mScanner.Enroll_N(2) != 0)
            continue;
        mScanner.WaitPress(0);

        while(1){
            getAndroid()->add(getMsgType(MSG_SCN_AND_ENROLL,3));            
            mScanner.WaitPress(1);
            if(mScanner.Capture(1) == 0)
                break;
        }

        getAndroid()->add(getMsgType(MSG_SCN_AND_RF));
        if(mScanner.Enroll_N(3) != 0)
            continue;
        mScanner.WaitPress(0);

        getAndroid()->add(getMsgType(MSG_SCN_AND_ENROLL_ACK));
        break;
    }
    close();
}

void ScannerFsm::close() {	
	if(bOpen) {
		mScanner.CmosLed(0);
		// mScanner.Close();
		bOpen = false;
	}
}

int ScannerFsm::fsm_handler(message_t *msg) {
    getDebug().printf("Got message of type %d\n",msg->type);
    switch(mState) {
        case STATE_SCN_IDLE:

        switch(msg->type) {
            case MSG_SCANNER_OPEN:
			getDebug().printf("Got message of type %d\n",msg->type);
			open();
			break;
			case MSG_SCANNER_SCAN:
			do_scan();
			break;
			case MSG_SCANNER_ENROLL:
			do_enroll(msg->data1);
			break;
			case MSG_SCANNER_CLOSE:
			close();
			break;
			default:
			break;
		}


		break;
	}

	return 0;
}


