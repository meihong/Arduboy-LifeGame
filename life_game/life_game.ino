#include <Arduboy2.h>
#include "life_game.h"
#include "bmp.h"

Arduboy2 arduboy;

#define STATUS_RUNNING      0
#define STATUS_INITIALIZING 1
#define STATUS_PAUSED       2
#define STATUS_SCREENSHOT   3
#define STATUS_RECV_WAIT    4
#define STATUS_RECEIVED     5

uint16_t generation;
uint8_t  running_status;

void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(30);
  arduboy.initRandomSeed();
  Serial.begin(9600);
  
  initialize();
  arduboy.display();
  running_status = STATUS_RUNNING;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (!(arduboy.nextFrame()))
    return;

  arduboy.pollButtons();
  if (arduboy.pressed(A_BUTTON)) {
    running_status = STATUS_INITIALIZING;
  } else if (arduboy.justPressed(B_BUTTON)) {
    running_status = running_status == STATUS_PAUSED ? STATUS_RUNNING : STATUS_PAUSED;
  } else if (running_status == STATUS_RUNNING && arduboy.pressed(RIGHT_BUTTON) && arduboy.pressed(LEFT_BUTTON)) {
    running_status = STATUS_SCREENSHOT;
  }

  switch (running_status) {
    case STATUS_INITIALIZING:
      initialize();
      running_status = STATUS_RUNNING;
      break;
    case STATUS_PAUSED:
      break;
    case STATUS_SCREENSHOT:
      sendBMP(arduboy.getBuffer());
      running_status = STATUS_RECV_WAIT;
      break;
    case STATUS_RECV_WAIT:
      if (waitRecvOk())
        running_status = STATUS_RECEIVED;
      break;
    case STATUS_RECEIVED:
      run();
      running_status = STATUS_RUNNING;
      break;
    case STATUS_RUNNING:
      run();
      break;
  }
}

void sendScreenshot() {
  Serial.write(arduboy.getBuffer(), WIDTH * HEIGHT / 8);
  running_status = STATUS_RECV_WAIT;
}

void initialize() {
  uint8_t* buffer = arduboy.getBuffer();

  arduboy.clear();
  for(uint16_t i = 0; i < MAX_HEIGHT * MAX_WIDTH / 8; i++) {
    *buffer = (uint8_t)random(255);
    buffer++;
  }
  generation = 0;
}

void run () {
  uint8_t  live[MAX_HEIGHT * MAX_WIDTH / 8];
  uint8_t* current = arduboy.getBuffer();

  for(uint16_t i = 0; i < MAX_HEIGHT * MAX_WIDTH / 8; i++) {
    live[i] = 0;
  }
  for(uint8_t y = 0; y < MAX_HEIGHT; y++) {
    evaluateVertical(current, live, y);
  }
  moveToNextGeneration(current, live);
  generation++;
  arduboy.setCursor(2, MAX_HEIGHT);
  arduboy.print(F("Gen# = "));
  arduboy.print(String(generation, DEC));
  arduboy.display();
}
