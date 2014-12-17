package com.example.android.BluetoothChat;

import com.example.android.BluetoothChat.MessageH;

public class RequestRemoveFingerMessage extends MessageH {

	RequestRemoveFingerMessage(){
		super();
		mType = REQUEST_REMOVE_FINGER;
		mLength = 0;
	}
	
}
