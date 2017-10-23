
//unsigned int maindoor_timer = 0;
//unsigned int garagedoor_timer = 0;
/*
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
*/
char garage_door_flag_counter = 0;
void garage_door_auto_lock(){
  if(garageDoor_flag == 1){
    garage_door_flag_counter = 0;
    if(garage_door_ready_flag == 0){
      garage_door_ready_timer = Timer5Counter + 60;
      garage_door_ready_flag = 1;
    }
    else if(garage_door_ready_flag == 1){
      if(Timer5Counter >= garage_door_ready_timer){
        if(garageMotion_reading > 300){
          garagedoor_timer = Timer5Counter + 300;
        }
        else {
          if(Timer5Counter >= garagedoor_timer){
            digitalWrite(garageDoor, LOW);
            delay(100);
            digitalWrite(garageDoor, HIGH);
            //garagedoor_timer = Timer5Counter + 30;
            //Serial.println("Door Close!!!");
            garage_door_ready_flag = 0;
           }
        } 
      }
      else{
        if(garageMotion_reading > 300){//constantly checking motion 
          garagedoor_timer = Timer5Counter + 300;
        }
      }
    }
  }
  else if(garageDoor_flag == 0){
    garage_door_flag_counter++;
    //prevent sensor false reading
    if(garage_door_flag_counter>20){ 
      garage_door_ready_flag = 0;    // for our mega,the loop can not run 20 times per second
    }                                //so we are able to aviod the one time-false reading
    
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
