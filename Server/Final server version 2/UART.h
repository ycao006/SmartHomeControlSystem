
char uart_char;
String uart_string,uart_string2;
int Uart_ready_flag = 0;
int Uart_recieved_flag = 0;
String Message;

void Uart_init(){
  Serial1.print("START\n");
        delay(100);
        while(Serial1.available()){
          uart_char = Serial1.read();
          if(uart_char == '\n'){
            if(uart_string == "READY"){
              Uart_ready_flag = 1;
            }
            uart_string = "";
          }
          else{
            uart_string += uart_char;
          }
        }
}

void Uart_end(){
  Uart_ready_flag = 0;
}

void Uart_send(String Message){
  if(Uart_ready_flag == 1){
      delay(100);
      Serial1.print(Message); 
      delay(100);  
      Uart_end();
  }
}

//-------------------------------------------------------------------------
int Bluno_state;
String MainDoor_state;
String AC_Heating_state;
String AC_Cooling_state;
int request_Flag=0;
int x[] = {1,2,3,4};
int index;

void Uart_request(int Bluno_state){
  // Send 1:Main Door State
  // send 2 :AC Heating State
  //send 3 :AC cooling State
  Uart_ready_flag = 1;
  String request_Message = "";
  if(Bluno_state == 1){
    //request Main Door State
    //Uart_init();
    //Uart_ready_flag = 1;
    request_Message = "Door State\n";
    Uart_send("Door State\n");
        while(Serial1.available()){
          uart_char = Serial1.read();
          if(uart_char == '\n'){
            if(uart_string2 == "Open" || uart_string2 == "Close"){
              MainDoor_state = uart_string2;
            }
            else if(uart_string2 == "ERROR"){
              MainDoor_state = "Door ERROR";
            }
            else{
              MainDoor_state = "Recieving ERROR";
            }
            uart_string2 = "";
            //delay(100);
            //Serial.print("Maindoor State: ");
            //Serial.println(MainDoor_state);
          }
          else{
            uart_string2 += uart_char;
          }
        }
  }
  else if(Bluno_state ==2){
    //request AC Heating State
    //Uart_init();
    request_Message = "AC heating State\n";
    Uart_send(request_Message);
      while(Serial1.available()){
          uart_char = Serial1.read();
          if(uart_char == '\n'){
            if(uart_string2 == "ON" || uart_string2 == "OFF"){
              AC_Heating_state = uart_string2;
            }
            else if(uart_string2 == "AC_ERROR" ){
              AC_Heating_state = "Hardware ERROR";
            }
            else{
              AC_Heating_state = "Recieving ERROR";
            }
            uart_string2 = "";
          }
          else{
            uart_string2 += uart_char;
          }
        }
  }
  else if(Bluno_state ==3){
    //request AC cooling State
    //Uart_init();
    request_Message = "AC cooling State\n";
    Uart_send(request_Message);
      while(Serial1.available()){
          uart_char = Serial1.read();
          if(uart_char == '\n'){
            if(uart_string2 == "ON"|| uart_string2 == "OFF"){
              AC_Cooling_state = uart_string2;
            }
            else if(uart_string2 == "AC_ERROR" ){
              AC_Cooling_state = "AC ERROR";
            }
            else{
              AC_Cooling_state = "Recieving ERROR";
            }
            uart_string2 = "";
          }
          else{
            uart_string2 += uart_char;
          }
        }
  }
  else if(Bluno_state ==4){
    request_Message = "ALL State\n";
    Uart_send(request_Message);
    while(Serial1.available()){
          uart_char = Serial1.read();
          if(uart_char == '\n'){
            //Serial.println(uart_string2);
            index = uart_string2.indexOf(",");
            MainDoor_state = uart_string2.substring(0,index);
            uart_string2 = uart_string2.substring(index+1, uart_string2.length());
            index = uart_string2.indexOf(",");
            AC_Heating_state = uart_string2.substring(0,index);
            AC_Cooling_state = uart_string2.substring(index+1, uart_string2.length());
            uart_string2 = "";
          }
          else{
            uart_string2 += uart_char;
          }
        }
  }
  
}
//---------------------------------------------------------
void AC_Control(){
  String AC_Message = "";
  EEPROM.update(AC_CoolDown_flag_address,AC_CoolDown_flag);
  //EEPROM.update(AC_CoolDown_counter_address,AC_CoolDown);
  EEPROM.update(AC_heating_relay_address,AC_heating_relay);
  EEPROM.update(AC_cooling_relay_address,AC_cooling_relay);

  if(AC_cooling_relay == 1 && AC_heating_relay == 0){
    if(AC_Cooling_state == "ON" && AC_Heating_state == "OFF");
    else if(AC_Cooling_state == "OFF" && AC_Heating_state == "ON")
    {
      Uart_ready_flag = 1;
      AC_Message = "AC_heatclose\n";
      Uart_send(AC_Message);
      Serial.println(AC_Message);
      AC_CoolDown = Timer5Counter + 600;
      AC_CoolDown_flag =1;
    }

    if(AC_Cooling_state == "OFF" && AC_Heating_state == "OFF" && AC_CoolDown_flag ==1){
      if(AC_CoolDown <= Timer5Counter){
        AC_CoolDown_flag =0;
      }
    }
    else if(AC_Cooling_state == "OFF" && AC_Heating_state == "OFF" && AC_CoolDown_flag ==0){
      Uart_ready_flag = 1;
      AC_Message = "AC_coolopen\n";
      Uart_send(AC_Message);
      Serial.println(AC_Message);
    }
  }
  else if(AC_heating_relay == 1 && AC_cooling_relay == 0){
    if(AC_Cooling_state == "OFF" && AC_Heating_state == "ON");
    else if(AC_Cooling_state == "ON" && AC_Heating_state == "OFF")
    {
      Uart_ready_flag = 1;
      AC_Message = "AC_coolclose\n";
      Uart_send(AC_Message);
      Serial.println(AC_Message);
      AC_CoolDown = Timer5Counter + 600;
      AC_CoolDown_flag =1;
    }
    if(AC_Cooling_state == "OFF" && AC_Heating_state == "OFF" && AC_CoolDown_flag ==1){
      if(AC_CoolDown <= Timer5Counter){
        AC_CoolDown_flag =0;
      }
    }
    else if(AC_Cooling_state == "OFF" && AC_Heating_state == "OFF" && AC_CoolDown_flag ==0){
      Uart_ready_flag = 1;
      AC_Message = "AC_heatopen\n";
      Uart_send(AC_Message);
      Serial.println(AC_Message);
    }
  }
  else if(AC_heating_relay == 0 && AC_cooling_relay == 0){
    if(AC_Cooling_state == "ON"){
      Uart_ready_flag = 1;
      AC_Message = "AC_coolclose\n";
      Uart_send(AC_Message);
      Serial.println(AC_Message);
      AC_CoolDown = Timer5Counter + 600;
      AC_CoolDown_flag =1;
    }
    if(AC_Heating_state == "ON"){
      Uart_ready_flag = 1;
      AC_Message = "AC_heatclose\n";
      Uart_send(AC_Message);
      Serial.println(AC_Message);
      AC_CoolDown = Timer5Counter + 600;
      AC_CoolDown_flag =1;
    }
    if(Timer5Counter > AC_CoolDown){
      AC_CoolDown_flag =0;
    }
  }
  else if(AC_heating_relay == 1 && AC_cooling_relay == 1){
    AC_heating_relay = 0;
    AC_cooling_relay = 0;
  }
    
}

//---------------------------------------------------------------------
void Maindoor_Control(){
    //---- Main Door Control------------------------
  if(DoorClose == 1)
   {
        //Uart_init();
        Uart_ready_flag = 1;
        Message = "Web_CloseDoor\n";
        Uart_send(Message);
        //Lock_close();
        DoorClose = 0;
  }
  else if(DoorOpen == 1)
  { 
      //Lock_open();
     // Uart_init();
      Uart_ready_flag = 1;
      Message = "Web_OpenDoor\n";
      Uart_send(Message); 
      DoorOpen = 0;
  } 
}

