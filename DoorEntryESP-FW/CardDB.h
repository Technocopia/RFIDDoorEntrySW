#include <stdint.h>

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

uint8_t dummyRead(uint16_t addr);
void dummyWrite(uint16_t addr);

uint32_t DB_getSite(uint16_t index,uint8_t siteb,uint8_t idb);
uint32_t DB_getID(uint16_t index,uint8_t siteb,uint8_t idb);
uint8_t  DB_getTime(uint16_t index,uint8_t slot);
uint8_t  DB_getDay(uint16_t index,uint8_t slot);

uint32_t DB_settSite(uint32_t site, uint16_t index,uint8_t siteb,uint8_t idb);
uint32_t DB_setID(uint32_t id, uint16_t index,uint8_t siteb,uint8_t idb);
uint8_t  DB_setTime(uint8_t itime,uint16_t index,uint8_t slot);
uint8_t  DB_setDay(uint8_t iday,uint16_t index,uint8_t slot);



