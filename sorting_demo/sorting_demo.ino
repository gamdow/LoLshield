#include <Charliplexing.h>
#include <EEPROM.h>

#include "lolshield.h"
#include "sort.h"

int EEPROMaddress = 0; // Use non-volatile memory to store the on/off state
int toggleState = 0;
long count = 0L;
unsigned long elasped_time = 0L;
unsigned long wrap_time = 10 * 60 * 1000L; // During of a complete cycle (ms)
long frame_time = wrap_time / pow(2,14);

uint8_t const led_off = 0u;
uint8_t const led_on = 2u;
uint8_t const led_off_high = 1u;
uint8_t const led_on_high = 7u;

BubbleSort bubbleSort;
CocktailSort cocktailSort;
SelectionSort selectionSort;
InsertionSort insertionSort;
SortBase * sorters[] = {&insertionSort, &bubbleSort, &cocktailSort, &selectionSort};
SortBase * sort = 0;

void setup() {
  Serial.begin(9600);
  toggleState = EEPROM.read(EEPROMaddress);
  EEPROM.write(EEPROMaddress, (toggleState + 1) % 2); // Toggle the on/off state with each reset
  LedSign::Init(GRAYSCALE);
  elasped_time = millis();
  sort = &cocktailSort;
}

void loop() {
  switch(toggleState){
  case 0:
    decay_hightlights(0.5f);
    
    if(sort->GetFinished()) {
      sort = sorters[random(0, 4)];
      sort->init();  
    } else  {
      sort->step();
    } 

    for (uint8_t row = 0; row < N_ROWS; ++row) {
      for (uint8_t col = 0; col < N_COLUMNS; ++col) {
        uint8_t val = row < elements[col] 
          ? highlights[col] * led_on_high + (1.0f - highlights[col]) * led_on
          : highlights[col] * led_off_high + (1.0f - highlights[col]) * led_off;
        LedSign::Set(N_COLUMNS - 1 - col, row, val);
      }
    }

    count += 1L;
    if (count > pow(2,14)) count = 0L;
    delay(max(0, frame_time - long(millis() - elasped_time)));

    elasped_time = millis();
    break;
  case 1:
    while(true) {}
  default:
    EEPROM.write(EEPROMaddress, 0);
  }
}
