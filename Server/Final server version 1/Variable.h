 
//---- mapping between contoller and electrical component
//---- pin5, 7, 10, 50, 51, and 52, 53 can not be used
//---- pin5 for wifi101 reset, pin7 for wifi101 handshake should not be used
//---- pins 50, 51, and 52 on the Mega. On both boards, pin 10 is used as SS. On the Mega, 
//---- the hardware SS pin, 53, is not used but it must be kept as an output or the SPI interface won't work.
//---- I2C: pin20(SDA) pin21(SCL)

//---- we have a bank of 8 relay: the corresponding pins in order from left to rgiht are:
//---- digital pin 23-49 for output:
//---- 110V: K1: homeLight pin:25, K2: garageLight pin:27 
//-----K3: undefined pin:29, K4: undefined pin:31
//---- K5: undefined pin:33, K6: sprinkler pin:35, K7: garageLight2 pin:37, K8: garageDoor pin:39
//---- output pins: component
int garageDoor = 39;
int garageLight = 29; // 110V
int garageLight2 = 37;
int sprinkler = 35;
int homeLight = 25;   // 110Vb

//---- digital pin 22-48 for input:
//---- input pins: sensors, flags

int DoorClose = 0; //push1; 
int DoorOpen = 0; //push2; ;
int garageDoorDisTrig = 40 ;
int garageDoorDisEcho = 42 ;
int carDisTrig = 44 ;
int carDisEcho = 46 ;
int homeTemp = 22 ;

//---- analog pin
int homelightACSensor = A0 ;
int homelightBritness = A1 ;
int homelightMotion = A2 ;
int sprinklerACSensor = A4 ;
int sprinklerHumidity = A5 ;
int garageMotion = A10;
int garageBritness = A11;
int garageLightACSensor = A14 ;

//---- other------------------------------------------------
long carDuration=0, carCm=0, garageDuration=0, garageCm=0;
int cnt=0;
int garageDoor_flag = 0;
int garage_small_light_flag = 0;
int garage_light_flag = 0;
int homelight_flag = 0;
int sprinkler_flag = 0;
int AC_heating_flag = 0;
int AC_heating_relay = 2; // defult 2-- do nothing, 0-- off, 1--on, 
int AC_cooling_flag = 0;
int AC_cooling_relay = 2;// defult 2-- do nothing, 0-- off, 1--on, 
int temperature = 0;
int homeBritness = 0;
int homeMotion = 0;
int garageMotion_reading = 0;
int garageBritness_reading = 0;
int soil_humidity = 0;
String ON = "on";
String OFF = "off";

//-----------temperature set up ------------------------
#define DS18B20 22
OneWire ourWire(DS18B20);
DallasTemperature sensors(&ourWire);

//----------AC Control ---------------------------------
int AC_CoolDown_flag =0;
volatile unsigned int AC_CoolDown = 0;
int AC_CoolDown_Diff = 0; // AC_CoolDown - timer
//------------Timer counter --------------------------
volatile unsigned int Timer5Counter = 0;
int carState = 0;
unsigned int LightAutoCounter = 0;
unsigned int LightAutoCounter2 = 100;
int LightBlinkCounter = 0;
int Car_dis_cnt = 0;
int Car_dis_sum = 0;
int Car_dis_avg = 0;

//------Home light auto----------------------------------------------
int homeLightState = 0;
unsigned int HomeLightAutoCounter = 0;
volatile unsigned int motionCounter = 0;
volatile unsigned int motionCounterFlag = 0;

//-----EEprom Address -----------------------------------------
int AC_CoolDown_flag_address = 0;
int AC_CoolDown_Diff_address = 16;

int AC_heating_relay_address = 48;
int AC_cooling_relay_address = 56;

//-----Auto Lock-------------
unsigned int maindoor_timer = 0;
int maindoor_Autolock_flag = 0;
unsigned int garagedoor_timer = 600; // the reason to set up garagedoor_timer = 600 because: in the initial state, we need to make sure it does not trig Auto lock function.
