///#include <Arduino.h>
#include <Charliplexing.h>
#include <EEPROM.h>

#include "demo_base.hpp"
#include "binary_clock.hpp"
#include "sorting.hpp"
//#include "little_man_computer.hpp"
#include "minimal_instruction_set_computer.hpp"

int const TOGGLE_ADDRESS = 0;
int const STATE_ADDRESS = 1;
unsigned long const DEMO_DURATION = 60 * 60 * 1000L; // 1 hour
unsigned long last_demo_change = 0L;
unsigned long last_frame_update = 0L;
float elasped_remainder = 0.f;
DemoBase * demo = 0;

void nextDemo() {
  delete demo;
  int state = EEPROM.read(STATE_ADDRESS);
  switch(state) {
    default: state = 0; // fall-through
    case 0: demo = new BinaryClock(); break;
    case 1: demo = new Sorting(); break;
    case 2: demo = new MinimalInstructionSetComputer(); break;
    // case 3: demo = new LittleManComputer(); break;
  }
  EEPROM.write(STATE_ADDRESS, state + 1);
  last_demo_change = millis();
}

void setup() {
  //Serial.begin(9600);
  // int toggle = EEPROM.read(TOGGLE_ADDRESS) % 2;
  // EEPROM.write(TOGGLE_ADDRESS, toggle + 1);
  // if(toggle == 0) {
    nextDemo();
  // }
  LedSign::Init(GRAYSCALE);
  last_frame_update = millis();
}

void loop() {
  if(demo == 0) {
    exit(0);
  }

  demo->update();
  demo->display();
  float frame_time = demo->getFrameMS() + elasped_remainder;
  elasped_remainder = frame_time - floor(frame_time);
  delay(max(0, floor(frame_time) - long(millis() - last_frame_update)));
  last_frame_update = millis();

  if(millis() - last_demo_change >= DEMO_DURATION) {
    nextDemo();
  }
}
