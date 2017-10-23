#include "Website_new.h"

//---- wifi account login informtion
//char ssid[] = "OnePiece";      //  your network SSID (name)
//char pass[] = "6268181383";   // your network password
char ssid[] = "TP-LINK_7122";      //  your network SSID (name)
char pass[] = "20732073";   // your network password
//int keyIndex = 0;                 // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(80);
//------------------------------------------------------------------------
void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  //Serial.print("IP Address: ");
  //Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("RSSI:");
  Serial.print(rssi);
  Serial.println(" dBm");
  // print where to go in a browser:
  Serial.print("Open http://");
  Serial.println(ip);
}
//--------------------------------------------------------------------------
void WIFI_Init(){
  //WIFI 
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true);       // don't continue
  }
  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) {
    Serial.print("Connecting: ");
    Serial.println(ssid);                   // print the network name (SSID);

    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                           // start the web server on port 80
  printWifiStatus();                        // you're connected now, so print out the status
}

//---------------------------------------------------------------------------
void Recover_WIFI() {
   //-----------------------------------------
   
   //---- wifi reconnect function.
  if( WiFi.status() == WL_DISCONNECTED){
     Serial.print("WIFI lost \n ");
     delay(1000);
     wdt_disable();//disable watchdog 
   while ( WiFi.status()!= WL_CONNECTED) {
    Serial.print("Attempting to connect: ");
    Serial.println(ssid);                   // print the network name (SSID);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  }
  server.begin();                 // start the web server on port 80
  wdt_enable(WDTO_8S);            //re-enable watchdog
  printWifiStatus();               // you're connected now, so print out the status
  }
}
//----------------------------------------------------------------------------
  
//-----------------------------------------------------------------------------
void WIFI_Server(){
  //---- wifi server  // listen for incoming clients
  
  WiFiClient client = server.available(); 
  if (client) {                             // if you get a client,
    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character
          
          //---- if the current line is blank, you got two newline characters in a row.
          //---- that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            
            //------Print Website-----------------------------
            Home_Webpage( client );
            
            //---- The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          }
          else {      // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        }
        else if (c != '\r') {    // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":
        //----main door------------------------------------------------
        if (currentLine.endsWith("GET /?DoorOpen")) {
          DoorOpen = 1;
          DoorClose = 0;
        }
        else if (currentLine.endsWith("GET /?DoorClose")) {
          DoorClose = 1;
          DoorOpen = 0;
        }
        
        //----garage door---------------------------------------------
        if (currentLine.endsWith("GET /?GDooropen")) {
            GarageDoor_Position();
            //-------------------------------------------
            // error detection and correction
            /*while(arageDoor_flag == 2){
              digitalWrite(garageDoor, LOW);
              delay(100);
              digitalWrite(garageDoor, HIGH);
              delay(5000);
              GarageDoor_Position();
              }*/
            if(garageDoor_flag == 1);
            else if(garageDoor_flag == 0){
              digitalWrite(garageDoor, LOW);
              delay(100);
              digitalWrite(garageDoor, HIGH);
              garagedoor_timer = Timer5Counter + 300; //-- to prevent trig auto lock function immediately.
            }
        }
        
        else if (currentLine.endsWith("GET /?GDoorclosed")) {
            GarageDoor_Position();
            if(garageDoor_flag == 0);
            else if(garageDoor_flag == 1){
              digitalWrite(garageDoor, LOW);
              delay(100);
              digitalWrite(garageDoor, HIGH);
            }
        }

        //----garage light1---------------------
        if (currentLine.endsWith("GET /?GarageLightOpen")) {
            digitalWrite(garageLight, !digitalRead(garageLight));
        }
        else if (currentLine.endsWith("GET /?GarageLightClose")) {
            digitalWrite(garageLight, !digitalRead(garageLight));
        }
  
        //------ bug garage light2 ---------------
        if (currentLine.endsWith("GET /?GLightopen") && garage_small_light_flag == 0) {
          digitalWrite(garageLight2, LOW);
          delay(100);
          digitalWrite(garageLight2, HIGH);
          garage_small_light_flag = 1;
        }
        else if (currentLine.endsWith("GET /?GLightclose") && garage_small_light_flag == 1) {
          digitalWrite(garageLight2, LOW);
          delay(100);
          digitalWrite(garageLight2, HIGH);
          garage_small_light_flag = 0;
        }
        
        //----Spinkler-----------------------
        if (currentLine.endsWith("GET /?SprinklerON")) {
          digitalWrite(sprinkler , LOW);
          
        }
        else if (currentLine.endsWith("GET /?SprinklerOFF")){
          digitalWrite(sprinkler , HIGH);
        }

       //----homeLight---------------------
        if (currentLine.endsWith("GET /?LightOpen")) {
            digitalWrite(homeLight, !digitalRead(homeLight));
        }
        else if (currentLine.endsWith("GET /?LightClose")) {
            digitalWrite(homeLight, !digitalRead(homeLight));
        }

        //----AC heat---------------------
        if (currentLine.endsWith("GET /Turn_ON_Heat")) {
            AC_heating_relay = 1;
            AC_cooling_relay = 0;
        }
        else if (currentLine.endsWith("GET /Turn_OFF_Heat")) {
            AC_heating_relay = 0;
        }
        //----AC cool----------------------------------------
        if (currentLine.endsWith("GET /Turn_OFF_Cool")) {
            AC_cooling_relay = 0;
        }
        else if (currentLine.endsWith("GET /Turn_ON_Cool")) {
            AC_cooling_relay = 1;
            AC_heating_relay = 0;
        }
        //-----Turn OFF both AC heat & cool -----------------
        if (currentLine.endsWith("GET /Turn_OFF_AC")) {
            AC_cooling_relay = 0;
            AC_heating_relay = 0;
        }
        if (currentLine.endsWith("GET /AC_IS_OFF")){
            AC_cooling_relay = 0;
            AC_heating_relay = 0;
        }
      }
    }
    // close the connection:
    client.stop();
    Serial.println("disonnected");
  }
}


