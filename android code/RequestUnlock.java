package com.example.android.BluetoothChat;

import com.example.android.BluetoothChat.MessageH;

public class RequestUnlock extends MessageH {
	
	RequestUnlock(){
		super();
		mType = REQUEST_UNLOCK;
		mLength = 0;
	}

}
