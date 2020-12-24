/*-------WIFI-----------*/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid="xxx"; 
const char* password = "xxx";
const String SERVER = "xxx";
long SENSOR_ID = 1;
int connectFailure = 0;
const int CONNECTION_FAILURE_MAX = 20;
long wifiStrength = 0;
const int REQEST_DELAY=5;
int reqCounter = 0;
int httpResult = -1;
/*-------//WIFI-----------*/
 
#include <OneWire.h>
#include <DallasTemperature.h>
 
/*-----SENSOR TEMPERATURE-------*/
const int temperaturePIN = 2;
OneWire oneWire( temperaturePIN ); 
DallasTemperature sensors(&oneWire); 
 
float T1 = 0.0;
float T2 = 0.0;
/*-----//SENSOR TEMPERATURE-------*/
 
/*----------DISPLAY------*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
String lcdLabel_1 = "Conn to WIFI...";
String lcdLabel_2 = "";
/*----------//DISPLAY------*/
 
int state = 1;
bool connectedToWifi = false;
 
void setup() {
  Serial.begin(9600);
  lcd.begin();
 
  display("Conn to WIFI...", "00000000000");
  WiFi.begin(ssid,password);
  while( WiFi.status() != WL_CONNECTED && connectFailure<CONNECTION_FAILURE_MAX){
      connectFailure++;
      display("Conn to WIFI...", "FailNo: "+String(connectFailure));
      delay(500);   
  }
  if(WiFi.status() == WL_CONNECTED){
     connectedToWifi = true;
  }
}
 
void loop() {
 
   switch(state){
    case 1: 
      state = 2;
      break;
    case 2: 
      sensors.requestTemperatures();
      T1 = sensors.getTempCByIndex(0);
      T2 = sensors.getTempCByIndex(1);
      lcdLabel_1 = "T1="+ String(T1) +"T2="+ String(T2);
      Serial.println(lcdLabel_1);
      if(WiFi.status() == WL_CONNECTED){
        lcdLabel_2 = "W S:"+String(wifiStrength);
        if(reqCounter==0){
          httpResult = sendAddTemperatureRequest(T1, T2);
        }else if( reqCounter > REQEST_DELAY){
          reqCounter = -1;
        }
        lcdLabel_2 +=" R:"+String(httpResult    )+" "+reqCounter;
        reqCounter++;
        
        wifiStrength = WiFi.RSSI();
  
        
      }else{
        lcdLabel_2 = "WIFI:FAIL";
      }
      display(lcdLabel_1, lcdLabel_2);
      delay(1000); 
      break;
   }
}
 
void display(String label_1, String label_2){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(label_1);
  lcd.setCursor(0,1);
  lcd.print(label_2);
}
 
int sendAddTemperatureRequest( float t1, float t2 ){
  String json = "";
  StaticJsonDocument<200> reading;
  reading["sensorId"] = SENSOR_ID;
  reading["t1"] = t1;
  reading["t2"] = t2;
  serializeJson(reading, json);
  
  String mapping = SERVER+"/sensor/temperature/"+String(SENSOR_ID);
  HTTPClient http;
  http.begin(mapping);  
  http.addHeader("Content-Type", "application/json");
  int responseCode = http.POST(json);
  http.end(); 
  return responseCode;
}