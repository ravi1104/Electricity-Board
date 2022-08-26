#include <ThingSpeak.h>
#include "EmonLib.h"
#include <WiFi.h>

const char ssid[] = "fdsfsdfsdf";
const char password[] = "ffsdfsd";
WiFiClient client;
EnergyMonitor emon;
#define vCalibration 100.8//106.8
#define currCalibration 10//0.52




const long CHANNEL =1111215;
const char *WRITE_API = "adsfdsfsfsd";
float kWh = 0;
unsigned long lastmillis = millis();
   
float a = 0.0;
float b = 0.0;
float c = 0.0;
float d = 0.0;
float e=0.0;
float f=215.6;
long prevMillisThingSpeak = 0;
int intervalThingSpeak = 200; // Minimum ThingSpeak write interval is 15 seconds
 

float avgload=0;
float curload=0;
float peakload=0;





void setup()
{
 Serial.begin(115200);
 Serial.println();
 Serial.println("Send Sensor's Data to ThingSpeak Using ESP32");
 Serial.println();
 emon.voltage(35, vCalibration, 1.0); // Voltage: input pin, calibration, phase_shift
  emon.current(34, currCalibration);
 WiFi.mode(WIFI_STA);
 ThingSpeak.begin(client); // Initialize ThingSpeak
}
 
void loop()
{
// Connect or reconnect to WiFi
 if (WiFi.status() != WL_CONNECTED) {
 Serial.print("Attempting to connect to SSID: ");
 Serial.println(ssid);
 while (WiFi.status() != WL_CONNECTED) {
 WiFi.begin(ssid, password);
 Serial.print(".");
 delay(5000);
 }
 Serial.println("\nConnected.");
 }
    emon.calcVI(20, 2000);
    
    Serial.print("\tIrms: ");
    Serial.print(((emon.Irms)), 2);
    Serial.print("A");
    
    
    
    Serial.print("\tkWh: ");
    kWh = kWh + emon.apparentPower*(millis()-lastmillis)/3600000000.0;
    Serial.print(kWh, 4);
    Serial.println("kWh");
    lastmillis = millis();
    
  //load factor
curload=kWh;
avgload=(curload+avgload)/2;
if(curload>peakload){peakload=curload;}



//Load Factor

     
if(emon.Vrms<60){a=0;}
 else{a=emon.Vrms;}      //voltage

     //current
  if(emon.Irms>30){
   b=0;
  }
  else{b=emon.Irms;}

Serial.println(emon.powerFactor);
 if(emon.powerFactor>0.70 &&  emon.powerFactor<1.00){
   c=emon.powerFactor;
    }
 else{c=0.999;}    //powerfactor
               
Serial.println(c);

 if(emon.Vrms=0){d=0;}
 else{d=avgload/peakload;}    //load

 e=((emon.Irms*emon.Vrms)*4.1)/60000;      //Bill
 f=f+e;
 Serial.print("Vrms: ");
    Serial.print(a);
    Serial.print("V");

    if(a<100){
      b=0;
    }
    else{b=b+1;}
    Serial.print("\tIrms: ");
    Serial.print(b);
    Serial.print("A");
 

   Serial.print("\tPowerFactor: ");
    Serial.println(c);
    Serial.print("\LoadFactor: ");          //load
Serial.println(d);
Serial.print("\Cost: ");
    Serial.print(f);
    Serial.println("Rs");
  
 // Set the fields with the values
 ThingSpeak.setField(1, a);               //voltage
 ThingSpeak.setField(2, b);               //current
 ThingSpeak.setField(3, c);                //powerfactor
 ThingSpeak.setField(4, d);                   //load
 ThingSpeak.setField(5, f);                    //Bill
 // Write to the ThingSpeak channel
 int x = ThingSpeak.writeFields(CHANNEL, WRITE_API);
 if (x == 200) {
 Serial.println("Channel update successful.");
 }
 else {
 Serial.println("Problem updating channel. HTTP error code " + String(x));
 }
 
 prevMillisThingSpeak = millis();
 
delay(3000);
 
}
