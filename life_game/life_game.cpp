#include "life_game.h"

uint8_t extractVerticalPattern (uint8_t* current, uint8_t x, uint8_t y) {
  uint8_t* ptr = current + y / 8 * MAX_WIDTH + x;
  uint16_t pattern;
  uint8_t  mod = y & 7;

  if (y == 0) {
    return (*ptr & 0x03) * 2;
  } else if (mod == 0) {
    return (uint8_t)(((uint16_t)*(ptr - MAX_WIDTH) + (uint16_t)*ptr * 0x100) * 2 / 0x100) & 0x07;
  } else {
    if (mod == 7 && y != MAX_HEIGHT - 1) {
      pattern = (uint16_t)(*(ptr + MAX_WIDTH)) * 0x100 + (uint16_t)*ptr;
    } else {
      pattern = (uint16_t)*ptr;
    }
    return (uint8_t)(pattern >> (mod - 1)) & 0x07;
  }
}

bool evaluateVertical (uint8_t* current, uint8_t* live, uint8_t y) {
  const uint8_t scoreTable[] = { 0, 1, 1, 2, 1, 2, 2, 3 };
  uint8_t verticals[MAX_WIDTH];
  uint8_t count;

  for (uint8_t x = 0; x < MAX_WIDTH; x++) {
    verticals[x] = extractVerticalPattern(current, x, y);
  }

  for (uint8_t x = 0; x < MAX_WIDTH; x++) {
    count = 0;
    // the left side of the designated designated horizontal axis;
    if (x > 0) {
      count += scoreTable[verticals[x - 1]];
    }
    // the designated designated horizontal axis;
    count += scoreTable[verticals[x] & 0x05];
 
    // the right side of the designated horizontal axis;
    if (x < MAX_WIDTH - 1) {
      count += scoreTable[verticals[x + 1]];
    }

    if (verticals[x] & 0x02) {
      if (count == 2 || count == 3)
        setAlive(live, x, y);
    } else {
    // currently dead
      if (count == 3)
        setAlive(live, x, y);
    }
  }
}

bool evaluate (uint8_t* current, uint8_t x, uint8_t y) {
  const uint8_t scoreTable[] = { 0, 1, 1, 2, 1, 2, 2, 3 };
  uint8_t vPattern;
  uint8_t count    = 0;

  // the left side of the designated designated horizontal axis;
  if (x > 0) {
    count += scoreTable[extractVerticalPattern(current, x - 1, y)];
  }
  // the designated designated horizontal axis;
  vPattern = extractVerticalPattern(current, x, y);
  count += scoreTable[vPattern & 0x05];
 
  // the right side of the designated horizontal axis;
  if (x < MAX_WIDTH - 1) {
    count += scoreTable[extractVerticalPattern(current, x + 1, y)];
  }

  if (vPattern & 0x02) {
    if (count == 2 || count == 3)
      return true;
    else
      return false;
  } else {
    // currently dead
    if (count == 3)
      return true;
    else
      return false;
  }
}

void setAlive (uint8_t* live, uint8_t x, uint8_t y) {
  uint16_t ptr  = y / 8 * MAX_WIDTH + x;
  uint8_t  mask = _BV(y % 8);

  live[ptr] = live[ptr] | mask;
}

void moveToNextGeneration (uint8_t* buffer, uint8_t* live) {
  for (uint16_t i = 0; i < MAX_HEIGHT * MAX_WIDTH / 8; i++) {
    *(buffer + i) = *(live + i);
  }
}

