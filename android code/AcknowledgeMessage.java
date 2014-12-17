package com.example.android.BluetoothChat;

import com.example.android.BluetoothChat.MessageH;

public class AcknowledgeMessage extends MessageH {

	AcknowledgeMessage(){
		super();
		mType = ACK_MESSAGE;
		mLength = 0;
	}
	
}
