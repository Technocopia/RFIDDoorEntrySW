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

 Serial.println("Card Test");
 card_record c;
 for (int i=0; i<16; i++) c.b[i]=0;

 c.f.magic='r';
 c.f.site=17;
 c.f.id=765432;
 c.f.hours[0].f.rday=monday;
 c.f.hours[0].f.rtimein=7*2;
 c.f.hours[0].f.rtimeout=17*2;
 Serial.println(sizeof(timedur_s));

 updateEEPROM("/cards.json");

}


void loop(void){
  handleclient();
}
