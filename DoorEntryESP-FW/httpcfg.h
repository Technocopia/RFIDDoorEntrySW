#include <ESP8266WebServer.h>
#include <Arduino.h>



bool handleFileRead(String path);
String formatBytes(size_t bytes);
String getContentType(String filename);
bool initHTTP();
void handleclient();
