#ifndef BMP_H
#define BMP_H

#include <arduino.h>

const unsigned char fileHeader[] = {
  'B',  'M',  0x3e, 0x04, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x3e, 0x00, 0x00, 0x00
};
const unsigned char bitmapInfo[] = {
  0x28, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00, 0x00,
  0x40, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x02, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0x00
};

void sendBMP(uint8_t* sBuffer);
boolean waitRecvOk(void);

#endif
