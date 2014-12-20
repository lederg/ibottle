#ifndef __FSMMESSAGE_HPP
#define __FSMMESSAGE_HPP

#define MSG_MEMPOOL_SIZE 128

typedef struct {
    int    type;   /* AD result of measured voltage */
    uint32_t data1;   /* A counter value               */
    uint32_t data2;   /* A counter value               */
} message_t;

#define MSG_SCANNER_OPEN 0
#define MSG_SCANNER_SCAN 1
#define MSG_SCANNER_CLOSE 2
#define MSG_SCANNER_IDENTIFIED 3
#define MSG_SCANNER_ENROLL 4
#define MSG_SCANNER_ENROLL_ACK 5
#define MSG_TIMEUP 6


#define MSG_SCN_AND_RF 7
#define MSG_SCN_AND_ENROLL 8
#define MSG_SCN_AND_ENROLL_ACK 9


#define MSG_CONTROLLER_ACTIVE 10
#define MSG_CONTROLLER_INACTIVE 11
#define MSG_CONTROLLER_RESET 12
#define MSG_CONTROLLER_CALIBRATE 14


#define MSG_CTRL_AND_HISTORY 15
#define MSG_CTRL_AND_NEW_SAMPLE 16
#define MSG_CTRL_PRINT_STATE 17


#define MSG_CONTROLLER_TICK 18


#endif  //__FSMMESSAGE_HPP
