#include <EEPROM.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <avr/wdt.h>  
#include <SPI.h>
#include <WiFi101.h>
//#include <Wire.h>

#include "Variable.h"
#include "Sensor_Function.h"
#include "Garage_Light_Auto.h"
#include "Home_Light_Auto.h"
#include "UART.h"
#include "WIFI_Server.h"
#include "Timer.h"
#include "Auto_Lock.h"

void setup() {
 //---- setup pin mode -----------------------------------------------------------
 //---- output digital pin--------------------------------------------------------
  pinMode (garageDoor,OUTPUT); 
  pinMode (garageLight,OUTPUT);
  pinMode (garageLight2,OUTPUT);
  pinMode (sprinkler,OUTPUT);
  pinMode (homeLight,OUTPUT);
//---- output pin for relay initialization  ---------------------------------------
  digitalWrite(garageDoor, HIGH); // disable Relay (normally Closed);
  digitalWrite(garageLight, HIGH);
  digitalWrite(garageLight2, HIGH);
  digitalWrite(sprinkler, HIGH);
  digitalWrite(homeLight, HIGH);

//---- sensor pin -----------------------------------------------------------------
  //---- Ultrasonic sensor-----------------------------------------------------------
  pinMode(garageDoorDisTrig, OUTPUT);
  pinMode(garageDoorDisEcho,INPUT);
  pinMode(carDisTrig, OUTPUT);
  pinMode(carDisEcho,INPUT);

  //---- AC sensors -----------------------------------------------------------
  pinMode(homelightACSensor,INPUT);
  pinMode(sprinklerACSensor,INPUT);
  pinMode(garageLightACSensor,INPUT);

  //---- Other sensors -----------------------------------------------------------
  pinMode(homelightBritness,INPUT);
  pinMode(homelightMotion,INPUT);
  pinMode(garageMotion,INPUT);
  pinMode(homeTemp,INPUT);
  pinMode(sprinklerHumidity,INPUT);
  
  //-------RS232 -----------------------------------------------------------------
  Serial.begin(9600);      // initialize serial communication
  Serial1.begin(9600);    //Arduino Mega Serial port 1
  //------------------------------------------------------------------------------
      //I2C communication
      //Wire.begin(); 
      // check for the presence of the shield:
  
  //------WIFI initialization-----------------------------------------------------
  WIFI_Init();

  //-----------Initialize Maindooe state -----------------------------------------
  for(int p = 0; p<3; p++){
    delay(1000);
    Uart_request(x[3]); // x[0] = 1; Main door state;
    Serial.print("Maindoor State: ");
    Serial.println(MainDoor_state);
    Serial.print("AC_Heating_state: ");
    Serial.println(AC_Heating_state);
    Serial.print("AC_Cooling_state: ");
    Serial.println(AC_Cooling_state);
  }
 /* for(int p = 0; p<3; p++){
    delay(1000);
    Uart_request(x[1]); // x[0] = 1; Main door state;
    Serial.print("AC_Heating_state: ");
    Serial.println(AC_Heating_state);
  }
  for(int p = 0; p<3; p++){
    delay(1000);
    Uart_request(x[2]); // x[0] = 1; Main door state;
    Serial.print("AC_Cooling_state: ");
    Serial.println(AC_Cooling_state);
  }
  */
   //----- Flag Reset from EEprom------------------------------------------------
   //EEPROM.update(AC_CoolDown_counter_address,AC_CoolDown);
   //EEPROM.update(Timer5Counter_address,Timer5Counter);
   //EEPROM.update(AC_CoolDown_flag_address,AC_CoolDown_flag);
   //EEPROM.update(AC_heating_relay_address,AC_heating_relay);
   //EEPROM.update(AC_cooling_relay_address,AC_cooling_relay);
   //EEPROM.update(AC_CoolDown_Diff_address,0);

   AC_heating_relay = EEPROM.read(AC_heating_relay_address);
   AC_cooling_relay = EEPROM.read(AC_cooling_relay_address);
   AC_CoolDown_flag = EEPROM.read(AC_CoolDown_flag_address);
   AC_CoolDown_Diff = EEPROM.read(AC_CoolDown_Diff_address);

  AC_CoolDown = (AC_CoolDown_Diff*3) + Timer5Counter;
  //--------- Initialize Timer ---------------------------------------------------
  Timer_init();    //initialize Timer5 in Arduino Mega
  Watchdog_init();//initialize Watchdog timer in Arduino Mega
  current_sensor_read();
}

//--------Timer Interupt Service Routine -------------------------------------------
ISR(TIMER5_OVF_vect){
  //TCNT1 = 34286;            // preload timer
  //Serial.print(TCNT1);
  //Serial.print("\n");
  //digitalWrite(ledPin1, digitalRead(ledPin1) ^ 1);
  TCNT5 = 0;
  Timer5Counter = Timer5Counter+1;
  if(AC_CoolDown_flag == 1){
    if(AC_CoolDown>Timer5Counter){
      AC_CoolDown_Diff = (AC_CoolDown - Timer5Counter)/3;
    }
    else
      AC_CoolDown_Diff = 0;
    EEPROM.update(AC_CoolDown_Diff_address,AC_CoolDown_Diff);
  }
  if(motionCounterFlag == 1){
     motionCounter++;
  }
  HomeLight_sensor();
  garageMotion_sensor();
  humidity_read();
  Serial.println(Timer5Counter);
 /*
  Serial.print("garage_door_ready_flag: ");
  Serial.println(garage_door_ready_flag);
  Serial.print("garage_door_ready_timer: ");
  Serial.println(garage_door_ready_timer);
  Serial.print("garagedoor_timer: ");
  Serial.println(garagedoor_timer);
*/
  
  //Serial.println(analogRead(sprinklerHumidity));
  
  //Serial.print("Humidity: ");
  //Serial.println(soil_humidity);
  //Serial.print("maindoor_timer: ");
  //Serial.println(maindoor_timer);  
  //Serial.print("MainDoor_state: ");
  //Serial.println(MainDoor_state);
  //Serial.print("garageBritness_reading: ");
  //Serial.println(garageBritness_reading);
  
  //Serial.print("maindoor_Autolock_flag: ");
  //Serial.println(maindoor_Autolock_flag);
  

  
/*  Serial.print("AC Action: ");
  Serial.print(AC_heating_relay);
  Serial.print(" and ");
  Serial.println(AC_cooling_relay);
  Serial.print("CoolDown: ");
  Serial.println(AC_CoolDown);
  
  Serial.print("CoolDown Flag: ");
  Serial.println(AC_CoolDown_flag);
*/
  //Serial.print("state: ");
  //Serial.println(carState);
  //Serial.print("LightAutoCounter: ");
  //Serial.println(LightAutoCounter);
  //Serial.print("LightBlinkCounter: ");
  //Serial.println(LightBlinkCounter);
  //Serial.print("Brightness: ");
  //Serial.println(homeBritness);
  //Serial.print("Motion: ");
  //Serial.println(homeMotion);
  //GarageDoor_Position();
  //car_Position();

}
//----------------------------------------------------------------------------------

void loop() {
  //---- wifi reconnect function. -------------------------------------------
  Recover_WIFI(); //reconnect to WIFI if WIFI connection is lost
  
  //---- WiFi server --------------------------------------------------------
  //WiFiClient client = server.available();   // listen for incoming clients
  WIFI_Server();
  HomeLight_sensor();

  GarageDoor_Position();
  //car_Position();
  Avg_car_distance();
  //AUTO control Garage Light when Car is in place---------------------------
  if(carPosition_flag != 2){
    //called Garage_Light_Auto function only when backdoor is close
    carState = Garage_Light_Auto(carState);
  }
  //---- Main Door Control---------------------------------------------------
    Maindoor_Control(); //  Main Door Control locates in UART.h
  //-------------------------------------------------------------------------
  //-----AC Control --------------------------------------
    AC_Control();
  //------------------------------------------------------
  //----AUTO control Home Light --------------------------
    homeLightState = Home_Light_Auto(homeLightState);
  //---------------------------------------------------
  //-------------Auto door lock------------
   garage_door_auto_lock();
   main_door_auto_lock();
  //----------------------------------------
  //--- garage small light counter ----------------------------------------------------
  if(garage_small_light_flag == 1){
      cnt = Timer5Counter + 300;
  }
  if(Timer5Counter >= cnt)
  {
    garage_small_light_flag = 0;
    cnt = 0;
  }

  //-------------------------------------------------------------------------
  wdt_reset(); //reset watchdog timer counter.
              //If watchdog is not reset within 9 sec, it resets the program.
  //-------------------------------------------------------------------------
  
  //Home sensor test
  //Serial.print("Brightness: ");
  //Serial.println(homeBritness);
  //Serial.print("Motion: ");
  //Serial.println(homeMotion);
  
    /* //Temperature Test
  temperature_read();
  Serial.print("temperature: ");
  Serial.println(temperature);
  delay(100);
  */

  // AC sensor Test
  
  //current_sensor_read();
  //Serial.print("homelight_flag: ");
  //Serial.println(homelight_flag);
  //Serial.print("garage_light_flag: ");
  //Serial.println(garage_light_flag);
  //Serial.print("sprinkler_flag: ");
  //Serial.println(sprinkler_flag);
  
  /*
  // Maindoor state test
  Uart_request(x[0]);
  Serial.print("Maindoor State: ");
  Serial.println(MainDoor_state);
  delay(1000);
  */         

  // Sprinkler test
  //Serial.println(analogRead(sprinkler_flag));
  //delay(3);

  //garage door distance test
  //GarageDoor_Position();
  //Serial.println(garageCm);
  //car_Position();

  //--------------humidity test ------------------------- 
  //humidity_read();
    //Serial.print("Humidity: ");
  //Serial.println(analogRead(sprinklerHumidity));
    //Serial.println(soil_humidity);

}
