// #include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

 
ESP8266WebServer rest_server(4711);

void rest_handleClient() {
   rest_server.handleClient();
}

void rest_set_led_on() {
  digitalWrite(LED_ESP_BUILTIN, LED_ESP_ON);
  rest_server.send(200, "text/json", "{\"name\": \"LED turned on\"}");
}

void rest_set_led_off() {
  digitalWrite(LED_ESP_BUILTIN, LED_ESP_OFF);
  rest_server.send(200, "text/json", "{\"name\": \"LED turned off\"}");
}
 
// Define routing
void restServerRouting() {
    rest_server.on(F("/led_on"), HTTP_GET, rest_set_led_on);
    rest_server.on(F("/led_off"), HTTP_GET, rest_set_led_off);
}
 
// Manage not found URL
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += rest_server.uri();
  message += "\nMethod: ";
  message += (rest_server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += rest_server.args();
  message += "\n";
  for (uint8_t i = 0; i < rest_server.args(); i++) {
    message += " " + rest_server.argName(i) + ": " + rest_server.arg(i) + "\n";
  }
  rest_server.send(404, "text/plain", message);
}
 
void rest_setup() { 
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
      delay(1000); 
      Serial.println("Rest Server is waiting...");
  }
  Serial.println("");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
 
  // Activate mDNS this is used to be able to connect to the server
  // with local DNS hostmane lampe.local
  if (MDNS.begin("lampe")) {
    Serial.println("MDNS responder started");
  }
 
  // Set server routing
  restServerRouting();
  // Set not found response
  rest_server.onNotFound(handleNotFound);
  // Start server
  rest_server.begin();
  Serial.println("HTTP server started");
}