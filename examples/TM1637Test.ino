#include <TM1637Display.h>

#define CLK 1
#define DIO 2

TM1637Display display(CLK, DIO);
uint8_t globalcount = 0;
bool cleardisplay = true;

void setup()
{
  display.setBrightness(0x0a);
  display.clear();
  cleardisplay = false;
}

void loop(){
  if (display.update()) {
    if (cleardisplay) {
      delay(500);
      uint8_t segs [1] = {0x40};
      display.setSegments(segs,1,globalcount%4);
      cleardisplay = false;
    }
    else {
      delay(200);
      display.showNumberDec(globalcount);
      cleardisplay = true;
      globalcount++;
    }
  }
  delayMicroseconds(100);
}

