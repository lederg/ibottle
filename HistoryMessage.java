package com.example.android.BluetoothChat;

import java.nio.ByteBuffer;

import com.example.android.BluetoothChat.MessageH;

public class HistoryMessage extends MessageH{

	HistoryMessage(){
		super();
		mType = HISTORY_MSG;
		mLength = 0;
	}
	
	HistoryMessage(int L, byte[] P){
		mType = HISTORY_MSG;
		mLength = L;
		payload = new byte[L];
		System.arraycopy(P, 0, payload, 0, L);
	}
	
}
