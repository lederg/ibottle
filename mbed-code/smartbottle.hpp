#include "mbed.h"
#include "AndroidMessage.hpp"
#include "AndroidSerial.hpp"
#include "FsmThread.hpp"
#include "ServoLock.hpp"
#include "LoadCell.hpp"
#include "AdxlModule.hpp"
#include "rtos.h"


FsmThread *getController();
FsmThread *getScanner();
FsmThread *getAndroid();
LoadCell *getLoadCell();
AdxlModule &getAdxl();
ServoLock &getLock();
Serial &getDebug();
AndroidSerial &getAndroidSerial();

MemoryPool<message_t, MSG_MEMPOOL_SIZE> *getMemPool();
message_t *getMsgType(int,int data=0);
