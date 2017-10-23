package com.example.blunobasicdemo;

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.TimeUnit;

import com.example.blunobasicdemo.R;

import android.os.Handler;
import android.os.IBinder;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.app.AlertDialog.Builder;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.bluetooth.BluetoothManager;
import android.content.BroadcastReceiver;
import android.content.ComponentName;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.TextView;
import android.widget.Toast;

public abstract  class BlunoLibrary  extends Activity{

	private Context mainContext=this;

	
	
//	public BlunoLibrary(Context theContext) {
//		
//		mainContext=theContext;
//	}
	public abstract void onConectionStateChange(connectionStateEnum theconnectionStateEnum);
	public abstract void onSerialReceived(String theString);
	public abstract void RSSIreceive(String theString);
	public void serialSend(String theString){
		if (mConnectionState == connectionStateEnum.isConnected) {
			mSCharacteristic.setValue(theString);
			mBluetoothLeService.writeCharacteristic(mSCharacteristic);
		}
	}
	
	private int mBaudrate=9600;	//set the default baud rate to 115200
	private String mPassword="AT+PASSWOR=DFRobot\r\n";
	
	BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
	private String mBaudrateBuffer = "AT+CURRUART="+mBaudrate+"\r\n";
	
//	byte[] mBaudrateBuffer={0x32,0x00,(byte) (mBaudrate & 0xFF),(byte) ((mBaudrate>>8) & 0xFF),(byte) ((mBaudrate>>16) & 0xFF),0x00};;
	
	
	int[] iarray= new int[]{99,99,99,99,99};
	
	
	public void serialBegin(int baud){
		mBaudrate=baud;
		mBaudrateBuffer = "AT+CURRUART="+mBaudrate+"\r\n";
	}
	
	
	static class ViewHolder {
		TextView deviceName;
		TextView deviceAddress;
		TextView deviceRssi;
	}
    private static BluetoothGattCharacteristic mSCharacteristic, mModelNumberCharacteristic, mSerialPortCharacteristic, mCommandCharacteristic;
    BluetoothLeService mBluetoothLeService;
    private ArrayList<ArrayList<BluetoothGattCharacteristic>> mGattCharacteristics =
            new ArrayList<ArrayList<BluetoothGattCharacteristic>>();
	private LeDeviceListAdapter mLeDeviceListAdapter=null;
	private BluetoothAdapter mBluetoothAdapter;
	private boolean mScanning =false;
	Builder mScanDeviceDialog;
    private String mDeviceName;
    private String mDeviceAddress;
    private String RSSI;
    public static String RSSII;
    public String mRssi="no connection now";
    public int  rssi1=0;
   // public int rssi2;
	public enum connectionStateEnum{isNull, isScanning, isToScan, isConnecting , isConnected, isDisconnecting};
	public connectionStateEnum mConnectionState = connectionStateEnum.isNull;
	private static final int REQUEST_ENABLE_BT = 1;

	private Handler mHandler= new Handler();
	
	public boolean mConnected = false;

    private final static String TAG = BlunoLibrary.class.getSimpleName();

    private Runnable mConnectingOverTimeRunnable=new Runnable(){

		@Override
		public void run() {
        	if(mConnectionState==connectionStateEnum.isConnecting)
			mConnectionState=connectionStateEnum.isToScan;
			onConectionStateChange(mConnectionState);
			mBluetoothLeService.close();
		}};
		
    private Runnable mDisonnectingOverTimeRunnable=new Runnable(){

		@Override
		public void run() {
        	if(mConnectionState==connectionStateEnum.isDisconnecting)
			mConnectionState=connectionStateEnum.isToScan;
			onConectionStateChange(mConnectionState);
			mBluetoothLeService.close();
		}};
    
	public static final String SerialPortUUID="0000dfb1-0000-1000-8000-00805f9b34fb";
	public static final String CommandUUID="0000dfb2-0000-1000-8000-00805f9b34fb";
    public static final String ModelNumberStringUUID="00002a24-0000-1000-8000-00805f9b34fb";
	
    public void onCreateProcess()
    {
    	if(!initiate())
		{
			Toast.makeText(mainContext, R.string.error_bluetooth_not_supported,
					Toast.LENGTH_SHORT).show();
			((Activity) mainContext).finish();
		}
		
        Intent gattServiceIntent = new Intent(mainContext, BluetoothLeService.class);
        mainContext.bindService(gattServiceIntent, mServiceConnection, Context.BIND_AUTO_CREATE);
		mLeDeviceListAdapter = new LeDeviceListAdapter();
        mScanDeviceDialog = new AlertDialog.Builder(mainContext).setTitle("open the lock now...");//必须增加dialog scan需要   否则 scan 按下去崩溃
      //  AlertDialog dialog = mScanDeviceDialog.show();  //多一个dialog
     //    if () { dialog.dismiss();}
      //  if (mBluetoothLeService.connect(mDeviceAddress)
    //    		{ dialog.dismiss();} //  问题在SCAN进行下去之后 这个Dialog不会消失
        /*   dialog 有很多adapter 点击选择一个listener
		// Initializes list view adapter.
		mLeDeviceListAdapter = new LeDeviceListAdapter();
		
		
		// Initializes and show the scan Device Dialog
		mScanDeviceDialog = new AlertDialog.Builder(mainContext)
		.setTitle("BLE Device Scan...")
		.setAdapter(mLeDeviceListAdapter, new DialogInterface.OnClickListener() {
		//	.setTitle("BLE Device Scan...").setAdapter(mLeDeviceListAdapter, new DialogInterface {

			@Override
			public void onClick(DialogInterface dialog, int which)
		
			{
				//final BluetoothDevice device = mLeDeviceListAdapter.getDevice(which);
		
		
				final BluetoothDevice device = adapter.getRemoteDevice("D0:39:72:C5:35:2E");   //点击然后选择设备  固定为BLUNO
				if (device == null)
					return;
				scanLeDevice(false);
				System.out.println("onListItemClick " + device.getName().toString());
				
				System.out.println("Device Name:"+device.getName() + "   " + "Device Name:" + device.getAddress());
				
				mDeviceName=device.getName().toString();
				mDeviceAddress=device.getAddress().toString();
				
		        if(mDeviceName.equals("No Device Available") && mDeviceAddress.equals("No Address Available"))
		        {
		        	mConnectionState=connectionStateEnum.isToScan;
		        	onConectionStateChange(mConnectionState);
		        }
		        else{
		        	if (mBluetoothLeService.connect(mDeviceAddress)) {
				        Log.d(TAG, "Connect request success");
			        	mConnectionState=connectionStateEnum.isConnecting;
			        	onConectionStateChange(mConnectionState);
			            mHandler.postDelayed(mConnectingOverTimeRunnable, 10000);
		        	}
			        else {
				        Log.d(TAG, "Connect request fail");
			        	mConnectionState=connectionStateEnum.isToScan;
			        	onConectionStateChange(mConnectionState);
					}
		        }
			}
		})          *//*
		*.setOnCancelListener(new DialogInterface.OnCancelListener(){

			@Override
			public void onCancel(DialogInterface arg0) {
				System.out.println("mBluetoothAdapter.stopLeScan");

	        	mConnectionState=connectionStateEnum.isToScan;
	        	onConectionStateChange(mConnectionState);
				mScanDeviceDialog.dismiss();

				scanLeDevice(false);
			}
		}).create();*/
		
    }
    
    
    
    public void onResumeProcess() {
    	System.out.println("BlUNOActivity onResume");
		// Ensures Bluetooth is enabled on the device. If Bluetooth is not
		// currently enabled,
		// fire an intent to display a dialog asking the user to grant
		// permission to enable it.
		if (!mBluetoothAdapter.isEnabled()) {
			if (!mBluetoothAdapter.isEnabled()) {
				//Intent enableBtIntent = new Intent(
					//	BluetoothAdapter.ACTION_REQUEST_ENABLE);
				//((Activity) mainContext).startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);//系统询问打开蓝牙
				mBluetoothAdapter.enable();//自动打开蓝牙 不需要询问
			}
		}
		
			   mainContext.registerReceiver(mGattUpdateReceiver, makeGattUpdateIntentFilter());

	   
	}
    

    public void onPauseProcess() {
    	System.out.println("BLUNOActivity onPause");
		scanLeDevice(false);
		mainContext.unregisterReceiver(mGattUpdateReceiver);
		mLeDeviceListAdapter.clear();
    	mConnectionState=connectionStateEnum.isToScan;
    	onConectionStateChange(mConnectionState);
		//mScanDeviceDialog.dismiss();
		if(mBluetoothLeService!=null)
		{
			
			mBluetoothLeService.disconnect();
            mHandler.postDelayed(mDisonnectingOverTimeRunnable, 10000);

//			mBluetoothLeService.close();
		}
		mSCharacteristic=null;

	}

	
	public void onStopProcess() {
		System.out.println("MiUnoActivity onStop");
		if(mBluetoothLeService!=null)
		{
//			mBluetoothLeService.disconnect();
//            mHandler.postDelayed(mDisonnectingOverTimeRunnable, 10000);
        	mHandler.removeCallbacks(mDisonnectingOverTimeRunnable);
			mBluetoothLeService.close();
		}
		mSCharacteristic=null;
	}

	public void onDestroyProcess() {
        mainContext.unbindService(mServiceConnection);
        mBluetoothLeService = null;
	}
	
	public void onActivityResultProcess(int requestCode, int resultCode, Intent data) {
		// User chose not to enable Bluetooth.
		if (requestCode == REQUEST_ENABLE_BT
				&& resultCode == Activity.RESULT_CANCELED) {
			((Activity) mainContext).finish();
			return;
		}
	}

	boolean initiate()
	{
		// Use this check to determine whether BLE is supported on the device.
		// Then you can
		// selectively disable BLE-related features.
		if (!mainContext.getPackageManager().hasSystemFeature(
				PackageManager.FEATURE_BLUETOOTH_LE)) {
			return false;
		}
		
		// Initializes a Bluetooth adapter. For API level 18 and above, get a
		// reference to
		// BluetoothAdapter through BluetoothManager.
		final BluetoothManager bluetoothManager = (BluetoothManager) mainContext.getSystemService(Context.BLUETOOTH_SERVICE);
		mBluetoothAdapter = bluetoothManager.getAdapter();
	
		// Checks if Bluetooth is supported on the device.
		if (mBluetoothAdapter == null) {
			return false;
		}
		return true;
	}
	
	 // Handles various events fired by the Service.
    // ACTION_GATT_CONNECTED: connected to a GATT server.
    // ACTION_GATT_DISCONNECTED: disconnected from a GATT server.
    // ACTION_GATT_SERVICES_DISCOVERED: discovered GATT services.
    // ACTION_DATA_AVAILABLE: received data from the device.  This can be a result of read
    //                        or notification operations.
    private final BroadcastReceiver mGattUpdateReceiver = new BroadcastReceiver() {
        @SuppressLint("DefaultLocale")
		@Override
        public void onReceive(Context context, Intent intent) {
        	final String action = intent.getAction();
            System.out.println("mGattUpdateReceiver->onReceive->action="+action);
            if (BluetoothLeService.ACTION_GATT_CONNECTED.equals(action)) {
                mConnected = true;
            	mHandler.removeCallbacks(mConnectingOverTimeRunnable);
        		System.out.println("successqqq");

            } else if (BluetoothLeService.ACTION_GATT_DISCONNECTED.equals(action)) {
                mConnected = false;
                mConnectionState = connectionStateEnum.isToScan;
                onConectionStateChange(mConnectionState);
            	mHandler.removeCallbacks(mDisonnectingOverTimeRunnable);
            	mBluetoothLeService.close();
            } else if (BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED.equals(action)) {
                // Show all the supported services and characteristics on the user interface.
            	for (BluetoothGattService gattService : mBluetoothLeService.getSupportedGattServices()) {
            		System.out.println("ACTION_GATT_SERVICES_DISCOVERED  "+
            				gattService.getUuid().toString());
            	}
            	getGattServices(mBluetoothLeService.getSupportedGattServices());
            } else if(BluetoothLeService.ACTION_RSSI_VALUE_READ.equals(action)){
            	mRssi = intent.getStringExtra(RSSII);	
            	RSSIreceive(mRssi);
            	//char chs[]=mRssi.toCharArray();
            	
            	int i = Integer.valueOf(mRssi).intValue();
            	int  average;
            	
            	for(int j=4;j>0;j--){
            		iarray[j]=iarray[j-1];
            	}
            	iarray[0]=i;
            	average=Math.abs(((iarray[0]+iarray[1]+iarray[2]+iarray[3]+iarray[4])/5));
            	if(average<50){
					
                    	serialSend("ooppeenn");				//send the data to the BLUNO自动发送CODE
                    	//serialSend(String.valueOf(average));
                    	System.out.println(mRssi);
            	}
            	            	//if(chs[1]<'5'&&chs[1]>'0'){
            }else if (BluetoothLeService.ACTION_DATA_AVAILABLE.equals(action)) {
            	if(mSCharacteristic==mModelNumberCharacteristic)
            	{
            		if (intent.getStringExtra(BluetoothLeService.EXTRA_DATA).toUpperCase().startsWith("DF BLUNO")) {
						mBluetoothLeService.setCharacteristicNotification(mSCharacteristic, false);
						mSCharacteristic=mCommandCharacteristic;
						mSCharacteristic.setValue(mPassword);
						mBluetoothLeService.writeCharacteristic(mSCharacteristic);
						mSCharacteristic.setValue(mBaudrateBuffer);
						mBluetoothLeService.writeCharacteristic(mSCharacteristic);
						mSCharacteristic=mSerialPortCharacteristic;
						mBluetoothLeService.setCharacteristicNotification(mSCharacteristic, true);
						mConnectionState = connectionStateEnum.isConnected;
						onConectionStateChange(mConnectionState);
				    	//Intent intent = getIntent(); 
				        //String num1 = intent.getStringExtra("one");    
				        //int ret = Integer.parseInt(num1); 
						
					}
            		else {
            			Toast.makeText(mainContext, "Please select DFRobot devices111",Toast.LENGTH_SHORT).show();
                        mConnectionState = connectionStateEnum.isToScan;
                        onConectionStateChange(mConnectionState);
                    	System.out.println("111111111");
					}
            	}
            	else if (mSCharacteristic==mSerialPortCharacteristic) {
            		onSerialReceived(intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
				}
            	
            
            	System.out.println("displayData "+intent.getStringExtra(BluetoothLeService.EXTRA_DATA));
				
            
            //	serialSend("open1994".toString());				//send the data to the BLUNO自动发送CODE
//            	mPlainProtocol.mReceivedframe.append(intent.getStringExtra(BluetoothLeService.EXTRA_DATA)) ;
//            	System.out.print("mPlainProtocol.mReceivedframe:");
//            	System.out.println(mPlainProtocol.mReceivedframe.toString());
            	
            }
        }
    };
	
    void buttonScanOnClickProcess()
    {
    	switch (mConnectionState) {
		case isNull:
			mConnectionState=connectionStateEnum.isScanning;
			onConectionStateChange(mConnectionState);
			scanLeDevice(true);
			mScanDeviceDialog.show();
			break;
		case isToScan:
			mConnectionState=connectionStateEnum.isScanning;
			onConectionStateChange(mConnectionState);
			scanLeDevice(true);
			mScanDeviceDialog.show();
			break;
		case isScanning:
			
			break;

		case isConnecting:
			
			break;
		case isConnected:
			mBluetoothLeService.disconnect();
            mHandler.postDelayed(mDisonnectingOverTimeRunnable, 10000);

//			mBluetoothLeService.close();
			mConnectionState=connectionStateEnum.isDisconnecting;
			onConectionStateChange(mConnectionState);
			break;
		case isDisconnecting:
			
			break;

		default:
			break;
		}
    	
    	
    }
    //寻找设备 http://www.blogjava.net/zh-weir/archive/2013/12/09/407373.html
	void scanLeDevice(final boolean enable) {
		if (enable) {
			// Stops scanning after a pre-defined scan period.

			System.out.println("mBluetoothAdapter.startLeScan");
			
			if(mLeDeviceListAdapter != null)
			{
				mLeDeviceListAdapter.clear();
				mLeDeviceListAdapter.notifyDataSetChanged();
			}
			
			if(!mScanning)
			{
				mScanning = true;
				mBluetoothAdapter.startLeScan(mLeScanCallback);
				//mBluetoothAdapter.startLeScan(UUID["D0:39:72:C5:35:2E"],BluetoothAdapter.LeScanCallback);

			}
		} else {
			if(mScanning)
			{
				mScanning = false;
				mBluetoothAdapter.stopLeScan(mLeScanCallback);
			}
		}
	}
	
	// Code to manage Service lifecycle.
    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder service) {
            System.out.println("mServiceConnection onServiceConnected");
        	mBluetoothLeService = ((BluetoothLeService.LocalBinder) service).getService();
            if (!mBluetoothLeService.initialize()) {
                Log.e(TAG, "Unable to initialize Bluetooth");
                ((Activity) mainContext).finish();
            }
            
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
        	System.out.println("mServiceConnection onServiceDisconnected");
            mBluetoothLeService = null;
        }
    };
	
	// Device scan callback.
	private BluetoothAdapter.LeScanCallback mLeScanCallback = new BluetoothAdapter.LeScanCallback() {

		@Override
		public void onLeScan(final BluetoothDevice device, final int rssi,
				byte[] scanRecord) {
			((Activity) mainContext).runOnUiThread(new Runnable() {
				@SuppressWarnings("static-access")
				@Override
				public void run() {
					System.out.println("mLeScanCallback onLeScan run ");
					
				/*	try {
						TimeUnit.SECONDS.sleep(1);
					} catch (InterruptedException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
					}*/
					
					
					final BluetoothDevice device = adapter.getRemoteDevice("D0:39:72:C5:35:2E");   //选择设备  固定为BLUNO的UUID
					//final BluetoothDevice device = adapter.getRemoteDevice("C4:BE:84:15:95:DD");   //选择设备  固定为BLUNO的UUID

					mDeviceName=device.getName().toString();//获取到设备名字地址 下面改变状态
					mDeviceAddress=device.getAddress().toString();
					while(mDeviceAddress==null){
						try {
							TimeUnit.MICROSECONDS.sleep(100);
						} catch (InterruptedException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}
					  if(mDeviceName.equals("No Device Available") && mDeviceAddress.equals("No Address Available"))
				        {
				        	mConnectionState=connectionStateEnum.isToScan;
				        	onConectionStateChange(mConnectionState);
				        }
				        else{
				        	if (mBluetoothLeService.connect(mDeviceAddress)) {
						        Log.d(TAG, "Connect request success");
					        	mConnectionState=connectionStateEnum.isConnecting;
					        	onConectionStateChange(mConnectionState);
					            mHandler.postDelayed(mConnectingOverTimeRunnable, 10000);
//					            AlertDialog Dialog;
//					            Dialog= mScanDeviceDialog.show();
//					            Dialog.dismiss(); //  问题在SCAN进行下去之后 这个Dialog不会消失
					            

				        	}
					        else {
						        Log.d(TAG, "Connect request fail");
					        	mConnectionState=connectionStateEnum.isToScan;
					        	onConectionStateChange(mConnectionState);
							}}	

					mLeDeviceListAdapter.addDevice(device);
					mLeDeviceListAdapter.notifyDataSetChanged();
					RSSI=Integer.toString(rssi);
					System.out.println("comeon "); 
				}
			});
		}
	};
	
    private void getGattServices(List<BluetoothGattService> gattServices) {
        if (gattServices == null) return;
        String uuid = null;
        mModelNumberCharacteristic=null;
        mSerialPortCharacteristic=null;
        mCommandCharacteristic=null;
        mGattCharacteristics = new ArrayList<ArrayList<BluetoothGattCharacteristic>>();
        // Loops through available GATT Services.
        for (BluetoothGattService gattService : gattServices) {
            uuid = gattService.getUuid().toString();
            System.out.println("displayGattServices + uuid="+uuid);
            
            List<BluetoothGattCharacteristic> gattCharacteristics =
                    gattService.getCharacteristics();
            ArrayList<BluetoothGattCharacteristic> charas =
                    new ArrayList<BluetoothGattCharacteristic>();

            // Loops through available Characteristics.
            for (BluetoothGattCharacteristic gattCharacteristic : gattCharacteristics) {
                charas.add(gattCharacteristic);
                uuid = gattCharacteristic.getUuid().toString();
                if(uuid.equals(ModelNumberStringUUID)){
                	mModelNumberCharacteristic=gattCharacteristic;
                	System.out.println("mModelNumberCharacteristic  "+mModelNumberCharacteristic.getUuid().toString());
                }
                else if(uuid.equals(SerialPortUUID)){
                	mSerialPortCharacteristic = gattCharacteristic;
                	System.out.println("mSerialPortCharacteristic  "+mSerialPortCharacteristic.getUuid().toString());
//                    updateConnectionState(R.string.comm_establish);
                }
                else if(uuid.equals(CommandUUID)){
                	mCommandCharacteristic = gattCharacteristic;
                	System.out.println("mSerialPortCharacteristic  "+mSerialPortCharacteristic.getUuid().toString());
//                    updateConnectionState(R.string.comm_establish);
                }
            }
            mGattCharacteristics.add(charas);
        }
        
        if (mModelNumberCharacteristic==null || mSerialPortCharacteristic==null || mCommandCharacteristic==null) {
		//	Toast.makeText(mainContext, "Please select DFRobot devices222",Toast.LENGTH_SHORT).show();
            mConnectionState = connectionStateEnum.isToScan;
            onConectionStateChange(mConnectionState);
            System.out.println("222222222221");
		}
        else {
        	mSCharacteristic=mModelNumberCharacteristic;
        	mBluetoothLeService.setCharacteristicNotification(mSCharacteristic, true);
        	mBluetoothLeService.readCharacteristic(mSCharacteristic);
		}
        
    }
    
    private static IntentFilter makeGattUpdateIntentFilter() {
        final IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_CONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_DISCONNECTED);
        intentFilter.addAction(BluetoothLeService.ACTION_GATT_SERVICES_DISCOVERED);
        intentFilter.addAction(BluetoothLeService.ACTION_DATA_AVAILABLE);
        intentFilter.addAction(BluetoothLeService.ACTION_RSSI_VALUE_READ); 
        return intentFilter;
    }
	
	private class LeDeviceListAdapter extends BaseAdapter {
		private ArrayList<BluetoothDevice> mLeDevices;
		private LayoutInflater mInflator;

		public LeDeviceListAdapter() {
			super();
			mLeDevices = new ArrayList<BluetoothDevice>();
			mInflator =  ((Activity) mainContext).getLayoutInflater();
		}

		public void addDevice(BluetoothDevice device) {
			if (!mLeDevices.contains(device)) {
				mLeDevices.add(device);
			}
		}

		public BluetoothDevice getDevice(int position) {
			return mLeDevices.get(position);
		}

		public void clear() {
			mLeDevices.clear();
		}

		@Override
		public int getCount() {
			return mLeDevices.size();
		}

		@Override
		public Object getItem(int i) {
			return mLeDevices.get(i);
		}

		@Override
		public long getItemId(int i) {
			return i;
		}

		@Override
		public View getView(int i, View view, ViewGroup viewGroup) {
			ViewHolder viewHolder;
			// General ListView optimization code.
			if (view == null) {
				view = mInflator.inflate(R.layout.listitem_device, null);
				viewHolder = new ViewHolder();
				viewHolder.deviceAddress = (TextView) view
						.findViewById(R.id.device_address);
				viewHolder.deviceName = (TextView) view
						.findViewById(R.id.device_name);
				viewHolder.deviceRssi = (TextView) view.findViewById(R.id.device_rssi);
						
				System.out.println("mInflator.inflate  getView");
				view.setTag(viewHolder);
			} else {
			viewHolder = (ViewHolder) view.getTag();
			}

			BluetoothDevice device = mLeDevices.get(i);
			final String deviceName = device.getName();
			final String devicerssi = RSSI;
			if (deviceName != null && deviceName.length() > 0)
		       	viewHolder.deviceName.setText(deviceName);
			else
				viewHolder.deviceName.setText(R.string.unknown_device);	
			    viewHolder.deviceAddress.setText(device.getAddress());
			    viewHolder.deviceRssi.setText(devicerssi);
			return view;
		}
	}

}
	
	
	



