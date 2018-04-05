#include <Charliplexing.h>
#include <EEPROM.h>

#include "lolshield.h"
#include "sort.h"

int EEPROMaddress = 0; // Use non-volatile memory to store the on/off state
int toggleState = 0;
unsigned long elasped_time = 0L;
long read_time = 10L;
long write_time = 20L;

uint8_t const led_off = 0u;
uint8_t const led_on = 2u;
uint8_t const led_off_high = 1u;
uint8_t const led_on_high = 7u;

BubbleSort bubbleSort;
CombSort combSort;
CocktailSort cocktailSort;
SelectionSort selectionSort;
InsertionSort insertionSort;
ShellSort shellSort;
SortBase * sorters[] = {&bubbleSort, &cocktailSort, &combSort, &insertionSort, &shellSort, &selectionSort};
int const num_sorters = 6;
int sort_idx = num_sorters - 1;

void setup() {
  Serial.begin(9600);
  toggleState = EEPROM.read(EEPROMaddress);
  EEPROM.write(EEPROMaddress, (toggleState + 1) % 2); // Toggle the on/off state with each reset
  LedSign::Init(GRAYSCALE);
  elasped_time = millis();
}

void display(SortBase * sort, long _delay) {
  for (uint8_t row = 0; row < N_ROWS; ++row) {
    for (uint8_t col = 0; col < N_COLUMNS; ++col) {
      uint8_t val = row == sort->elements[col]
        ? sort->writes[col] * led_on_high + (1.0f - sort->writes[col]) * led_on
        : sort->reads[col] * led_off_high + (1.0f - sort->reads[col]) * led_off;
      LedSign::Set(N_COLUMNS - 1 - col, row, val);
    }
  }
  delay(_delay);
}

void loop() {
  switch(toggleState){
  case 0:
    sorters[sort_idx]->decayHightlights(0.5f);

    if(sorters[sort_idx]->getFinished()) {
      display(sorters[sort_idx], (write_time + read_time) * 10);
      sort_idx = (sort_idx + 1) % num_sorters;
      sorters[sort_idx]->init();
      display(sorters[sort_idx], (write_time + read_time) * 10);
    } else  {
      sorters[sort_idx]->step();
    }

    display(sorters[sort_idx], write_time * sorters[sort_idx]->write_count + read_time * sorters[sort_idx]->read_count);

    elasped_time = millis();
    break;
  case 1:
    while(true) {}
  default:
    EEPROM.write(EEPROMaddress, 0);
  }
}
