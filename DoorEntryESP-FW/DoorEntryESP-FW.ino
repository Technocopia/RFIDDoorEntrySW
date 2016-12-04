#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include "wifi-pass.h"
#include <FS.h>
#include "httpcfg.h"
#include "CardDB.h"

//const char* ssid = "........";
//const char* password = "........";





const int led = 13;

void setup(void){
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

 initHTTP();
 Serial.println("HTTP server started");
}

void loop(void){
  handleclient();
}
