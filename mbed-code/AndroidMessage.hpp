#ifndef __ANDMESSAGE_HPP
#define __ANDMESSAGE_HPP

typedef struct {
	int header = 0xcccccccc;
    int type;   
    int length;    
} and_message_t;


#define AND_M_ENROLL 0
#define AND_M_CALIBRATE 1
#define AND_M_SET_WEIGHT 2
#define AND_M_REQ_HISTORY 3

#define M_AND_ACK 4
#define M_AND_RF 5
#define M_AND_ENROLL 6
#define M_AND_HISTORY 7
#define M_AND_NEW_SAMPLE 8

#define AND_M_OPEN 11


// typedef struct {
// 	int header = 0xcccc;
//     int type;   
//     int length;    
//     int ack_type;
//     int status;
// } and_ack_message_t;




#endif  //__ANDMESSAGE_HPP
