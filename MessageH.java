package com.example.android.BluetoothChat;

import java.nio.ByteBuffer;
import java.util.Arrays;

public class MessageH {

	int mHeader;
	int mType;
	int mLength;
	byte[] payload;
	// ----------------------- Messages

	public final static int REQUEST_ENROLL_MSG 			= 0;//DONE
	public final static int REQUEST_CALIBRATE_MSG 		= 1; //DONE
	public final static int SET_WEIGHT_MSG 				= 2; //DONE
	public final static int REQUEST_HISTORY_MSG 		= 3;
	
	public final static int ACK_MESSAGE 				= 4; //DONE
	public final static int REQUEST_REMOVE_FINGER 		= 5;//DONE
	public final static int PRESS_FINGER_MSG 			= 6;
	
	public final static int HISTORY_MSG 				= 7; //DONE
	
	public final static int NEW_SAMPLE_MSG 				= 8; //DONE
	
	public final static int REQUEST_NUMBER_OF_PILLS_MSG = 9; //DONE
	public final static int NUMBER_OF_PILLS_MSG 		= 10; //DONE
	public final static int REQUEST_UNLOCK 				= 11;
	//public final int FINGER_PRINT_ACCEPTED = ;
	//public final int FINGER_PRINT_REJECTED = ;
	
	// ----------------------- Constructors
	MessageH(){
		mHeader=0xCCCCCCCC;
	}
	
	MessageH(int T, int L, byte[] P){
		mHeader=0xCCCCCCCC;
		mType = T;
		mLength = L;
		payload = new byte[mLength];
		System.arraycopy(P, 0, payload, 0, L);
	}
	
	//------------------------- Setters
	public void setType(int T){
		mType = T;
	}
	public void setLength(int L){
		mLength = L;
	}
	public void setPayload(byte[] P){
		payload = new byte[P.length];
		System.arraycopy(P, 0, payload, 0, P.length);
		mLength = payload.length;
	}
	
	//------------------------ Getters
	public int getType(){
		return mType;
	}
	public int getLength(){
		return mLength;
	}
	public byte[] getPayload(){
		return payload;
	}
	
	//------------------------ Methods
	public byte[] toByte(){
		int tempHeader = mHeader;
		int tempType = mType;
		int tempLength = mLength;
		
		//Bytes will be placed in the byte array
		byte[] outputByteArray = new byte[4+4+4+tempLength]; 
		
		ByteBuffer tmp = ByteBuffer.allocate(4);
		//Insert tempHeader into outputByteArray
		tmp.putInt(tempHeader);
		System.arraycopy(tmp.array(),0,outputByteArray,0,4);
		tmp.clear();
		//Insert tempType into outputByteArray
		tmp.putInt(tempType);
		System.arraycopy(tmp.array(),0,outputByteArray,4,4);
		tmp.clear();
		//Insert tempHeader into outputByteArray
		tmp.putInt(tempLength);
		System.arraycopy(tmp.array(),0,outputByteArray,8,4);
		tmp.clear();
		
		//Insert tempPayload to outputByteArray
		if(tempLength>0 ) System.arraycopy(payload, 0, outputByteArray, 12, tempLength);
		
		return outputByteArray;
	}
	
	
	
	
	
	
	
	
	
	
	public static MessageH fromByte(byte[] ArrayOfBytes){
		//Check the header
		byte[] tmp = new byte[4];
		System.arraycopy(ArrayOfBytes, 0, tmp, 0, 4);
		int tmpHeader = ByteBuffer.wrap(tmp).getInt();
		
		if(tmpHeader == 0xCCCCCCCC){
			
			//Get the Type of MessageH
			System.arraycopy(ArrayOfBytes, 4, tmp, 0, 4);
			int tmpType = ByteBuffer.wrap(tmp).getInt();
			
			switch(tmpType){
			case REQUEST_ENROLL_MSG: 							//0
				return new RequestEnrollMessage();
			case REQUEST_CALIBRATE_MSG: 						//1
				return new RequestCalibrateMessage();
			case SET_WEIGHT_MSG: 								//2
				return new SetWeightMessage();
			case REQUEST_HISTORY_MSG: 							//3
				return new RequestHistoryMessage();	
			case ACK_MESSAGE: 									//4
				return new AcknowledgeMessage();	
			case REQUEST_REMOVE_FINGER: 						//5
				return new RequestRemoveFingerMessage();
			case PRESS_FINGER_MSG:								//6
				return new RequestPressFingerMessage();
			case HISTORY_MSG: 									//7
				System.arraycopy(ArrayOfBytes, 8, tmp, 0, 4);
				int l = ByteBuffer.wrap(tmp).getInt();
				byte[] p = new byte[l];
				System.arraycopy(ArrayOfBytes, 12, p, 0, l);
				return new HistoryMessage(l,p);	
			case NEW_SAMPLE_MSG: 								//8
				byte[] s = new byte[4];
				System.arraycopy(ArrayOfBytes, 12, s, 0, 4);
				return new NewSampleMessage(s);	
			case REQUEST_NUMBER_OF_PILLS_MSG: 					//9
				return new RequestNumberOfPills();
			case NUMBER_OF_PILLS_MSG: 							//10
				byte[] tmpLengthAndPayload = new byte[ArrayOfBytes.length - 8];
				System.arraycopy(ArrayOfBytes, 8, tmpLengthAndPayload, 0, tmpLengthAndPayload.length);
				return new NumberOfPillsMessage(tmpLengthAndPayload);
			case REQUEST_UNLOCK:								//11
				return new RequestUnlock();
			default: 
				return null;
			}
		}
		else return null;
		
	}
	
	
}
