package com.example.blunobasicdemo;

import android.os.Bundle;
import android.content.DialogInterface;
import android.content.Intent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
public class MainActivity  extends BlunoLibrary {
	private Button buttonScan;
	private Button buttonSerialSend;
	private EditText serialSendText;
	private TextView serialReceivedText;
	private TextView rssi;
	private Button buttonopen;
	private Button buttonclose;
	@Override
	
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		setContentView(R.layout.activity_main);
        onCreateProcess();														//onCreate Process by BlunoLibrary
        
        serialBegin(9600);													//set the Uart Baudrate on BLE chip to 9600
  
        
        serialReceivedText=(TextView) findViewById(R.id.serialReveicedText);	//initial the EditText of the received data
        serialSendText=(EditText) findViewById(R.id.serialSendText);			//initial the EditText of the sending data
        rssi=(TextView) findViewById(R.id.textrssi);
        rssi.setText("rssi:"+RSSII);
        buttonSerialSend = (Button) findViewById(R.id.buttonSerialSend);		//initial the button for sending the data
        buttonSerialSend.setOnClickListener(new OnClickListener() {	
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
				
				
				serialSend(serialSendText.getText().toString());				//send the data to the BLUNO
			}
		});
        
        
        buttonopen = (Button) findViewById(R.id.buttonopen);
        buttonopen.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
			    
			   // getRssiVal();
			   // serialSend(Boolean.toString(getRssiVal()));
			    //serialSend(Integer.toString(rssi));
				serialSend("open".toString());				//send the data to the BLUNO
			}
		});
        
        
        buttonclose = (Button) findViewById(R.id.buttonclose);
        buttonclose.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {

		        
		        
				serialSend("close".toString());				//send the data to the BLUNO
				
			}
		});
        buttonScan = (Button) findViewById(R.id.buttonScan);					//initial the button for scanning the BLE device
        buttonScan.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub

				buttonScanOnClickProcess();										//Alert Dialog for selecting the BLE device
			}
		});
        
	}

	protected void onResume(){
		buttonScanOnClickProcess();	
		super.onResume();
		System.out.println("BlUNOActivity onResume");
		onResumeProcess();//onResume Process by BlunoLibrary
	}
	
	
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		onActivityResultProcess(requestCode, resultCode, data);					//onActivityResult Process by BlunoLibrary
		super.onActivityResult(requestCode, resultCode, data);
		
	}
	
    @Override
    protected void onPause() {
        super.onPause();
        onPauseProcess();
        //onPause Process by BlunoLibrary
        
    }
	
	protected void onStop() {
		super.onStop();
		onStopProcess();														//onStop Process by BlunoLibrary
	}
    
	@Override
    protected void onDestroy() {
        super.onDestroy();	
        onDestroyProcess();														//onDestroy Process by BlunoLibrary
    }

	@Override
	public void onConectionStateChange(connectionStateEnum theConnectionState) {//Once connection state changes, this function will be called
		switch (theConnectionState) {											//Four connection state
		case isConnected:
			buttonScan.setText("Connected");
			break;
		case isConnecting:
			buttonScan.setText("Connecting");
			break;
		case isToScan:
			buttonScan.setText("Scan");
			break;
		case isScanning:
			buttonScan.setText("Scanning");
			break;
		case isDisconnecting:
			buttonScan.setText("isDisconnecting");
			break;
		default:
			break;
		}
	}

	@Override
	public void onSerialReceived(String theString) {							//Once connection data received, this function will be called
		// TODO Auto-generated method stub
		serialReceivedText.append(theString);							//append the text into the EditText
		//The Serial data from the BLUNO may be sub-packaged, so using a buffer to hold the String is a good choice.
       if(theString.startsWith("ALREADY")){
        	mBluetoothLeService.disconnect();
        }			
	}
	public void RSSIreceive(String theString) {							//Once connection data received, this function will be called
		// TODO Auto-generated method stu							//append the text into the EditText
		//The Serial data from the BLUNO may be sub-packaged, so using a buffer to hold the String is a good choice.
		rssi.setText("rssi:"+theString);
	}
	

}