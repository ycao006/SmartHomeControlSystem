//-----------Home light Auto------------------
//this file is a finite state machine for home light 's state in kitchen
//state 1: has light
//state 2: no light no people
//state 3: no light has people

enum homeLightMotion{homeLightinit, homeLightState1, homeLightState2, homeLightState3};
int Home_Light_Auto(int homeLightState){
  //state check
  switch(homeLightState){
      case homeLightinit:
        if(homeBritness < 430){ //has light at day time
          homeLightState = homeLightState3;  
        }
        else if(homeBritness < 800){ //day time
          homeLightState = homeLightinit;  
        }
        else{
          homeLightState = homeLightState1;
        }
        break;
        
      case homeLightState1: //no people no light
        if(homeBritness < 800){ //has
          homeLightState = homeLightinit;
        }
        else if((homeBritness > 700)&&(homeMotion>300)){
          HomeLightAutoCounter = Timer5Counter + 120; // assign a 2 mins lighting up period for home light;
          homeLightState = homeLightState2;           
          if(homeBritness > 700){
                digitalWrite(homeLight, !digitalRead(homeLight));
          }
        }
        else{
          homeLightState = homeLightState1;
        }
        break;

      case homeLightState2: //has people, no light so turn on light
        if(Timer5Counter >= HomeLightAutoCounter){
          if(homeMotion>300){
            homeLightState = homeLightState2;
            HomeLightAutoCounter = Timer5Counter + 120;
          }
          else{
            if(homeBritness < 430){
                HomeLightAutoCounter +=2;
                digitalWrite(homeLight, !digitalRead(homeLight));
            }
            else{
              homeLightState = homeLightState1;
            }
          }
        }
        else{
           homeLightState = homeLightState2;
        }
        break;

      case homeLightState3:
        if(homeMotion < 300 && homeBritness > 430){
              motionCounter = 0;
              motionCounterFlag = 0;
              homeLightState = homeLightinit;
        }
        else if(homeMotion < 300){
          motionCounterFlag = 1;
          if(motionCounter > 120){
            current_sensor_read();
            if(homelight_flag > 200){
                digitalWrite(homeLight, !digitalRead(homeLight));
            }
            if(motionCounter > 123){
              motionCounter = 0;
              motionCounterFlag = 0;
              homeLightState = homeLightinit;
            }
          }
        }
        else{
           motionCounterFlag = 0;
           motionCounter = 0;
           homeLightState = homeLightState3;
        }
        break;    
  }  
  //state action
  switch(homeLightState){
      case homeLightinit:
          //Serial.print("state init\n");
          break;
      case homeLightState1:
          //Serial.print("state1\n");
          break;
      case homeLightState2:
          //Serial.print("state2\n");
          break;
      case homeLightState3:
          //Serial.print("state3\n");
          break;
  }  
  return homeLightState;
}




