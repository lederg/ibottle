package com.example.android.BluetoothChat;

import com.example.android.BluetoothChat.MessageH;

public class RequestHistoryMessage extends MessageH{

	RequestHistoryMessage(){
		super();
		mType = REQUEST_HISTORY_MSG;
		mLength = 0;
	}
	
}
