
//---------------------------------------------------------------------------
long microsecondsToCentimeters(long microseconds)
{
// The speed of sound is 340 m/s or 29 microseconds per centimeter.
// The ping travels out and back, so to find the distance of the
// object we take half of the distance travelled.
return microseconds / 29 / 2;
}
//-------------------------------------------------------------
int carPosition_flag = 0;
//-------------------------------------------------------------
void car_Position(){
  // send a ultrosonic signal
  //car position 80< p < 110
  digitalWrite(carDisTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(carDisTrig, HIGH);
  delayMicroseconds(5);
  digitalWrite(carDisTrig, LOW);
  // read position
  carDuration = pulseIn(carDisEcho, HIGH);
  // convert the time into a distance
  carCm = microsecondsToCentimeters(carDuration); 
  /*
  Serial.print("Car: ");
  Serial.print(carCm);
  Serial.print("cm");
  Serial.println();
  */

}
//-------------------------------------------------------------
void Avg_car_distance(){
  if(Car_dis_cnt<=20){
    car_Position();
    Car_dis_cnt++;
    Car_dis_sum+=carCm;
  }
  else{
    carCm = Car_dis_sum/Car_dis_cnt;
    Car_dis_sum = 0;
    Car_dis_cnt = 0;
  }

  //Set Car distance flag
  if(carCm > 400) 
  {
    carPosition_flag = 0; // Not in Garage
  }
  else if(carCm <= 115 && carCm >= 80) 
  {
    carPosition_flag = 1; // Car is in Proper position
  }
  else if(carCm <= 45) 
  {
    carPosition_flag = 2; // Back door open
  }
  else 
  {
    carPosition_flag = 3; // error detected, car under the door.
  }
}

//-------------------------------------------------------------

void GarageDoor_Position(){
  // send a ultrosonic signal
  digitalWrite(garageDoorDisTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(garageDoorDisTrig, HIGH);
  delayMicroseconds(5);
  digitalWrite(garageDoorDisTrig, LOW);
  // read position
  garageDuration = pulseIn(garageDoorDisEcho, HIGH);
  // convert the time into a distance
  garageCm = microsecondsToCentimeters(garageDuration); 
  /*
  Serial.print("Garage Door: ");
  Serial.print(garageCm);
  Serial.print("cm");
  Serial.println();
  */
  if(garageCm > 180) 
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
}
//-------------------------------------------------------------
void HomeLight_sensor(){
  homeBritness = analogRead(homelightBritness);
  homeMotion = analogRead(homelightMotion);
}
//------------------------------------------------------------
void garageMotion_sensor(){
  garageMotion_reading = analogRead(garageMotion);
  garageBritness_reading = analogRead(garageBritness);

}
//--------------------------------------------------------------
void temperature_read(){
  sensors.requestTemperatures();
  temperature = sensors.getTempCByIndex(0);
}
//--------------------------------------------------------------
void humidity_read(){
  soil_humidity = 0;
  for(int j = 0; j < 50; j++){
    soil_humidity += analogRead(sprinklerHumidity);
  }
  soil_humidity = soil_humidity / 50;
}
//--------------------------------------------------------------
int check_relay=0;

void current_sensor_read(){
  garage_light_flag = 0;
  homelight_flag = 0;
  sprinkler_flag = 0;
  for(int j = 0; j < 20; j++){
    delay(10);
    homelight_flag += analogRead(homelightACSensor);
    //sprinkler_flag += analogRead(sprinklerACSensor);
    //garage_light_flag += analogRead(garageLightACSensor);
  }

  //--------------------------------------------------------------------
  if(!digitalRead(garageLight)) 
    check_relay++;
  if(!digitalRead(homeLight)) 
    check_relay++;
  if(!digitalRead(sprinkler))
  {  check_relay++;
      sprinkler_flag = 1;
  }
  else{
    sprinkler_flag = 0;
  }
  //----------------------------------
  if(homelight_flag < 400 && homeBritness >430){
    homelight_flag = 0;
  }
  //---------------------------------------------------------------------
  /*if(check_relay == 0){           //condition: No Relay On
    if(garage_light_flag < 10)   //                         ON: 620-720
      garage_light_flag = 0;      // garage_ligh_flag Reading: OFF: 0
  }                             //                         ON: 150 - 220
  else if(check_relay==1){
    if(garage_light_flag < 80)
      garage_light_flag = 0;
  }
  else if(check_relay==2){
    if(garage_light_flag < 170)
      garage_light_flag = 0;
  }
  else if(check_relay==3){
    if(garage_light_flag < 190)
      garage_light_flag = 0;
  }*/
  if( garageBritness_reading < 200 ){
    garage_light_flag = 100;
  }
  else{
    garage_light_flag = 0;
  }
  
  //-------------------------------------------------------------------
  check_relay=0;


}
//-------------------------------------------------------------








