#ifndef LIFE_GAME_H
#define LIFE_GAME_H

#include <Arduboy2.h>

#define MAX_WIDTH  WIDTH
#define MAX_HEIGHT (HEIGHT - 8)

uint8_t extractVerticalPattern (uint8_t*, uint8_t, uint8_t);
bool evaluateVertical (uint8_t* current, uint8_t* live, uint8_t y);
bool evaluate (uint8_t* current, uint8_t x, uint8_t y);
void setAlive (uint8_t* live, uint8_t x, uint8_t y);
void moveToNextGeneration (uint8_t* buffer, uint8_t* live);

#endif

