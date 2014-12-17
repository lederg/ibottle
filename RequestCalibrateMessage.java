package com.example.android.BluetoothChat;

import com.example.android.BluetoothChat.MessageH;

public class RequestCalibrateMessage extends MessageH {

	RequestCalibrateMessage(){
		super();
		mType = REQUEST_CALIBRATE_MSG;
		mLength = 0;
	}
	
}
