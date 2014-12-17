package com.example.android.BluetoothChat;

import java.nio.ByteBuffer;

import com.example.android.BluetoothChat.MessageH;

public class SetWeightMessage extends MessageH {

	SetWeightMessage(){
		super();
		mType = SET_WEIGHT_MSG;
		mLength = 4;
	}
	
	SetWeightMessage(byte[] Pa){
		super();
		mType = SET_WEIGHT_MSG;
		mLength = 4;
		System.arraycopy(Pa, 0, payload, 0, mLength);
	}
	
	public void setWeight(int num){
		payload = ByteBuffer.allocate(4).putInt(num).array();
	}
	
}
