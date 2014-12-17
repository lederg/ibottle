package com.example.android.BluetoothChat;

import java.nio.ByteBuffer;
import com.example.android.BluetoothChat.MessageH;

public class NumberOfPillsMessage extends MessageH{

	//----------------------------- Constructors
	NumberOfPillsMessage(){
		super();
		mType=NUMBER_OF_PILLS_MSG;
	}
	
	NumberOfPillsMessage(byte[] A){
		super();
		mType=NUMBER_OF_PILLS_MSG;
		
		byte[] tmp = new byte[4];
		System.arraycopy(A, 0, tmp, 0, 4);
		mLength = ByteBuffer.wrap(tmp).getInt();
		
		payload = new byte[A.length - 4];
		System.arraycopy(A, 4, payload, 0, mLength);		
	}
	
	// ---------------------------- Methods
	public int NumberOfPills(){
		byte[] tmp = new byte[4];
		System.arraycopy(payload, 0, tmp, 4-mLength, mLength);
		for(int i=0; i<(4-mLength); i++) tmp[i]=0;
		int pills = (ByteBuffer.wrap(tmp).getInt()) ;
		
		return pills;
	}
	
}
