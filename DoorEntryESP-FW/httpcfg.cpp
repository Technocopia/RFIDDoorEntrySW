#include <ESP8266WiFi.h>
#include "httpcfg.h"
#include <ESP8266WebServer.h>
#include <Arduino.h>
#include <FS.h>

ESP8266WebServer server(80);
File fsUploadFile;

bool handleFileRead(String path){
  Serial.println("handleFileRead: " + path);
  if(path.endsWith("/")){
    if(SPIFFS.exists("/index.htm") | SPIFFS.exists("/index.htm.gz")) path += "index.htm";
    else if(SPIFFS.exists("/index.html") | SPIFFS.exists("/index.html.gz")) path += "index.html";
    else return false;
  }
  String contentType = getContentType(path);
  String pathWithGz = path + ".gz";
  if(SPIFFS.exists(pathWithGz) || SPIFFS.exists(path)){
    if(SPIFFS.exists(pathWithGz))
      path += ".gz";
    File file = SPIFFS.open(path, "r");
    size_t sent = server.streamFile(file, contentType);
    file.close();
    return true;
  }
  return false;
}

//format bytes
String formatBytes(size_t bytes){
  if (bytes < 1024){
    return String(bytes)+"B";
  } else if(bytes < (1024 * 1024)){
    return String(bytes/1024.0)+"KB";
  } else if(bytes < (1024 * 1024 * 1024)){
    return String(bytes/1024.0/1024.0)+"MB";
  } else {
    return String(bytes/1024.0/1024.0/1024.0)+"GB";
  }
}

String getContentType(String filename){
  if(server.hasArg("download")) return "application/octet-stream";
  else if(filename.endsWith(".htm")) return "text/html";
  else if(filename.endsWith(".html")) return "text/html";
  else if(filename.endsWith(".css")) return "text/css";
  else if(filename.endsWith(".js")) return "application/javascript";
  else if(filename.endsWith(".png")) return "image/png";
  else if(filename.endsWith(".gif")) return "image/gif";
  else if(filename.endsWith(".jpg")) return "image/jpeg";
  else if(filename.endsWith(".ico")) return "image/x-icon";
  else if(filename.endsWith(".xml")) return "text/xml";
  else if(filename.endsWith(".pdf")) return "application/x-pdf";
  else if(filename.endsWith(".zip")) return "application/x-zip";
  else if(filename.endsWith(".gz")) return "application/x-gzip";
  return "text/plain";
}

// called from the main loop.
void handleclient(){
  server.handleClient();
}

// write config options JSON to ffs.
void adminJSON(){
   Serial.printf(server.arg("plain").c_str());
   fsUploadFile = SPIFFS.open("/admin.json", "w+");
   fsUploadFile.println(server.arg("plain"));
   fsUploadFile.close();
   
  server.send ( 200, "text/json", "{success:true}" );
}

bool initHTTP(){

  // init FS.
  SPIFFS.begin();

  // List FS contents on boot
  {
    Dir dir = SPIFFS.openDir("/");
    while (dir.next()) {    
      String fileName = dir.fileName();
      size_t fileSize = dir.fileSize();
      Serial.printf("FS File: %s, size: %s\n", fileName.c_str(), formatBytes(fileSize).c_str());
    }
    Serial.printf("\n");
  }

   //called when the url is not defined here
   //use it to load content from SPIFFS
   server.onNotFound([](){
    
    if(!handleFileRead(server.uri()))
      server.send(404, "text/plain", "FileNotFound");
  });

  // web UI will POST new config json
  server.on("/admin.json", HTTP_POST, adminJSON);

  // hack. spiffs won't do that many nested directories and I dont want to modify CSS.
  server.on("/semantic/themes/default/assets/fonts/icons.woff2", HTTP_GET,[](){ handleFileRead("/semantic/fonts/icons.woff2"); });
  server.on("/semantic/themes/default/assets/fonts/icons.otf",   HTTP_GET,[](){ handleFileRead("/semantic/fonts/icons.otf"); });
  server.on("/semantic/themes/default/assets/fonts/icons.svg",   HTTP_GET,[](){ handleFileRead("/semantic/fonts/icons.svg"); });
  server.on("/semantic/themes/default/assets/fonts/icons.ttf",   HTTP_GET,[](){ handleFileRead("/semantic/fonts/icons.ttf"); });
  server.on("/semantic/themes/default/assets/fonts/icons.eot",   HTTP_GET,[](){ handleFileRead("/semantic/fonts/icons.eot"); });
  server.on("/semantic/themes/default/assets/fonts/icons.woff",  HTTP_GET,[](){ handleFileRead("/semantic/fonts/icons.woff"); });
  
  server.begin();
  
}


