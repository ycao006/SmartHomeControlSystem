#include <SoftwareSerial.h>
SoftwareSerial blunoserial(11,12);// RX TX 
String uart_cmd="";
char uart_letter;

int rightsensor = 6;
int leftsensor = 5;
int MotorPowerEN = 2; //
int Motorlogic1 = 4;  // 
int Motorlogic2 = 3; //
char heating_relay = 9;  //
char cooling_relay = 10;  //
int push1 = 0; // want to close the door
int push2 =0; //want to open the door
char relay1 = 7;
char relay2 = 8;
char checkright = 0;
char checkleft = 0;
int heating_sensor = A1;
int cooling_sensor = A0;
String comdata ="";  // data from serial terminal.
String LockState; // position of the main lock
int ac_heating_state = 0;
int ac_cooling_state = 0;
int ac_heating_reading = 0;
int ac_cooling_reading = 0;

//------------------------------------------------
char sentence[100];
String x = "I am Array\n";
String ReceivedCommand ="";  // data from serial terminal.

void cooling_heating_sensor(){
     ac_cooling_reading = 0;
     ac_heating_reading = 0;
     for(int i=0; i<20; i++){
       ac_heating_reading  += analogRead(heating_sensor);
       ac_cooling_reading  += analogRead(cooling_sensor);
     delay(5);
     }
       Serial.print("heating ac: ");
       Serial.println(ac_heating_reading);
       Serial.print(digitalRead(heating_relay));
       Serial.print("\n ");
       Serial.print("cooling ac: ");
       Serial.println(ac_cooling_reading);
       Serial.print(digitalRead(cooling_relay));
       Serial.print("\n ");
     // reading figure can be changed depending on the current sensor
     if(( ac_heating_reading > 50) && digitalRead(heating_relay)){
        ac_heating_state = 1; // 1 means on
     }
     else{
        ac_heating_state = 0; // 0 means off
     }
     if(( ac_cooling_reading > 50) && digitalRead(cooling_relay)){
        ac_cooling_state = 1;
     }
     else{
        ac_cooling_state = 0;
     }
}


//-------------------------------------RS232---start-------------------
void uart_bluno_state_R(){
    String state_list="";
    cooling_heating_sensor();
    if(uart_cmd == "Door State\n"){
      blunoserial.print(LockState);
    }
    if(uart_cmd == "AC heating State\n"){
      if(ac_heating_state == 0){
        blunoserial.print("OFF\n");
      }
      else{
        blunoserial.print("ON\n");
      }
      if( (ac_heating_state == 1) && (ac_cooling_state == 1) ){
        blunoserial.print("ERROR\n");
      }
    }
    if(uart_cmd == "AC cooling State\n"){
      if(ac_cooling_state == 0){
        blunoserial.print("OFF\n");
      }
      else{
        blunoserial.print("ON\n");
      }
      if( (ac_heating_state == 1) && (ac_cooling_state == 1) ){
        blunoserial.print("ERROR\n");
      }
    }
    if(uart_cmd == "ALL State\n"){
        
        state_list = state_list + LockState.substring(0,LockState.length()-1) + ",";
        if(ac_heating_state == 0){
          //blunoserial.print("OFF\n");
          state_list =  state_list + "OFF,";
        }
        else if(ac_heating_state == 1){
         // blunoserial.print("ON\n");
          state_list =  state_list + "ON,";
        }
        else if( (ac_heating_state == 1) && (ac_cooling_state == 1) ){          
           state_list =  state_list + "ERROR,";
          //blunoserial.print("ERROR\n");
        }
        if(ac_cooling_state == 0){
        //blunoserial.print("OFF\n");
        state_list =  state_list + "OFF";
        }
        else if(ac_cooling_state == 1){
        //blunoserial.print("ON\n");
        state_list =  state_list + "ON";
        }
        else if( (ac_heating_state == 1) && (ac_cooling_state == 1) ){
        //blunoserial.print("AC_ERROR\n");
        state_list =  state_list + "ERROR";
        }
        state_list = state_list + "\n";
        Serial.print("state_list");
        Serial.println(state_list);
        blunoserial.print(state_list);
        state_list="";
    }
  }
  
void uart_bluno_action_R(){
  while(blunoserial.available() > 0)  //serial.availabe()return the number of bits data in the buffer
  {
      uart_letter = blunoserial.read();
      uart_cmd = uart_cmd+ uart_letter;
  }
  Serial.println(uart_cmd);
  uart_bluno_state_R();
    if (uart_cmd == "START\n")
  {
    blunoserial.print("READY\n");
    digitalWrite(10,HIGH);
    delay(2000);
    digitalWrite(10,LOW);
    delay(2000);
  } 
    else if (uart_cmd == "Web_OpenDoor\n")
    {
      push2=1; 
      //Serial.print("open the main door\n");
    } 
    
    else if(uart_cmd == "Web_CloseDoor\n")
    {
      push1=1;    
      //Serial.print("close the main door\n");
     }    

    else if(uart_cmd == "AC_heatopen\n")
    {
      // cooling is on , heating is off now
      if((ac_cooling_state == 1) && (ac_heating_state == 0)){
        digitalWrite(cooling_relay,LOW);
        digitalWrite(heating_relay,HIGH);
      }
      // cooling is off; heating is on
      else if ( (ac_cooling_state == 0) && (ac_heating_state == 1) );
      // cooling is off; heating is off
      else if ( (ac_cooling_state == 0) && (ac_heating_state == 0) ){
        digitalWrite(heating_relay,HIGH);
      }
      // heating and cooling both are on error
      else{
        digitalWrite(cooling_relay,LOW);
        digitalWrite(heating_relay,LOW);
      } 
    }  
      
    else if(uart_cmd == "AC_heatclose\n")
    {
      
      digitalWrite(heating_relay,LOW);  
    }

    else if(uart_cmd == "AC_coolopen\n")
    {
      // cooling is on , heating is off now
       if((ac_cooling_state == 1) && (ac_heating_state == 0));
      // cooling is off; heating is on
      else if ( (ac_cooling_state == 0) && (ac_heating_state == 1) ){
        digitalWrite(heating_relay,LOW);
        digitalWrite(cooling_relay,HIGH);
      }
      // cooling is off; heating is off
      else if ( (ac_cooling_state == 0) && (ac_heating_state == 0) ){
        digitalWrite(cooling_relay,HIGH);
      }
      // heating and cooling both are on error
      else{
        digitalWrite(cooling_relay,LOW);
        digitalWrite(heating_relay,LOW);
      }
    }  
      
    else if(uart_cmd == "AC_coolclose\n")
    {
      digitalWrite(cooling_relay,LOW); 
    }

    
    uart_cmd="";
}

//-------------------------------------RS232---end---------------------

void SetMotorForward(){
  digitalWrite(Motorlogic1,HIGH);
  digitalWrite(Motorlogic2,LOW);
}
void SetMotorBackward(){
  digitalWrite(Motorlogic2,HIGH);
  digitalWrite(Motorlogic1,LOW);
}


void Lock_open(){
  if(push2==1)
   {
      checkright = digitalRead(rightsensor);
      checkleft = digitalRead(leftsensor);
      //Serial.println("hi");
      // Close door
      if(checkright == 1 && checkleft == 1)
      {
          digitalWrite(relay1, HIGH);  //enable relays
          digitalWrite(relay2, HIGH);
          SetMotorForward();
          digitalWrite(MotorPowerEN, HIGH);
          delay(50);
          while(checkright == 1 || checkleft == 1){
            checkright = digitalRead(rightsensor);
            checkleft = digitalRead(leftsensor);
          }
          delay(50);
          while(checkleft == 0){
            checkleft = digitalRead(leftsensor);
          }
          digitalWrite(MotorPowerEN, LOW);
          delay(150);
          checkright = digitalRead(rightsensor);
          checkleft = digitalRead(leftsensor);
          
          if(checkright == 0 && checkleft == 1)
          {   
              SetMotorBackward();
              digitalWrite(MotorPowerEN, HIGH);
              delay(100);
              while(checkleft == 1){
                checkleft = digitalRead(leftsensor);
              }
              delay(50);
              while(checkleft == 0){
                checkleft = digitalRead(leftsensor);
                //delay(50);
              }
              
              digitalWrite(MotorPowerEN, LOW);
              delay(150);
          }
      }
      //Serial.print("Door Opened");
      push2=0;
      LockState = "Open\n";
      //Serial.print("\n");
      Serial.print("ALREADY");
  }
}

void Lock_close(){
  if(push1==1)
  { 
      checkright = digitalRead(rightsensor);
      checkleft = digitalRead(leftsensor);
      //open door;
      if(checkright == 0 && checkleft == 1)
      {
          digitalWrite(relay1, HIGH);    //enable relays
          digitalWrite(relay2, HIGH);
          SetMotorBackward();
          digitalWrite(MotorPowerEN, HIGH);
          delay(50);
          while(checkright == 0){
            checkright = digitalRead(rightsensor);
          }
          while(checkleft == 0){
            checkleft = digitalRead(leftsensor);
          }
          delay(50);
          digitalWrite(MotorPowerEN, LOW);
          delay(150);
          
          checkright = digitalRead(rightsensor);
          checkleft = digitalRead(leftsensor);
          
          if(checkright == 1 && checkleft == 1)
          {   
              SetMotorForward();
              digitalWrite(MotorPowerEN, HIGH);
              delay(100); 
              while(checkleft == 1){
                checkleft = digitalRead(leftsensor);
              }
              delay(50);
              while(checkleft == 0){
                checkleft = digitalRead(leftsensor);
                //delay(50);
              }
              delay(50);
              digitalWrite(MotorPowerEN, LOW);
              delay(150);
          }
      }
    }
  //Serial.println("Door Closed");
  push1=0;
  LockState = "Close\n";
  
}

void setup(){
    //door&bluetooth lock part
    // Serial port at baud rate 9600
    Serial.begin(9600);
    blunoserial.begin(9600);
    
    
    //Pin Set up
    pinMode(MotorPowerEN, OUTPUT); //pin2
    pinMode(Motorlogic1, OUTPUT);  //pin 4 
    pinMode(Motorlogic2, OUTPUT);   //pin 3
    pinMode(relay1, OUTPUT);  //pin 7
    pinMode(relay2, OUTPUT);  //pin 8
    pinMode(heating_relay, OUTPUT);  //pin 9
    pinMode(cooling_relay, OUTPUT);  //pin 10
    pinMode(rightsensor, INPUT);  //pin6
    pinMode(leftsensor, INPUT);   //pin5
    pinMode(heating_sensor, INPUT);   //pin A)
    pinMode(cooling_sensor, INPUT);   //pin A1
    
    digitalWrite(MotorPowerEN, LOW); //disable Motor driver
    digitalWrite(relay1, LOW); //disable relays
    digitalWrite(relay2, LOW); 
}

void loop(){
  //bluetooth connection
    if(Serial.available()){
      while(Serial.available()>0){
        comdata +=char (Serial.read());
        delay(2);
      }
      if(comdata.length()>0)
      {
        Serial.println(comdata);
        if(comdata == "ooppeenn"){
          push2 = 1;}
        else if(comdata == "close"){
          push1 = 1;}
          
        comdata = "";
      }
    }
    // web_control 
    
    uart_bluno_action_R();

    
    digitalWrite(relay1, LOW); //disable main door relays
    digitalWrite(relay2, LOW); 

 //Control function of the Digital Lock
   if(push1==1) // want to close door
   {
      Lock_close();
   }
  else if(push2==1) // want to open the door
  { 
    Lock_open();
  }
  digitalWrite(relay1, LOW); //disable relays
  digitalWrite(relay2, LOW); 
  // check main door's state
  checkright = digitalRead(rightsensor);
  checkleft = digitalRead(leftsensor);
  if(checkright == 0 && checkleft == 1)
  {
    LockState = "Open\n";
  }
  else if(checkright == 1 && checkleft == 1 )
  {
    LockState = "Close\n";
  }
  else
  {
    LockState = "ERROR\n";
  }  
  //Serial.print("Current Position: "+ LockState);
  //Serial.print("\n");
}

