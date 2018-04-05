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
ShellSort shellSort;
SelectionSort selectionSort;
InsertionSort insertionSort;
SortBase * sorters[] = {&bubbleSort, &cocktailSort, &combSort, &insertionSort, &selectionSort};
int num_sorters = 5;
SortBase * sort = 0;

void setup() {
  Serial.begin(9600);
  toggleState = EEPROM.read(EEPROMaddress);
  EEPROM.write(EEPROMaddress, (toggleState + 1) % 2); // Toggle the on/off state with each reset
  LedSign::Init(GRAYSCALE);
  elasped_time = millis();
  sort = sorters[0];
}

void display(long _delay) {
  for (uint8_t row = 0; row < N_ROWS; ++row) {
    for (uint8_t col = 0; col < N_COLUMNS; ++col) {
      uint8_t val = row == elements[col]
        ? writes[col] * led_on_high + (1.0f - writes[col]) * led_on
        : reads[col] * led_off_high + (1.0f - reads[col]) * led_off;
      LedSign::Set(N_COLUMNS - 1 - col, row, val);
    }
  }
  delay(_delay);
}

void loop() {
  switch(toggleState){
  case 0:
    decay_hightlights(0.5f);

    if(sort->GetFinished()) {
      display((write_time + read_time) * 10);
      sort = sorters[random(0, num_sorters)];
      sort->init();
      display((write_time + read_time) * 10);
    } else  {
      sort->step();
    }

    display(write_time * write_count + read_time * read_count);

    elasped_time = millis();
    break;
  case 1:
    while(true) {}
  default:
    EEPROM.write(EEPROMaddress, 0);
  }
}
