#include "CardDB.h"
#include <ArduinoJson.h>
#include <FS.h>



uint8_t dummyEEPROM[2048];

bool updateEEPROM(String cfg){
  File db = SPIFFS.open(cfg, "r");
  if (db==0) return false;

  // isolate a record. We can't hand whole json file to parser.
  bool morejson=true;
  int eeprecordindex=2;
  while(morejson){
    char buf[1024];
    int count=0;
    int start=0;
    int len=1;
    int bytesread = db.readBytes(buf,1024); // read 1k fo the file.
    int i=0;
    
    // Find the offset of the next json in the array and remember its offset.
    while(buf[i]!='{' && i<8192) i++;
    start=i;
  
    // begin counting {} to isolate whole object.
    for(int i=start; i<bytesread; i++){
      if (buf[i] == '{') count++;
      if (buf[i] == '}') count--;
      if (count==0) break;
      len++;
    }
    
    // read ahead a bit, are we at the end of the json array?
    for(int i=start+len; i<bytesread; i++){
      if (buf[i]==']') {
        morejson=false; // end of array
        break;
      }
      if (buf[i]==','){
        morejson=true;
        break;
      }
    }
  
    // We know know the start and size of the next record. isolate it and
    // feed it to the JSON parser as an object.
    
    db.seek((-bytesread)+start,SeekCur);// seek to start of object
    db.readBytes(buf,len); // read object into buffer
    buf[len+1]=0;// make sure it's a valid null terminated c string.
    
    // Parse object w json parser.
    DynamicJsonBuffer jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(buf);

    
      
    printCardInfo(root);

    card_record c;
    for(int i=0; i<16; i++) c.b[i]=0;
    
    c.f.magic='r';
    c.f.site=root["card"]["site"];
    c.f.id=  root["card"]["id"];
    for(int i=0; i<root["hours"][i].size(); i++){
    c.f.hours[i].f.rday=weekStringToNum((const char*)root["hours"][i]["day"]);
    c.f.hours[i].f.rtimein=(const int)root["hours"][i]["range"][0]/50;
    c.f.hours[i].f.rtimeout=(const int)root["hours"][i]["range"][1]/50;
    }

    printCardRecord(c);
    dbSetCard(c,eeprecordindex);
    eeprecordindex++;
  }
  db.close();
  int cc=0;
  for (int i=0; i<2046; i++) {
    print_hex(dummyEEPROM[i],8);
    Serial.print(" ");
    cc++;
    if (cc>15){
      Serial.println("");
      cc=0;
      }
    }
}

void printCardInfo(JsonObject& root){
  Serial.println("Card Record");
    Serial.print("  Name: ");
      Serial.print((const char*)root["name"]["first"]);
      Serial.println((const char*)root["name"]["last"]);
    Serial.print("  ID: ");
      Serial.print((const char*)root["card"]["site"]);
      Serial.print(":");
      Serial.println((const char*)root["card"]["id"]);
    Serial.println("  Hours");
    for (int i=0; i<root["hours"][i].size(); i++){
      Serial.print("    ");
      Serial.print((const char*)root["hours"][i]["day"]);
      Serial.print(": ");  
      Serial.print((const int)root["hours"][i]["range"][0]);
      Serial.print(" to ");
      Serial.println((const int)root["hours"][i]["range"][1]);
    }
    Serial.println("");
}

void printCardRecord(card_record c){
   Serial.println("EEPROM CardRecord");
   Serial.print("  magic: ");Serial.println(c.f.magic);
   Serial.print("  site:  ");Serial.println(c.f.site);
   Serial.print("  ID:    ");Serial.println(c.f.id);
 for(int i=0; i<4; i++){
   if(c.f.hours[i].f.rday==0) break;
   Serial.print("  Hours");
   Serial.print("    ");
  
   Serial.print(numtoWeekString(c.f.hours[i].f.rday));Serial.print(": ");
   Serial.print(c.f.hours[i].f.rtimein,HEX);Serial.print(" to ");
   Serial.println(c.f.hours[i].f.rtimeout,HEX);
  
 }
 Serial.println("");

 for (int i=0; i<16; i++) {Serial.print(c.b[i],HEX); Serial.print(" ");}
 Serial.println("");Serial.println("");
}

uint8_t weekStringToNum(const char* string){
  if(strcmp(string,"weekday")==0) return weekday;
  if(strcmp(string,"weekend")==0) return weekend;
  if(strcmp(string,"monday")==0) return monday;
  if(strcmp(string,"tuesday")==0) return tuesday;
  if(strcmp(string,"wednesday")==0) return wednesday;
  if(strcmp(string,"thursday")==0) return thursday;
  if(strcmp(string,"friday")==0) return friday;
  if(strcmp(string,"saturday")==0) return saturday;
  if(strcmp(string,"sunday")==0) return sunday;
  if(strcmp(string,"everyday")==0) return everyday;
  return 0;
}

char * numtoWeekString(uint8_t i){
  switch(i){
    case weekday:   return "weekday";
    case weekend:   return "weekend";
    case monday:    return "monday";
    case tuesday:   return "tuesday";
    case wednesday: return "wednesday";
    case thursday:  return "thursday";
    case friday:    return "friday";
    case saturday:  return "saturday";
    case sunday:    return "sunday";
    case everyday:  return "everyday";
  }
  
}

uint8_t dummyRead(uint16_t addr){
  if(addr>2047) return 0;
  return dummyEEPROM[addr];
  
}
void dummyWrite(uint16_t addr,uint8_t data){
  if(addr>2047) return;
  dummyEEPROM[addr]=data;
}

#define eepromRead  dummyRead
#define eepromWrite dummyWrite

card_record dbGetCard(uint16_t index){
  card_record card;
  for(int i=0; i<16; i++) card.b[i]=eepromRead((index*16)+i);
  
}
void dbSetCard(card_record r, uint16_t index){
  for(int i=0; i<16; i++) eepromWrite((index*16)+i,r.b[i]);  
}

void print_hex(int v, int num_places)
{
    int mask=0, n, num_nibbles, digit;

    for (n=1; n<=num_places; n++)
    {
        mask = (mask << 1) | 0x0001;
    }
    v = v & mask; // truncate v to specified number of places

    num_nibbles = num_places / 4;
    if ((num_places % 4) != 0)
    {
        ++num_nibbles;
    }

    do
    {
        digit = ((v >> (num_nibbles-1) * 4)) & 0x0f;
        Serial.print(digit, HEX);
    } while(--num_nibbles);

}

