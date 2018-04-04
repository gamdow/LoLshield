#include <Charliplexing.h>
#include <EEPROM.h>

int EEPROMaddress = 0; // Use non-volatile memory to store the on/off state
int toggleState = 0;
long count = 0L;
unsigned long elasped_time = 0L;
unsigned long wrap_time = 60 * 60 * 1000L; // During of a complete cycle (ms)
long frame_time = wrap_time / pow(2,14);

void setup() {
  toggleState = EEPROM.read(EEPROMaddress);
  EEPROM.write(EEPROMaddress, (toggleState + 1) % 2); // Toggle the on/off state with each reset
  LedSign::Init(GRAYSCALE);
  elasped_time = millis();
}

void loop() {
  switch(toggleState){
  case 0:
    for (byte line=0; line<9; ++line) {
      for (byte led=0; led<14; ++led) {
        if ((count-line) & (1<<led)) {
          LedSign::Set(led, line, 7 - line/1.5);
        } else {
          LedSign::Set(led, line, 0);
        }
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
    toggleState = 0;
  }
}
