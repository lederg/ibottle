package com.example.android.BluetoothChat;

import java.nio.ByteBuffer;

import com.example.android.BluetoothChat.MessageH;

public class NewSampleMessage extends MessageH{
	
	NewSampleMessage(){
		super();
		mType = NEW_SAMPLE_MSG;
	}
	
	NewSampleMessage(byte[] Pa){
		super();
		mType = NEW_SAMPLE_MSG;
		mLength = 4;
		payload = new byte[mLength];
		System.arraycopy(Pa, 0, payload, 0, mLength);
	}
	
	public void setNumP(byte[] Pa)
	{
		mLength = 4;
		payload = new byte[mLength];
		System.arraycopy(Pa, 0, payload, 0, mLength);
	}
	
	public int getNumP(){
		if(mLength>0){
			return ByteBuffer.wrap(payload).getInt();
		}
		else return 0;
	}
	
}
