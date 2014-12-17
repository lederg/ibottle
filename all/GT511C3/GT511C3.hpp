/**
 * @section LICENSE
 *
 * Copyright (c) 2013 @tosihisa, MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of this software 
 * and associated documentation files (the "Software"), to deal in the Software without restriction, 
 * including without limitation the rights to use, copy, modify, merge, publish, distribute, 
 * sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all copies or 
 * substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING 
 * BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND 
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, 
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * @section DESCRIPTION
 *
 * Fingerprint reader module "GT-511C3" class.
 *
 * http://www.adh-tech.com.tw/?22,gt-511c3-gt-511c31
 * http://www.adh-tech.com.tw/files/GT-511C3_datasheet_V1%201_20131127[1].pdf
 * https://www.sparkfun.com/products/11792
 * https://github.com/sparkfun/Fingerprint_Scanner-TTL/ 
 * http://blog.digit-parts.com/archives/51894096.html
 */
#ifndef __GT511C3_HPP
#define __GT511C3_HPP
#include "mbed.h"

class GT511C3 : public Serial {
public:
    enum Command {
        CMD_Open = 0x01, // 01 Open Initialization 
        CMD_Close = 0x02, // 02 Close Termination 
        CMD_UsbInternalCheck = 0x03, // 03 UsbInternalCheck Check if the connected USB device is valid 
        CMD_ChangeBaudrate = 0x04, // 04 ChangeBaudrate Change UART baud rate 
        CMD_SetIAPMode = 0x05, // 05 SetIAPMode Enter IAP Mode In this mode, FW Upgrade is available 
        CMD_CmosLed = 0x12, // 12 CmosLed Control CMOS LED 
        CMD_GetEnrollCount = 0x20, // 20 GetEnrollCount Get enrolled fingerprint count 
        CMD_CheckEnrolled = 0x21, // 21 CheckEnrolled Check whether the specified ID is already enrolled 
        CMD_EnrollStart = 0x22, // 22 EnrollStart Start an enrollment 
        CMD_Enroll1 = 0x23, // 23 Enroll1 Make 1st template for an enrollment 
        CMD_Enroll2 = 0x24, // 24 Enroll2 Make 2nd template for an enrollment 
        CMD_Enroll3 = 0x25, // 25 Enroll3  Make 3rd template for an enrollment, merge three templates into one template, save merged template to the database 
        CMD_IsPressFinger = 0x26, // 26 IsPressFinger Check if a finger is placed on the sensor 
        CMD_DeleteID = 0x40, // 40 DeleteID Delete the fingerprint with the specified ID 
        CMD_DeleteAll = 0x41, // 41 DeleteAll Delete all fingerprints from the database 
        CMD_Verify = 0x50, // 50 Verify 1:1 Verification of the capture fingerprint image with the specified ID 
        CMD_Identify = 0x51, // 51 Identify 1:N Identification of the capture fingerprint image with the database 
        CMD_VerifyTemplate = 0x52, // 52 VerifyTemplate 1:1 Verification of a fingerprint template with the specified ID 
        CMD_IdentifyTemplate = 0x53, // 53 IdentifyTemplate 1:N Identification of a fingerprint template with the database 
        CMD_CaptureFinger = 0x60, // 60 CaptureFinger Capture a fingerprint image(256x256) from the sensor 
        CMD_MakeTemplate = 0x61, // 61 MakeTemplate Make template for transmission
        CMD_GetImage = 0x62, // 62 GetImage Download the captured fingerprint image(256x256) 
        CMD_GetRawImage = 0x63, // 63 GetRawImage Capture & Download raw fingerprint image(320x240) 
        CMD_GetTemplate = 0x70, // 70 GetTemplate Download the template of the specified ID 
        CMD_SetTemplate = 0x71, // 71 SetTemplate Upload the template of the specified ID 
        CMD_GetDatabaseStart = 0x72, // 72 GetDatabaseStart Start database download, obsolete 
        CMD_GetDatabaseEnd = 0x73, // 73 GetDatabaseEnd End database download, obsolete 
        CMD_UpgradeFirmware = 0x80, // 80 UpgradeFirmware Not supported 
        CMD_UpgradeISOCDImage = 0x81, // 81 UpgradeISOCDImage Not supported 
        CMD_Ack = 0x30, // 30 Ack Acknowledge. 
        CMD_Nack = 0x31, // 31 Nack Non-acknowledge.
    };
    enum Error {
        NACK_TIMEOUT = 0x1001, // NACK_TIMEOUT 0x1001 Obsolete, capture timeout 
        NACK_INVALID_BAUDRATE = 0x1002, // NACK_INVALID_BAUDRATE 0x1002 Obsolete, Invalid serial baud rate 
        NACK_INVALID_POS = 0x1003, // NACK_INVALID_POS 0x1003 The specified ID is not between 0~199 
        NACK_IS_NOT_USED = 0x1004, // NACK_IS_NOT_USED 0x1004 The specified ID is not used 
        NACK_IS_ALREADY_USED = 0x1005, // NACK_IS_ALREADY_USED 0x1005 The specified ID is already used 
        NACK_COMM_ERR = 0x1006, // NACK_COMM_ERR 0x1006 Communication Error 
        NACK_VERIFY_FAILED = 0x1007, // NACK_VERIFY_FAILED 0x1007 1:1 Verification Failure 
        NACK_IDENTIFY_FAILED = 0x1008, // NACK_IDENTIFY_FAILED 0x1008 1:N Identification Failure 
        NACK_DB_IS_FULL = 0x1009, // NACK_DB_IS_FULL 0x1009 The database is full 
        NACK_DB_IS_EMPTY = 0x100A, // NACK_DB_IS_EMPTY 0x100A The database is empty 
        NACK_TURN_ERR = 0x100B, // NACK_TURN_ERR 0x100B Obsolete, Invalid order of the enrollment (The order was not as: EnrollStart -> Enroll1 -> Enroll2 -> Enroll3) 
        NACK_BAD_FINGER = 0x100C, // NACK_BAD_FINGER 0x100C Too bad fingerprint 
        NACK_ENROLL_FAILED = 0x100D, // NACK_ENROLL_FAILED 0x100D Enrollment Failure 
        NACK_IS_NOT_SUPPORTED = 0x100E, // NACK_IS_NOT_SUPPORTED 0x100E The specified command is not supported 
        NACK_DEV_ERR = 0x100F, // NACK_DEV_ERR 0x100F Device Error, especially if Crypto-Chip is trouble 
        NACK_CAPTURE_CANCELED = 0x1010, // NACK_CAPTURE_CANCELED 0x1010 Obsolete, The capturing is canceled 
        NACK_INVALID_PARAM = 0x1011, // NACK_INVALID_PARAM 0x1011 Invalid parameter 
        NACK_FINGER_IS_NOT_PRESSED = 0x1012, // NACK_FINGER_IS_NOT_PRESSED 0x1012 Finger is not pressed 
        NACK_IO_ERR = 0xF000,   // ORIGINAL ERROR CODE. Serial line error.
    };
    unsigned long LastError;
    unsigned long FirmwareVersion; 
    unsigned long IsoAreaMaxSize; 
    unsigned char DeviceSerialNumber[16]; 

    GT511C3(PinName _tx, PinName _rx) : Serial(_tx,_rx) , LastError(0) {}
    int Init(void);
    int SendCommand(unsigned long Parameter,unsigned short Command);
    int RecvResponse(unsigned long *Parameter,unsigned short *Response);
    int RecvData(unsigned char *data,unsigned long size);
    int SendRecv(unsigned short Command,unsigned long *Parameter,unsigned short *Response);
    int ClearLine(void);
    int Open(void);
    int WaitPress(int press);
    int CmosLed(int onoff);
    int IsPress(void);
    int Capture(int best);
    int Enroll_N(int N);
    int Identify(void);
    int Enroll(int ID,int (*progress)(int status,char *msg));
    int CheckEnrolled(int ID);
    int DeleteID(int ID);
};

#endif  //__GT511C3_HPP
