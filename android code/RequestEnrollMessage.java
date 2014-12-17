package com.example.android.BluetoothChat;

import java.nio.ByteBuffer;

import com.example.android.BluetoothChat.MessageH;

public class RequestEnrollMessage extends MessageH {

	RequestEnrollMessage(){
		super();
		mType = REQUEST_ENROLL_MSG;
		mLength = 4;
		payload = ByteBuffer.allocate(4).putInt(1).array();
	}
	
	RequestEnrollMessage(int num){
		super();
		mType = REQUEST_ENROLL_MSG;
		mLength = 4;
		payload = ByteBuffer.allocate(4).putInt(num).array();
	}
	
	public void setID(int num)
	{
		mLength = 4;
		payload = ByteBuffer.allocate(4).putInt(num).array();
	}
	
}
