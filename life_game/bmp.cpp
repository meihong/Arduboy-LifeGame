#include <Arduboy2.h>
#include "bmp.h"

void sendBMP (uint8_t* sBuffer) {
  uint8_t  mask  = 0x80;
  uint8_t  datum = 0;
  uint8_t* ptr;
  uint8_t* startAddress;

  Serial.write(fileHeader, 14);
  Serial.write(bitmapInfo, 48);
  for (uint8_t y = HEIGHT - 1; y < HEIGHT; y--) {
    if (y & 0x07 == 0x07)
      startAddress = sBuffer + (y >> 3) * WIDTH;
    ptr = startAddress;
    for (uint8_t x = 0; x < WIDTH; x++) {
      datum *= 2;
      if (*ptr & mask) datum |= 1;
      if (x % 8 == 7) {
        Serial.write(datum);
        datum = 0;
      }
      ptr++;
    }
    mask = mask >> 1;
    if (!mask)
      mask = 0x80;
  }
}

boolean waitRecvOk () {
  uint8_t incoming;
  
  if (Serial.available() > 0) {
    incoming = Serial.read();
    if (incoming == 'K' || incoming == 'k')
      return true;
  }
  return false;
}
