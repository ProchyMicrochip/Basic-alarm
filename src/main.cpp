#include <Arduino.h>
#include "program.h"
#if defined(ESP32)
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <Logging/LoggerFactory.h>
#include <Logging/Logger.h>
#include <EEPROM.h>

#define EEPROM_SIZE sizeof(byte) * 1024

//#include "WiFiManager.h"
const char* ssid = "OTA1";
const char* password = "nsvbnc170";
Program program;
LoggerFactory factory;
Logger OTAlogger("OTA");

bool initWiFi(int eeprom_addrs, int timeout){
    //String ssid = EEPROM.readString(eeprom_addrs);
    String ssid = "IOT";
    if(ssid.length() > 30) return false;
    for (int i = 0; i < ssid.length(); i++) {
        if (ssid[i] < 32 || ssid[i] > 127){
          WiFi.begin("dummy","dummy");
        return false;
        }
          
    }

    //String pass = EEPROM.readString(eeprom_addrs+sizeof(char)*20);
    String pass = "septikjenaprd";
    for (int i = 0; i < pass.length(); i++) {
      if (pass[i] < 32 || pass[i] > 127){
        WiFi.begin("dummy","dummy");
        return false;
      }
    }
    if(pass.length() > 30) return false;
    Serial.print("Connecting to: ");
    Serial.println(ssid);
    Serial.println(pass);
    //String ssid = "OTTOTEST";
    //String pass = "NOT_FOUND";
    WiFi.begin(ssid.c_str(),pass.c_str());
    unsigned long currentMillis = millis();
    unsigned long previousMillis = currentMillis;

  while(WiFi.status() != WL_CONNECTED) {
    currentMillis = millis();
    if (currentMillis - previousMillis >= timeout) {
      Serial.println("Failed to connect.");
      return false;
    }
    delay(20);
  }
  Serial.print("Connected ip: ");
  Serial.println(WiFi.localIP());
  return true;
}
void setup() {
  EEPROM.begin(EEPROM_SIZE);
  factory.Begin();
  OTAlogger.Iniciialize(factory);
  Serial.begin(115200);
  //factory.Info("Main","Booting");
  bool sta = initWiFi(0,10000);




  
  // Port defaults to 3232
  // ArduinoOTA.setPort(3232);

  // Hostname defaults to esp3232-[MAC]
  // ArduinoOTA.setHostname("myesp32");

  // No authentication by default
  // ArduinoOTA.setPassword("admin");

  // Password can be set with it's md5 value as well
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");
if(sta){
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      //OTAlogger.Debug("Start updating " + type);
    });
    ArduinoOTA
    .onEnd([]() {
      //OTAlogger.Debug("\nEnd");
    });
    ArduinoOTA
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
    });
    ArduinoOTA
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) OTAlogger.Error("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) OTAlogger.Error("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) OTAlogger.Error("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) OTAlogger.Error("Receive Failed");
      else if (error == OTA_END_ERROR) OTAlogger.Error("End Failed");
    });

  ArduinoOTA.begin();
 // OTAlogger.Info("IP address: ");
  //OTAlogger.Info(WiFi.localIP().toString());
   //OTAlogger.Info("Ready");
}
else{
  WiFi.disconnect();
  String ssid ="OTTO-"+ WiFi.macAddress();
  WiFi.softAP(ssid.c_str(), "ottobyprochy");
  //OTAlogger.Warning("OTA not available in AP mode");
}
  program.setup(factory);
}

void loop() {
  //ArduinoOTA.handle();
  program.loop();
}