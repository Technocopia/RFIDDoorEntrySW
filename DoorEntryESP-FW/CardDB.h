#include <stdint.h>
#include <Arduino.h>
#include <ArduinoJson.h>

#define weekday   0x1
#define weekend   0x2
#define monday    0x3
#define tuesday   0x4
#define wednesday 0x5
#define thursday  0x6
#define friday    0x7
#define saturday  0x8
#define sunday    0x9
#define everyday  0xa



typedef struct{
  unsigned int   rday:4;
  unsigned int   rtimein:6;
  unsigned int   rtimeout:6;
}__attribute__((packed, aligned(1))) timedur_s;

typedef union{
  timedur_s   f;
  uint16_t    w;
} timedur;

typedef struct{
  uint8_t      magic;
  uint8_t      rsvd;
  uint16_t     site;
  uint32_t     id;
  timedur    hours[4];
}__attribute__((packed, aligned(1))) card_record_s;

typedef union{
  card_record_s f;
  uint8_t       b[16];
} card_record;

// Reader config section.
typedef struct{
  uint8_t   sitestart;
  uint8_t   sitelen;
  uint8_t   idstart;
  uint8_t   idlen;
  
  uint8_t   partype1;
  uint8_t   parbit1;
  uint8_t   parstart1;
  uint8_t   parlen1;
  
  uint8_t   partype2;
  uint8_t   parbit2;
  uint8_t   parstart2;
  uint8_t   parlen2;

  uint8_t   ignoreparity;
  uint8_t   ignoresite;
  uint8_t   ignoreid;
  uint8_t   rsvd1;

  timedur   openhours[8];
} __attribute__((packed, aligned(1))) reader_config_s;

typedef union{
  reader_config_s f;
  uint8_t  b[32];
} reader_config;



bool updateEEPROM(String cfg);
bool updateEEPROMConfig(String cfg);
void printCardInfo(JsonObject& root);
void printCardConfigInfo(JsonObject& root);

void dumpEEPROM();
void printCardRecord(card_record c);
uint8_t weekStringToNum(const char* string);
char * numtoWeekString(uint8_t i);

uint8_t          dummyRead(uint16_t addr);
void          dummyWrite(uint16_t addr,uint8_t data);

card_record   dbGetCard(uint16_t index);
void          dbSetCard(card_record r, uint16_t index);

void print_hex(int v, int num_places);




