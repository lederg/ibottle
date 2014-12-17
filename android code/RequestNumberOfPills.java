package com.example.android.BluetoothChat;

import com.example.android.BluetoothChat.MessageH;

public class RequestNumberOfPills extends MessageH{
	
	RequestNumberOfPills(){
		super();
		mType = REQUEST_NUMBER_OF_PILLS_MSG;
		mLength = 0;
	}
	
}
