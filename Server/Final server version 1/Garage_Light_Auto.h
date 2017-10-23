//-----------Car Auto------------------
//this file is a finite state machine for car's state in garage door
//state 1: no car, garage door is closed
//state 2: car moving in, garage door is open
//state 3: car moving into the parking postion, garage door is open
//         state action1: garage door blink three times to inform driver car is in the right parking postion
//         state action2: garage light is on for 3 minitures, then light is off
//         
//state 4: car in, door open
//state 5: car moving out, garage door is open
//state 6: car is out, garage door is open.

/*
 *   if(garageCm > 180) 
  {
    garageDoor_flag = 1; // opened
  }
  else if(garageCm <= 55) 
  {
    garageDoor_flag = 0; // closed
  }
  else 
  {
    garageDoor_flag = 2; //error
  }
 */
 /*
  if(carCm > 400) 
  {
    carPosition_flag = 0; // Not in Garage
  }
  else if(carCm <= 110 && carCm >= 80) 
  {
    carPosition_flag = 1; // Car is in Proper position
  }
  else if(carCm <= 45) 
  {
    carPosition_flag = 2; // back door open
  }
  else 
  {
    carPosition_flag = 3; // error detected, car under the door.
  }
*/
enum carPostion{carInit, carState1, carState2, carState3, carState4, carState5, carState6};
int Garage_Light_Auto(int carState){
  //state check
  switch(carState){
      case carInit:
        if((carCm > 200)&&(garageDoor_flag==0)){ //no car, door close
          LightAutoCounter2 = 0;
          carState = carState1;
        }
        else if((carCm <= 130 && carCm >= 60) &&(garageDoor_flag==1)){ //  car in door open
           LightAutoCounter2 = 0;
          carState = carState6;
        }
        else if(garageDoor_flag==1){// unknown car position door open
           LightAutoCounter2 = 0;
           carState = carState2;
           
        }
        else{
          carState = carInit;
        }
        break;
        
      case carState1: //no car door close
        if((carPosition_flag == 1)&&(garageDoor_flag == 1)){ //car just in proper postion , door open
          carState = carState3;
          LightAutoCounter = Timer5Counter+1;
        } 
        else if(garageDoor_flag == 1){ // no car, door open
          carState = carState2;
        }
        
        else if((garageDoor_flag == 0) && (carCm<115)){ // car in, door close
          carState = carInit;
        }
        else{
          carState = carState1;
        }
        break;

      case carState2: //no car, door open
        if((carPosition_flag == 0)&&(garageDoor_flag == 0)){// no car, door close
          carState = carState1;
        }
        else if((carPosition_flag == 1)&&(garageDoor_flag == 1)){// car just in proper position, door open
          carState = carState3;
          LightAutoCounter = Timer5Counter+1;
        }
        else{
          carState = carState2;//else no car, door open
        }
        break;

      case carState3:
        if(LightBlinkCounter >= 4){//car just in propoer postion, garage door close
          carState = carState4;
          LightAutoCounter = Timer5Counter;
          LightBlinkCounter = 0;
          current_sensor_read();
          if(garage_light_flag == 0){
             digitalWrite(garageLight, !digitalRead(garageLight));
          }
        }
        else{
          carState = carState3;
        }
        break;

      case carState4:
        if(Timer5Counter >= (LightAutoCounter+120)){//car in proper postion, grage door open
          carState = carState6;
        }
        else if( ((carCm > 50)&&(carCm < 80)) || ((carCm>115)&&(carCm<200)) ){ // car is not in proper postion, door open
          if (LightAutoCounter2 == 0){
           LightAutoCounter2 = Timer5Counter+3;
          }
          else if(Timer5Counter >= LightAutoCounter2){
            current_sensor_read();
            if(garage_light_flag >= 50){
              digitalWrite(garageLight, !digitalRead(garageLight));
            }
            carState = carState5; // car in, door open
            LightAutoCounter2 = 0;
          }
        }
        else{
          carState = carState4;
          LightAutoCounter2 = 0;
        }
        break;

      case carState5:
        if(Timer5Counter >= (LightAutoCounter+120)){//car in proper postion, grage door open
          carState = carState6;
        }
        else if(carPosition_flag == 1){
          if (LightAutoCounter2 == 0){
            LightAutoCounter2 = Timer5Counter+3;
          }
          else if(Timer5Counter >= LightAutoCounter2){
            current_sensor_read();
            if(garage_light_flag < 50){
              digitalWrite(garageLight, !digitalRead(garageLight));
            }
            carState = carState4; // car in, door open
            LightAutoCounter2 = 0;
          } 
        }
        else{
          LightAutoCounter2 = 0;
          carState = carState5;
        }
        break;

       case carState6:
       if((garageDoor_flag == 0) && (carCm<115)){ // car in, door close
          carState = carInit;
          current_sensor_read();
          if((garage_light_flag >= 50) && (garageMotion_reading < 300)){
             digitalWrite(garageLight, !digitalRead(garageLight));
          }
       }
       else if((garageDoor_flag == 1)&&(carCm>300)){
          carState = carState2;
       }
       else{
          carState = carState6; // car in, door open
       }
       break;
        
  }  
  //state action
  switch(carState){
      case carInit:
          if(garageMotion_reading > 300){
            LightAutoCounter2 = Timer5Counter + 600;
          }
          else{
            if(Timer5Counter > LightAutoCounter2){
              current_sensor_read();
              if((garage_light_flag >= 50) && (garageMotion_reading < 300)){
               digitalWrite(garageLight, !digitalRead(garageLight));
              }
              LightAutoCounter2 = 0;
            }
          }
          //Serial.print("state init\n");
          break;
      case carState1:
         // Serial.print("state1\n");
          break;
      case carState2:
         // Serial.print("state2\n");
          break;
      case carState3:
          //Serial.print("state3\n");
          if(Timer5Counter == LightAutoCounter){
            digitalWrite(garageLight, !digitalRead(garageLight));
            LightBlinkCounter = LightBlinkCounter + 1;
            LightAutoCounter = Timer5Counter + 1;
          }
          break;
      case carState4:
         // Serial.print("state4\n");
          break;
      case carState5:

          //Serial.print("state5\n");
          break;
      case carState6:
          //Serial.print("state6\n");
          break;
  }  
  return carState;
}


