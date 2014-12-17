#ifndef __ANDMESSAGE_HPP
#define __ANDMESSAGE_HPP

typedef struct {
	int header = 0xcccc;
    int type;   
    int length;    
} and_message_t;

#define AND_M_ENROLL 0
#define AND_M_CALIBRATE 1
#define AND_M_SET_WEIGHT 2


#endif  //__ANDMESSAGE_HPP
