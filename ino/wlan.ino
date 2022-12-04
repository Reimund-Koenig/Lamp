#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>          // https://github.com/tzapu/WiFiManager
 
// Set web server port number to 80
WiFiServer server(80);
WiFiClient net;

 
int WiFiConnRetry=0; // Zähler für WiFi Connect Retrys
int WiFiConnMax=100; // Max Anzahl WiFi Verbindungsversuche
 
//flag for saving data
bool shouldSaveConfig = false;
 
//callback notifying us of the need to save config
void saveConfigCallback () {
  Serial.println("Should save config");
  shouldSaveConfig = true;
}
 
void wlan_setup() { 
 
  // WiFiManager
  // Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wifiManager;
 
  //set config save notify callback
  wifiManager.setSaveConfigCallback(saveConfigCallback);
 
  // set custom ip for portal
  //wifiManager.setAPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
  
  // Uncomment and run it once, if you want to erase all the stored information
  //wifiManager.resetSettings();
 
  //set minimum quality of signal so it ignores AP's under that quality
  //defaults to 8%
  //wifiManager.setMinimumSignalQuality();
 
  //sets timeout until configuration portal gets turned off
  //useful to make it all retry or go to sleep
  //in seconds
  wifiManager.setTimeout(180);
 
  // fetches ssid and pass from eeprom and tries to connect
  // if it does not connect it starts an access point with the specified name
  // here  "AutoConnectAP"
  // and goes into a blocking loop awaiting configuration
  wifiManager.autoConnect("lampe");  // Normale Routine
  // or use this for auto generated name ESP + ChipID
  // wifiManager.autoConnect();
 
  // Start WiFi Manager mit Timeout Überprüfung. Nach Timeout... Tiefschlaf
  if (!wifiManager.autoConnect("lampe")) {
    Serial.println("failed to connect and hit timeout");
    delay(3000);
    //reset and try again, or maybe put it to deep sleep
    ESP.deepSleep(0); // sleep forever
  }
 
  // if you get here you have connected to the WiFi
  Serial.println("Connected.");
 
  WiFi.softAPdisconnect (true); // turns off Access Point visibility in net neighbourhood
 
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  //++++++++++++++++++++++++Sensor activity & MQTT publishing++++++++++++++++++++
  //+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
 
 //WiFi.begin(ssid, password); // not needed! WiFi Manager's job!
 // Nachfolgende WHILE Schleife wird benötigt, um sauber mit dem Router zu connecten. 
 // Insbesondere die Fritz!Box hat sonst manchmal Probleme beim Aufwachen aus dem Stromsparmodus. 
   while (WiFi.status() != WL_CONNECTED) {
      delay(100); 
      Serial.print(".");
      WiFiConnRetry++;
      if (WiFiConnRetry >= WiFiConnMax){
         Serial.println("No WiFi Connect, sleep 20 sec");
         delay(1000);
         ESP.deepSleep(20e6);
       }
   }
}