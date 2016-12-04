#include "CardDB.h"


uint8_t dummyEEPROM[2048];
uint8_t dummyRead(uint16_t addr){
  if(addr>2047) return 0;
  
}
void dummyWrite(uint16_t addr){
  if(addr>2047) return;
  
}

void getCardRecord(uint16_t index){
  
}

