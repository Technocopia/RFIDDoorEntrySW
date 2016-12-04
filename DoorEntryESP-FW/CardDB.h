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

typedef struct{
  unsigned int   rday:4;
  unsigned int   rtimein:6;
  unsigned int   rtimeout:6;
} timedur_s;

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
} card_record_s;

typedef union{
  card_record_s f;
  uint8_t       b[16];
} card_record;


uint8_t          dummyRead(uint16_t addr);
void          dummyWrite(uint16_t addr,uint8_t data);

card_record   dbGetCard(uint16_t index);
void          dbSetCard(card_record r, uint16_t index);




