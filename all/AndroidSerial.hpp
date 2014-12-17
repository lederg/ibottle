#ifndef __AndroidSerial_HPP
#define __AndroidSerial_HPP
#include "mbed.h"
#include "AndroidMessage.hpp"

class AndroidSerial : public Serial {
	char serial_buf[1024];

public:
    AndroidSerial(PinName _tx, PinName _rx) : Serial(_tx, _rx) {}
	void run();    
};

void android_serial_stub(void const *);

#endif  //__AndroidSerial_HPP
