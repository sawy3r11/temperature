/*-------WIFI-----------*/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
const char* ssid="";
const char* password = "";
const String SERVER2 = "http://51.77.158.65:8081";
const String ACCESS_TOKEN = "5fBuGZf98JmzDVzs";
long SENSOR_ID = 1;
int connectFailure = 0;
const int CONNECTION_FAILURE_MAX = 20;
long wifiStrength = 0;
const int REQEST_DELAY=5;
int reqCounter = 0;
int httpResult = -1;
WiFiClient wifiClient;
/*-------//WIFI-----------*/

#include <OneWire.h>
#include <DallasTemperature.h>

/*-----SENSOR TEMPERATURE-------*/
const int temperaturePIN = 2;
OneWire oneWire( temperaturePIN );
DallasTemperature sensors(&oneWire);
 /*
  T1-> GŁOWICA
  T2 -> ZBIORNIK
  T3 -> 10-PÓŁKA
  T4 -> FAJKA
 */
float T1 = 0.0;
float T2 = 0.0;
float T3 = 0.0;
float T4 = 0.0;
/*-----//SENSOR TEMPERATURE-------*/

/*----------DISPLAY------*/
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
LiquidCrystal_I2C lcd(0x27,20,4);
String lcdLabel_1 = "Conn to WIFI...";
String lcdLabel_2 = "";
/*----------//DISPLAY------*/

int state = 1;
bool connectedToWifi = false;

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();

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

      T1 = sensors.getTempCByIndex(3); // GLOWICA
      T2 = sensors.getTempCByIndex(0); // ZBIORNIK
      T3 = sensors.getTempCByIndex(2); // 10-POLKA
      T4 = sensors.getTempCByIndex(1); // FAJKA

      lcdLabel_1 = ""+ String((int)T4) +" "+ String(T1)+" ["+ String(T3)+"]";
      Serial.println(lcdLabel_1);
      if(WiFi.status() == WL_CONNECTED){
        lcdLabel_2 = "Z"+String(T2) + " S:"+String(wifiStrength)+" "+String(reqCounter);
        if(reqCounter==0){
          httpResult = sendAddTemperatureRequest(T1, T2, T3, T4);
        }else if( reqCounter > REQEST_DELAY){
          reqCounter = -1;
        }
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

int sendAddTemperatureRequest( float t1, float t2, float t3, float t4 ){
  String json = "";
  StaticJsonDocument<200> reading;
  reading["sensorId"] = SENSOR_ID;
  reading["t1"] = t1;
  reading["t2"] = t2;
  reading["t3"] = t3;
  reading["t4"] = t4;
  serializeJson(reading, json);

  HTTPClient http2;
  String mapping = SERVER2+"/api/sensor/temperature/1";
  http2.begin(wifiClient, mapping);
  http2.addHeader("Content-Type", "application/json");
  int responseCode = http2.POST(json);
  http2.end();

  return responseCode;
}