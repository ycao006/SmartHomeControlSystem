
//unsigned int maindoor_timer = 0;
//unsigned int garagedoor_timer = 0;

void garage_door_auto_lock(){
  if(garageDoor_flag == 1){
    if(garageMotion_reading > 300){
      garagedoor_timer = Timer5Counter + 300;
    }
    else {
      if(Timer5Counter >= garagedoor_timer){
          digitalWrite(garageDoor, LOW);
          delay(100);
          digitalWrite(garageDoor, HIGH);
          garagedoor_timer = Timer5Counter + 30;
       }
    } 
  }
}

void main_door_auto_lock(){
    if(maindoor_timer < Timer5Counter){
      Uart_request(x[3]);
      if(MainDoor_state=="Open"){
          if(maindoor_Autolock_flag == 1){
            DoorClose = 1;
            DoorOpen = 0;
            maindoor_Autolock_flag = 0;
          }
          else{
            maindoor_Autolock_flag = 1;
          }
      }
      else {
          maindoor_Autolock_flag = 0;
      }
      maindoor_timer = Timer5Counter + 150;
    }
}
