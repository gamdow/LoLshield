#include <Arduino.h>
#include <Charliplexing.h>
#include <EEPROM.h>

#include "demo_base.hpp"
#include "binary_clock.hpp"
#include "sorting.hpp"

int const toggleAddr = 0;
int const stateAddr = 1;
unsigned long elasped_time = 0L;
float elasped_remainder = 0.f;
DemoBase * demo = 0;

void setup() {
  // Serial.begin(9600);
  int toggle = EEPROM.read(toggleAddr) % 1;
  EEPROM.write(toggleAddr, toggle + 1);
  if(toggle == 0) {
    int state = EEPROM.read(stateAddr);
    switch(state) {
      default: state = 0; // fall-through
      case 0: demo = new BinaryClock(); break;
      case 1: demo = new Sorting(); break;
    }
    EEPROM.write(stateAddr, state + 1);
  }
  LedSign::Init(GRAYSCALE);
  elasped_time = millis();
}

void loop() {
  if(demo != 0) {
    demo->update();
    demo->display();
    float frame_time = demo->getFrameMS() + elasped_remainder;
    elasped_remainder = frame_time - floor(frame_time);
    delay(max(0, floor(frame_time) - long(millis() - elasped_time)));
    elasped_time = millis();
  } else {
    exit(0);
  }
}
