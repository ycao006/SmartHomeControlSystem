void Home_Webpage(WiFiClient client ){
 //-----------------HTML WEBSERVER---------------------------------------------------------------------------------------
            //---- the content of the HTTP response follows the header:
             client.println("<!DOCTYPE HTML>");
             client.println("<HTML>");
                
      //-------------new added---------------------------------------------------------------------------------
                   // 添加一个meta刷新标签, 浏览器会每20秒刷新一次
                   // 如果此处刷新频率设置过高，可能会出现网页的卡死的状况
                     client.println("<meta http-equiv=\"refresh\" content=\"20; Home\">"); 
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
             client.println("<link rel='stylesheet' type='text/css' href='http://randomnerdtutorials.com/ethernetcss.css' />");
             client.println("<TITLE>HOME CONTROL SYSTEM</TITLE>");
             client.println("</HEAD>");
             client.println("<BODY>");
             client.println("<H1>HOME CONTROL SYSTEM</H1>");
             client.println("<hr />");
             client.println("<br />");  
             client.println("<H2>Arduino with WIFI101 Shield</H2>");
            
     //---- Control Button--------------------------------------------------------------------------------------------
             //---- main door button
             client.println("<br />");  
             client.println("<a href=\"/?DoorOpen\"\">Open the Door</a>");
             client.println("<a href=\"/?DoorClose\"\">Close the Door</a><br />");   
             client.println("<br />");     
             
             //---- Garage door button -------------------------------------------------------------------------
             client.println("<br />"); 
             client.println("<a href=\"/?GDooropen\"\">Open Garage Door</a>");
             client.println("<a href=\"/?GDoorclosed\"\">Close Garage Door</a>"); 
                //--------Garage Door Position ----------------------------------------
                 client.println("<a \">Garage Door: ");
                 GarageDoor_Position();
                 client.print( garageCm );
                 client.println( " cm;  ");
                 if( garageDoor_flag == 1) // opened
                    client.println( "OPEN </a><br />");
                 else if( garageDoor_flag == 0) // closed
                    client.println( "CLOSE </a><br />"); 
                 else
                    client.println( "ERROR  </a><br />"); 
                 
             client.println("<br />"); 
            //--------------------------------------------------------------------------------------------------
            
            //-------Car Position ------------------------------------------------------------------------------
             client.println("<br />"); 
             client.println("<a \">Car Position:  ");
             car_Position();
             client.print( carCm );
             client.println( " cm; ");
             
              if( carPosition_flag == 1) // 
                 client.println( "In Garage </a><br />");
              else if( carPosition_flag == 0) // 
                 client.println( "NOT In Garage </a><br />"); 
              else if( carPosition_flag == 2) // 
                 client.println( "Back Door Opened </a><br />");
              else
                 client.println( "ERROR Detected </a><br />");
             
             client.println("<br />"); 
             //---------------------------------------------------------------------------------------------------
             
             //---- Garage Light1 button (110V)-------------------------------------------------------------------
             client.println("<br />"); 
             client.println("<a href=\"/?GarageLightOpen\"\">Turn On Garage light</a>");
             client.println("<a href=\"/?GarageLightClose\"\">Turn OFF Garage light</a><br />"); 
             client.println("<br />"); 
             
             //---- Garage light2 button (on the garage door controler)------------------------------------------
             client.println("<br />"); 
             if(garagelight_flag == 0)
                client.println("<a href=\"/?GLightopen\"\">Turn on Garage small light</a><br />");
             else
                client.println("<a href=\"/?GLightclose\"\">Turn off Garage small light</a><br />"); 
             client.println("<br />");
             
             //---- Sprinkler button  ----------------------------------------------------------------------------
             client.println("<br />"); 
             client.println("<a href=\"/?SprinklerON\"\">Open Sprinkler</a>");
             client.println("<a href=\"/?SprinklerOFF\"\">Close sprinkler</a>"); 
                //-------- Sprinkler Condition -------------------------------------------------------------------
                 client.println("<a \">Sprinker: ");
 
                 if( sprinkler_flag == 1) // opened
                    client.println( "OPEN </a><br />");
                 else if( sprinkler_flag == 0) // Closed
                    client.println( "CLOSE </a><br />"); 
                 
             client.println("<br />"); 
            //-----------------------------------------------------------------------------------------------------
            
             //---- HomeLight button ------------------------------------------------------------------------------
             client.println("<br />"); 
             client.println("<a href=\"/?LightOpen\"\">Turn On Homelight</a>");
             client.println("<a href=\"/?LightClose\"\">Turn OFF Homelight</a>"); 
             //-------- Home Condition -------------------------------------------------------------------
                 HomeLight_sensor();
                 client.println("<a \">Home Brightness: ");
                 client.print(homeBritness);
                 client.println( "; Motion: ");
                 client.print(homeMotion);
                 client.println( " </a><br />"); 
             client.println("<br />"); 
           //-----------------------------------------------------------------------------------------------------
             client.println("</BODY>");
             client.println("</HTML>");
             
             //-----------------------------------------------------------------------------------------
             //---- camera part
             //---- camera snapshot  
             //client.println("<img src = \"http://192.168.1.109:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=guest&pwd=2073\">"); 
             //---- camera video  
               client.println("<script language=\"javaScript\" type=\"text/javascript\">");
               client.println("function reload(){setTimeout('reloadImg(\"refresh\")',1000)};");
               client.println("function reloadImg(id){");
               client.println("var obj = document.getElementById(id);");
               client.println("var date = new Date();");
               client.println("obj.src = \"http://192.168.1.109:88/cgi-bin/CGIProxy.fcgi?cmd=snapPicture2&usr=guest&pwd=2073&t=\" + Math.floor(date.getTime()/1000);}");
               client.println("</script>");
               client.println("<img src = \"http://192.168.1.109:88/cgi-bin/CGIProxy.fcgi");
               client.println("fcgi?cmd=snapPicture2&usr=guest&pwd=2073&t=\" name=\"refresh\" id=\"refresh\" onload='reload(this)' onerror='reload(this)'>");
            //-----------------------------------------------------------------------------------------     
            //--------------------------------------------------------
            
          }
           
