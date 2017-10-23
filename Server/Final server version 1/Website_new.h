void Home_Webpage(WiFiClient client ){
 //-----------------HTML WEBSERVER---------------------------------------------------------------------------------------
            //---- the content of the HTTP response follows the header:
             //client.println("<!DOCTYPE HTML>");
             client.println("<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">");
             client.println("<html>");
                
      //-------------new added---------------------------------------------------------------------------------
                   // 添加一个meta刷新标签, 浏览器会每20秒刷新一次
                   // 如果此处刷新频率设置过高，可能会出现网页的卡死的状况
                     client.println("<meta http-equiv=\"refresh\" content=\"25; ,Home\">"); 

                     //Read Sensor value
                     current_sensor_read();
                     humidity_read();
                     temperature_read();
                     HomeLight_sensor();
                     Uart_request(x[3]);
                     //Uart_request(x[1]);
                     //Uart_request(x[2]);
                     
      //---------------------------------------------------------------------------------------------------
              
            client.println("<HEAD>");
             client.println("<meta name='apple-mobile-web-app-capable' content='yes' />"); //添加到主屏后的标题
             client.println("<meta name='apple-mobile-web-app-status-bar-style' content='black-translucent' />");
              //设置状态栏的背景颜色，只有在 `"apple-mobile-web-app-capable" content="yes"` 时生效

              
       //----------------new added -----------------------------------------------------------------------------
                //iPhone 6对应的图片大小是750×1294，iPhone 6 Plus 对应的是1242×2148 。
               client.println( "<link rel=\"apple-touch-startup-image\" href=\"launch6.png\" media=\"(device-width: 375px)\">");
               client.println( "<link rel=\"apple-touch-startup-image\" href=\"launch6plus.png\" media=\"(device-width: 414px)\">");

      // -------------------------------------------------------------------------------------------------------
             // ---------css file----------
             client.println("<link rel='stylesheet' type='text/css' href='https://364200ed38e3b4bbcdd16e9d6ce0bbefa37bbea7-www.googledrive.com/host/0B2VjPyYzfuZkN1dDd250RE9rRG8' /></link>");
            // ---------weather function ----------
            client.println("<iframe id=\"forecast_embed\" type=\"text/html\" frameborder=\"0\" height=\"245\" width=\"100%\"");
            client.println("src=\"http://forecast.io/embed/#lat=33.9533&lon=-117.3952&name=Riverside &color=#00aaff&font=Georgia&units=us\"></iframe>");
     
     //------------------------------------------------------------------------------------        
             client.println("<title>Senior Design: Home System</title>");
             
             client.println("</HEAD>");

             //----------button lightup JS function------------------------------------------
            /* client.println("<script>");
             client.println("function changeclass(obj){document.getElementById(obj).className=");
             client.println("(document.getElementById(obj).className=='menuitemup'?'menuitemdown':'menuitemup')}");
             client.println("function changefont(obj){document.getElementById(obj).innerHTML=");
             client.println("(document.getElementById(obj).innerHTML=='OFF'?'On':'OFF')}  ");
             client.println("function sprinkler(obj){changefont (obj); changeclass(obj);}");
             client.println("</script>");
             //------------------------------------------------------
             */
             
             client.println("<body>");

             //---------header of the website----------------------- 
             client.println("<div id=\"pageWrapper\"><div id=\"header\">");
             client.println("<img src=\"http://activemotionrehab.com/wp-content/uploads/2015/05/home-security-monitoring.jpg\"");
             
             client.println("alt=\"Intelligent Home System\" width=\"400px\" height=\"320px\" id=\"logo\"/>");
             client.println("<h1 id=\"blogTitle\"></h1>");
             client.println("<p id=\"blogSlogan\">Intelligent Home System</p>");



             //------------------------------------------------------------------------------
             //----------------------Garage door and garage door light-----------------------
             client.println("<div id=\"menu\"><div class=\"menuNormal\">");
             client.println("<table class=\"menu\">");
             client.println("<tr><td><font size=\"5px\"><strong><p>Garage door:</p></strong></font></td>");
             
             client.println("<td><a href=\"/?GDooropen\" class=\"");
             if(garageDoor_flag == 1)
                client.println("down");
             else
                client.println("up");
             client.println("\">Open</a></td>");
             
             client.println("<td ><a href=\"/?GDoorclosed\" class=\"");
             if(garageDoor_flag == 0)
                client.println("down");
             else
                client.println("up");
             client.println("\">Close</a></td>");
             
             if(garage_small_light_flag == 0)
                client.println("<td ><a href=\"/?GLightopen\" class=\"up\">Small light</a>");
             else
                client.println("<td ><a href=\"/?GDoorclosed\" class=\"down\">Small light</a>");
             client.println("</td></tr>");
             
             //------------Garage and Home light -------------------------------------------------------- 
             client.println("<tr><td><font size=\"5px\"><strong><p>Lights:</p></strong></font></td>");
             if(garage_light_flag >= 50)
                client.println("<td ><a href=\"/?GarageLightClose\" class=\"down\">Glight</a></td>");
             else
                client.println("<td ><a href=\"/?GarageLightOpen\" class=\"up\">Glight</a></td>");
                
             if(homelight_flag >= 200)
                client.println("<td ><a href=\"/?LightClose\" class=\"down\">Homelight</a></td></tr>");
             else
                client.println("<td ><a href=\"/?LightOpen\" class=\"up\">Homelight</a></td></tr>");
             
             //------------Air-conditioner-------------------------------------------------------- 
             client.println("<tr><td><font size=\"5px\"><strong><p>Air-conditioner:</p></strong></font></td>");
             if(AC_Cooling_state == "ON")
                client.println("<td ><a href=\"/Turn_OFF_Cool\" class=\"down\">Cold ON</a></td>");
             else if(AC_Cooling_state == "OFF")
                client.println("<td ><a href=\"/Turn_ON_Cool\" class=\"up\">Cold OFF</a></td>");
             
             if(AC_Heating_state == "ON")
                client.println("<td > <a href=\"/Turn_OFF_Heat\" class=\"down\">Heat ON</a></td>");
             else if(AC_Heating_state == "OFF")
                client.println("<td > <a href=\"/Turn_ON_Heat\" class=\"up\">Heat OFF</a></td>");
             
             if(AC_Cooling_state == "OFF" && AC_Heating_state == "OFF")
                client.println("<td ><a href=\"/AC_IS_OFF\" class=\"down\">Off</a></td></tr>");
             else
                client.println("<td ><a href=\"/Turn_OFF_AC\" class=\"up\">Off</a></td></tr>");
             
             //------------Home Door-------------------------------------------------------- 
             client.println("<tr><td><font size=\"5px\"><strong><p>Home door:</p></strong></font></td>");
             
             client.println("<td ><a href=\"/?DoorOpen\" class=\"");
             if(MainDoor_state=="Close")
                client.println("up");
             else if(MainDoor_state=="Open")
                client.println("down");
             client.println("\">Open</a></td>");

              client.println("<td ><a href=\"/?DoorClose\" class=\"");
             if(MainDoor_state=="Open")
                client.println("up");
             else if(MainDoor_state=="Close")
                client.println("down");
             client.println("\">Close</a></td></tr>");
              
             //------------Sprinkler-------------------------------------------------------- 
             client.println("<tr><td><font size=\"5px\"><strong><p>Sprinkler:</p></strong></font></td>");
             if(sprinkler_flag ==1)
                client.println("<td ><a href=\"/?SprinklerOFF\" class=\"down\">ON</a></td>");
             else
                client.println("<td ><a href=\"/?SprinklerON\" class=\"up\">OFF</a></td>");
             
             client.println("</tr></table></div></table></div> ");
        //-------------- End of botton ---------------------------------------


        //---------Home Status----------------------------
         client.println("<div id=\"state\"><h1>House States</h1>");
             client.println("<a>Home Temperature: </a><a class=\"st\">");
             
              client.print(temperature);
              
                 //-------- Home Condition -------------------------------------------------------------------
                     client.println("&#8451</a><br><a>Home Brightness: </a><a class=\"st\">");
                     client.print(homeBritness);
                     client.println( "</a><br><a>Home Motion: </a><a class=\"st\">");
                     client.print(homeMotion);
                     client.println( "</a>"); 
                 //-----------------------------------------------------------------------------------------------------
             
             client.println("<br><a>Garage door distance: </a><a class=\"st\">");
             //--------Garage Door Position ----------------------------------------
                   //GarageDoor_Position();
                   client.print( garageCm );
                   client.println( " cm;");     
             client.println("</a><br><a>Garage door state: </a><a class=\"st\">");
                   if( garageDoor_flag == 1) // opened
                      client.println("OPEN");
                   else if( garageDoor_flag == 0) // closed
                      client.println("CLOSE"); 
                   else
                      client.println("ERROR"); 
                //----------------------------------------------------------------------
                //client.println("<br><a>Back door distance:</a>");
             
             client.println("</a><br><a>Car Position: </a><a class=\"st\">");
                 //-------Car Position ------------------------------------------------------------------------------
                 //car_Position();
                 client.print( carCm );
                 client.println( " cm; ");
                  if( carPosition_flag == 1) // 
                     client.println( "Car in Garage");
                  else if( carPosition_flag == 0) // 
                     client.println( "Car NOT in Garage"); 
                  else if( carPosition_flag == 2) // 
                     client.println( "Back Door Opened");
                  else
                     client.println( "ERROR");
                 //---------------------------------------------------------------------------------------------------
                 
             client.println("</a><br><a>AC State: </a><a class=\"st\">");
                if(AC_CoolDown_flag == 1 ){
                  client.println("Cooldown");
                }
                else if(AC_Cooling_state == "ON" && AC_Heating_state == "OFF"){
                  client.println("Cold ON");
                }
                else if(AC_Cooling_state == "OFF" && AC_Heating_state == "ON"){
                  client.println("Heat ON");
                }
                else if(AC_Cooling_state == "OFF" && AC_Heating_state == "OFF"){
                  client.println("OFF");
                }
                else{
                  client.println("Error");
                }
            ////////// client.print( air-conditioner state );


            
             client.println("</a><br><a>Sprinkler state: </a><a class=\"st\">");
                  //-------- Sprinkler Condition -------------------------------------------------------------------
                // client.println("<a>");
                 if( sprinkler_flag ==1) // opened
                    client.println("OPEN");
                 //else if( sprinkler_flag == 0) // Closed
                 else
                    client.println("CLOSE"); 
                 //-----------------------------------------
                 
             client.println("</a><br/>"); 
             client.println("</div>");

             //-- End of states ---------------------


             
     //---- Control Button--------------------------------------------------------------------------------------------
             //---- main door button
   //          client.println("<br/>");  
   //          client.println("<a href=\"/?DoorOpen\"\">Open the Door</a>");
   //          client.println("<a href=\"/?DoorClose\"\">Close the Door</a><br />");   
   //          client.println("<br />");     
             
             //---- Garage door button -------------------------------------------------------------------------
   //          client.println("<br />"); 
   //          client.println("<a href=\"/?GDooropen\"\">Open Garage Door</a>");
   //          client.println("<a href=\"/?GDoorclosed\"\">Close Garage Door</a>"); 
               
            //--------------------------------------------------------------------------------------------------
            
           
             //---- Garage Light1 button (110V)-------------------------------------------------------------------
             //client.println("<br />"); 
     //        client.println("<a href=\"/?GarageLightOpen\"\">Turn On Garage light</a>");
     //        client.println("<a href=\"/?GarageLightClose\"\">Turn OFF Garage light</a><br />"); 
            // client.println("<br />"); 
             
             //---- Garage light2 button (on the garage door controler)------------------------------------------
    //         client.println("<br />"); 
    //       if(garagelight_flag == 0)
    //            client.println("<a href=\"/?GLightopen\"\">Turn on Garage small light</a><br />");
    //         else
    //            client.println("<a href=\"/?GLightclose\"\">Turn off Garage small light</a><br />"); 
    //         client.println("<br />");
             
             //---- Sprinkler button  ----------------------------------------------------------------------------
            // client.println("<br />"); 
             //client.println("<a href=\"/?SprinklerON\"\">Open Sprinkler</a>");
             //client.println("<a href=\"/?SprinklerOFF\"\">Close sprinkler</a>"); 
                
            //-----------------------------------------------------------------------------------------------------
            
             //---- HomeLight button ------------------------------------------------------------------------------
          //   client.println("<br />"); 
          //   client.println("<a href=\"/?LightOpen\"\">Turn On Homelight</a>");
          //   client.println("<a href=\"/?LightClose\"\">Turn OFF Homelight</a>"); 
             
             
             
             //-----------------------------------------------------------------------------------------
             //---- camera part
             //---- camera snapshot  
             //client.println("<img src = \"http://192.168.1.109:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=guest&pwd=2073\">"); 
             //---- camera video  
               client.println("<div id=\"cam\"><h2>IP camera</h2>");
               client.println("<script language=\"javaScript\" type=\"text/javascript\">");
               client.println("function reload(){setTimeout('reloadImg(\"refresh\")',1000)};");
               client.println("function reloadImg(id){");
               client.println("var obj= document.getElementById(id);");
               client.println("var date= new Date();");
               client.println("obj.src=\"http://192.168.1.109:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=guest&pwd=2073&t=\" + Math.floor(date.getTime()/1000);}");
               client.println("</script>");
               client.println("<img src=\"http://192.168.1.109:88/cgi-bin/CGIProxy.fcgi height=\"600\" width=\"1000\"");
               client.println("fcgi?cmd=snapPicture2&usr=guest&pwd=2073&t=\" name=\"refresh\" id=\"refresh\" onload='reload(this)' onerror='reload(this)'>");
               
               //---------Google Search Box--------------------------
               client.println("<div id=\"searchBox\"><form action=\"https://www.google.com/search\" method=\"get\">");
               client.println("<input type=\"text\" id=\"s\" name=\"q\" size=\"31\" maxlength=\"255\" value=\"\"/>");
               client.println("<input type=\"hidden\" name=\"ie\" value=\"GB2312\"/>");
               client.println("<input type=\"hidden\" name=\"oe\" value=\"GB2312\"/>");
               //client.println("<input type=\"submit\" name=\"btnG\" value=\"Google\"/></div>");
               client.println("<input type=\"image\" src=\"http://www.thelogofactory.com/wp-content/uploads/2015/09/fixed-google-logo-font.png\" alt=\"Submit Form\" name=\"btnG\" height=\"25\" width=\"80\"</div>");
               client.println("</form></div>");
               
               client.println("</div>");
            //-----------------------------------------------------------------------------------------     
            //--------------------------------------------------------
              client.println("</body>");
             client.println("</html>");
          }
           
