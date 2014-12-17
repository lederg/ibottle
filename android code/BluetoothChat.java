/*
 * Copyright (C) 2009 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * 
 * THIS CODE AND ANY (OTHER CODE ASSOCIATED WITH IT) HAS BEEN MODIFIED BY:
 * Vicenc Rubies Royo, vrubies@berkeley.edu
 * 
 * */

package com.example.android.BluetoothChat;

import com.example.android.BluetoothChat.MessageH;
import com.example.android.BluetoothChat.NumberOfPillsMessage;
import com.example.android.BluetoothChat.RequestEnrollMessage;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.graphics.drawable.Drawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.SystemClock;
import android.util.Log;
import android.view.KeyEvent;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.Window;
import android.view.View.OnClickListener;
import android.view.inputmethod.EditorInfo;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;
import android.widget.ViewFlipper;


/**
 * This is the main Activity that displays the current chat session.
 */
public class BluetoothChat extends Activity implements OnClickListener {
    // Debugging
    private static final String TAG = "BluetoothChat";
    private static final boolean D = true;

    // Message types sent from the BluetoothChatService Handler
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final int MESSAGE_READ = 2;
    public static final int MESSAGE_WRITE = 3;
    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final int MBED_MESSAGE_READ = 6;

    // Key names received from the BluetoothChatService Handler
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";

    // Intent request codes
    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;

    // Layout Views
    private TextView mTitle,FingerPrint,infodisplay;
    private ListView mConversationView;
    private EditText mOutEditText,SetWeightET,getIDbox;
    private Button mConnect,Help,About,Cali,Setw,PillH,CFP,Setw2,ret1,ret2,disc,gotoenroll,unlock;
    private ViewFlipper viewFlipper;

    // Name of the connected device
    private String mConnectedDeviceName = null;
    // Array adapter for the conversation thread
    private ArrayAdapter<String> mConversationArrayAdapter;
    // String buffer for outgoing messages
    private StringBuffer mOutStringBuffer;
    // Local Bluetooth adapter
    private BluetoothAdapter mBluetoothAdapter = null;
    // Member object for the chat services
    private BluetoothChatService mChatService = null;

    private int i=0;
    private String checker = "$$$",fetchm="";

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        if(D) Log.e(TAG, "+++ ON CREATE +++");

        // Set up the window layout
        requestWindowFeature(Window.FEATURE_CUSTOM_TITLE);
        setContentView(R.layout.main);
        getWindow().setFeatureInt(Window.FEATURE_CUSTOM_TITLE, R.layout.custom_title);

        // Set up the custom title
        mTitle = (TextView) findViewById(R.id.title_left_text);
        mTitle.setText(R.string.app_name);
        mTitle = (TextView) findViewById(R.id.title_right_text);
        
        // Set Edit Text
        SetWeightET = (EditText) findViewById(R.id.weightbox);
        getIDbox = (EditText) findViewById(R.id.getIDbox);
        
        // Set up ViewFlipper
        viewFlipper = (ViewFlipper) findViewById(R.id.myViewFlipper);
        
        //Set up TextView
        FingerPrint = (TextView) findViewById(R.id.FPrint);
        infodisplay = (TextView) findViewById(R.id.infdisplay);
        
        //Set up images
        Drawable FPPic = getResources().getDrawable(R.drawable.fingerprint);
        FPPic.setAlpha(70);
        FingerPrint.setBackgroundDrawable(FPPic);
        
        //Set up buttons
        mConnect = (Button) findViewById(R.id.connect);
        mConnect.setOnClickListener(this);
        Help = (Button) findViewById(R.id.helpbutton);
        Help.setOnClickListener(this);
        About = (Button) findViewById(R.id.aboutbutton);
        About.setOnClickListener(this);
        Cali = (Button) findViewById(R.id.calibottle);
        Cali.setOnClickListener(this);
        Setw = (Button) findViewById(R.id.setweight);
        Setw.setOnClickListener(this);
        Setw2 = (Button) findViewById(R.id.weightbutton);
        Setw2.setOnClickListener(this);
        PillH = (Button) findViewById(R.id.history);
        PillH.setOnClickListener(this);
        gotoenroll = (Button) findViewById(R.id.gotoenroll);
        gotoenroll.setOnClickListener(this);
        CFP = (Button) findViewById(R.id.ecfprint);
        CFP.setOnClickListener(this);
        ret1 = (Button) findViewById(R.id.ret1);
        ret1.setOnClickListener(this);
        ret2 = (Button) findViewById(R.id.ret2);
        ret2.setOnClickListener(this);
        disc = (Button) findViewById(R.id.disconnect);
        disc.setOnClickListener(this);
        unlock = (Button) findViewById(R.id.unlock);
        unlock.setOnClickListener(this);
        
        // Get local Bluetooth adapter
        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();

        // If the adapter is null, then Bluetooth is not supported
        if (mBluetoothAdapter == null) {
            Toast.makeText(this, "Bluetooth is not available", Toast.LENGTH_LONG).show();
            finish();
            return;
        }
    }

    @Override
    public void onStart() {
        super.onStart();
        if(D) Log.e(TAG, "++ ON START ++");

        // If BT is not on, request that it be enabled.
        // setupChat() will then be called during onActivityResult
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
        // Otherwise, setup the chat session
        } else {
            if (mChatService == null) setupChat();
        }
    }

    @Override
    public synchronized void onResume() {
        super.onResume();
        if(D) Log.e(TAG, "+ ON RESUME +");

        // Performing this check in onResume() covers the case in which BT was
        // not enabled during onStart(), so we were paused to enable it...
        // onResume() will be called when ACTION_REQUEST_ENABLE activity returns.
        if (mChatService != null) {
            // Only if the state is STATE_NONE, do we know that we haven't started already
            if (mChatService.getState() == BluetoothChatService.STATE_NONE) {
              // Start the Bluetooth chat services
              mChatService.start();
            }
        }
    }

    private void setupChat() {
        Log.d(TAG, "setupChat()");

        // Initialize the array adapter for the conversation thread
        mConversationArrayAdapter = new ArrayAdapter<String>(this, R.layout.message);
        mConversationView = (ListView) findViewById(R.id.in);
        mConversationView.setAdapter(mConversationArrayAdapter);

        // Initialize the compose field with a listener for the return key
        mOutEditText = (EditText) findViewById(R.id.edit_text_out);
        mOutEditText.setOnEditorActionListener(mWriteListener);

        // Initialize the BluetoothChatService to perform bluetooth connections
        mChatService = new BluetoothChatService(this, mHandler);

        // Initialize the buffer for outgoing messages
        mOutStringBuffer = new StringBuffer("");
    }

    @Override
    public synchronized void onPause() {
        super.onPause();
        if(D) Log.e(TAG, "- ON PAUSE -");
    }

    @Override
    public void onStop() {
        super.onStop();
        if(D) Log.e(TAG, "-- ON STOP --");
    }

    @Override
    public void onDestroy() {
        super.onDestroy();
        // Stop the Bluetooth chat services
        if (mChatService != null) mChatService.stop();
        if(D) Log.e(TAG, "--- ON DESTROY ---");
    }

    private void ensureDiscoverable() {
        if(D) Log.d(TAG, "ensure discoverable");
        if (mBluetoothAdapter.getScanMode() !=
            BluetoothAdapter.SCAN_MODE_CONNECTABLE_DISCOVERABLE) {
            Intent discoverableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_DISCOVERABLE);
            discoverableIntent.putExtra(BluetoothAdapter.EXTRA_DISCOVERABLE_DURATION, 300);
            startActivity(discoverableIntent);
        }
    }
    
    private void readMessage(String binput){
    	// Check that we're actually connected before trying anything
        if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
            Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }
        
        mChatService.read();
    }
    
    /**
     * Sends a message.
     * @param message  A string of text to send.
     */
    private void sendMessage(String message) {
        // Check that we're actually connected before trying anything
        if (mChatService.getState() != BluetoothChatService.STATE_CONNECTED) {
            Toast.makeText(this, R.string.not_connected, Toast.LENGTH_SHORT).show();
            return;
        }
        if(!message.contentEquals(checker)) message = message + (char)13 + (char)10;
        // Check that there's actually something to send
        if (message.length() > 0) {
            // Get the message bytes and tell the BluetoothChatService to write
            byte[] send = message.getBytes();
            mChatService.write(send);
            
            // Reset out string buffer to zero and clear the edit text field
            mOutStringBuffer.setLength(0);
            mOutEditText.setText(mOutStringBuffer);
        }
        
    }

    // The action listener for the EditText widget, to listen for the return key
    private TextView.OnEditorActionListener mWriteListener =
        new TextView.OnEditorActionListener() {
        public boolean onEditorAction(TextView view, int actionId, KeyEvent event) {
            // If the action is a key-up event on the return key, send the message
            if (actionId == EditorInfo.IME_NULL && event.getAction() == KeyEvent.ACTION_UP) {
                String message = view.getText().toString();
                sendMessage(message);
            }
            if(D) Log.i(TAG, "END onEditorAction");
            return true;
        }
    };

    // The Handler that gets information back from the BluetoothChatService
    private final Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
            case MESSAGE_STATE_CHANGE:
                if(D) Log.i(TAG, "MESSAGE_STATE_CHANGE: " + msg.arg1);
                switch (msg.arg1) {
                case BluetoothChatService.STATE_CONNECTED:
                    mTitle.setText(R.string.title_connected_to);
                    mTitle.append(mConnectedDeviceName);
                    mConversationArrayAdapter.clear();
                    break;
                case BluetoothChatService.STATE_CONNECTING:
                    mTitle.setText(R.string.title_connecting);
                    break;
                case BluetoothChatService.STATE_LISTEN:
                case BluetoothChatService.STATE_NONE:
                    mTitle.setText(R.string.title_not_connected);
                    break;
                }
                break;
            case MESSAGE_WRITE:
                byte[] writeBuf = (byte[]) msg.obj;
                // construct a string from the buffer
                String writeMessage = new String(writeBuf);
                mConversationArrayAdapter.add("Me:  " + writeMessage + " " + fetchm);
                break;
            case MESSAGE_READ: //TODO
                byte[] readBuf = (byte[]) msg.obj;
                // construct a string from the valid bytes in the buffer
                String readMessage = new String(readBuf, 0, msg.arg1);
                mConversationArrayAdapter.add(mConnectedDeviceName+":  " + readMessage + " " + fetchm);
                break;
            case MESSAGE_DEVICE_NAME:
                // save the connected device's name
                mConnectedDeviceName = msg.getData().getString(DEVICE_NAME);
                Toast.makeText(getApplicationContext(), "Connected to "
                               + mConnectedDeviceName, Toast.LENGTH_SHORT).show();
                break;
            case MESSAGE_TOAST:
                Toast.makeText(getApplicationContext(), msg.getData().getString(TOAST),
                               Toast.LENGTH_SHORT).show();
                break;
            case MBED_MESSAGE_READ:
            	
            	MessageH temp = (MessageH) msg.obj;
            	if(temp.getType() == MessageH.REQUEST_ENROLL_MSG){
            		RequestEnrollMessage temp2 = (RequestEnrollMessage) temp; 			//0
            		//fetchm=fetchm+"REM_FING";
            	}
            	else if(temp.getType() == MessageH.REQUEST_CALIBRATE_MSG){
            		RequestCalibrateMessage temp2 = (RequestCalibrateMessage) temp;		//1
            		//fetchm=fetchm+"REM_FING";
            	}
            	else if(temp.getType() == MessageH.SET_WEIGHT_MSG){
            		SetWeightMessage temp2 = (SetWeightMessage) temp;					//2
            		//fetchm=fetchm+"REM_FING";
            	}
            	else if(temp.getType() == MessageH.REQUEST_ENROLL_MSG){
            		RequestHistoryMessage temp2 = (RequestHistoryMessage) temp;			//3
            		//fetchm=fetchm+"REM_FING";
            	}
            	else if(temp.getType() == MessageH.ACK_MESSAGE){
            		AcknowledgeMessage temp2 = (AcknowledgeMessage) temp;				//4
            		//fetchm=fetchm+"REM_FING";
            		FingerPrint.setText("FINGER PRINT STORED, RETURNING TO MENU...");
            		SystemClock.sleep(1000);
            		viewFlipper.setDisplayedChild(viewFlipper.indexOfChild(findViewById(R.id.MainMenu)));
            	}
            	else if(temp.getType() == MessageH.REQUEST_REMOVE_FINGER){
            		RequestRemoveFingerMessage temp2 = (RequestRemoveFingerMessage) temp; //5
            		FingerPrint.setText("REMOVE FINGER");
            		//fetchm=fetchm+"REM_FING";
            	}
            	else if(temp.getType() == MessageH.PRESS_FINGER_MSG){
            		RequestPressFingerMessage temp2 = (RequestPressFingerMessage) temp; //6
            		FingerPrint.setText("PLEASE, PRESS FINGER AGAINST SCANNER");
            	}
            	else if(temp.getType() == MessageH.HISTORY_MSG){
            		HistoryMessage temp2 = (HistoryMessage) temp;						//7
            		//fetchm=fetchm+"REM_FING";
            	}
            	else if(temp.getType() == MessageH.NEW_SAMPLE_MSG){
            		NewSampleMessage temp2 = (NewSampleMessage) temp;					//8
            		infodisplay.setText("Num. Of Pills: " + String.valueOf(temp2.getNumP()));
            	}
            	else if(temp.getType() == MessageH.REQUEST_NUMBER_OF_PILLS_MSG){
            		RequestNumberOfPills temp2 = (RequestNumberOfPills) temp; 			//9
            	}
            	else if(temp.getType() == MessageH.NUMBER_OF_PILLS_MSG){
            		NumberOfPillsMessage temp2 = (NumberOfPillsMessage) temp;			//10
            	}
            	else if(temp.getType() == MessageH.REQUEST_UNLOCK){
            		RequestUnlock temp2 = (RequestUnlock) temp;							//11
            	}
            	break;
            }
        }
    };

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if(D) Log.d(TAG, "onActivityResult " + resultCode);
        switch (requestCode) {
        case REQUEST_CONNECT_DEVICE:
            // When DeviceListActivity returns with a device to connect
            if (resultCode == Activity.RESULT_OK) {
                // Get the device MAC address
                String address = data.getExtras()
                                     .getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                // Get the BLuetoothDevice object
                BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
                // Attempt to connect to the device
                mChatService.connect(device);
            }
            break;
        case REQUEST_ENABLE_BT:
            // When the request to enable Bluetooth returns
            if (resultCode == Activity.RESULT_OK) {
                // Bluetooth is now enabled, so set up a chat session
                setupChat();
            } else {
                // User did not enable Bluetooth or an error occured
                Log.d(TAG, "BT not enabled");
                Toast.makeText(this, R.string.bt_not_enabled_leaving, Toast.LENGTH_SHORT).show();
                finish();
            }
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.option_menu, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        case R.id.scan:
            // Launch the DeviceListActivity to see devices and do scan
            Intent serverIntent = new Intent(this, DeviceListActivity.class);
            startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
            return true;
        case R.id.discoverable:
            // Ensure this device is discoverable by others
            ensureDiscoverable();
            return true;
        }
        return false;
    }

	@Override
	public void onClick(View arg0) {
		switch(arg0.getId()){
		case R.id.button_send: 
			TextView view = (TextView) findViewById(R.id.edit_text_out); 
			String message = view.getText().toString();
			sendMessage(message);
			break;
		case R.id.connect:
			if((mChatService.getState() == BluetoothChatService.STATE_NONE) ||
			   (mChatService.getState() == BluetoothChatService.STATE_LISTEN))
			{
				Intent serverIntent = new Intent(this, DeviceListActivity.class);
				startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
			}
			else{
				//mChatService.write((new RequestHistoryMessage()).toByte());
				viewFlipper.setDisplayedChild(viewFlipper.indexOfChild(findViewById(R.id.MainMenu)));
			}
            break;
		case R.id.helpbutton:
			
			break;
		case R.id.aboutbutton:
			
			break;
		case R.id.calibottle: 																		//DONE
			mChatService.write((new RequestCalibrateMessage()).toByte());
			break;
		case R.id.setweight: 																		//DONE
			viewFlipper.setDisplayedChild(viewFlipper.indexOfChild(findViewById(R.id.setweightmenu)));
			break;
		case R.id.weightbutton:	
			int weight = Integer.parseInt(SetWeightET.getText().toString());
			SetWeightMessage t = new SetWeightMessage(); t.setWeight(weight);
			mChatService.write(t.toByte());
			break;
		case R.id.history:
			//mChatService.write((new RequestEnrollMessage()).toByte());
			viewFlipper.setDisplayedChild(viewFlipper.indexOfChild(findViewById(R.id.historyscreen)));
			break;
		case R.id.gotoenroll: 																		//DONE
			mChatService.write((new RequestEnrollMessage(
					Integer.parseInt(getIDbox.getText().toString())
					)).toByte());
			viewFlipper.setDisplayedChild(viewFlipper.indexOfChild(findViewById(R.id.FPrint)));
			break;
		case R.id.ecfprint: 																		//DONE
			viewFlipper.setDisplayedChild(viewFlipper.indexOfChild(findViewById(R.id.preenroll)));
			break;
		case R.id.ret1:																				//DONE
			viewFlipper.setDisplayedChild(viewFlipper.indexOfChild(findViewById(R.id.MainMenu)));
			break;
		case R.id.ret2: 																			//DONE
			viewFlipper.setDisplayedChild(viewFlipper.indexOfChild(findViewById(R.id.MainMenu)));
			break;
		case R.id.disconnect:																		//DONE
			mChatService.stop();
			viewFlipper.setDisplayedChild(viewFlipper.indexOfChild(findViewById(R.id.SelectScreen)));
			break;
		case R.id.unlock:
			mChatService.write((new RequestUnlock()).toByte());
			break;
		default: 
			break;
		}
		
	}
}