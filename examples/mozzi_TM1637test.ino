#include <TM1637Display.h>

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin2048_int8.h>

#define DATAPIN 4
#define IRQPIN  3
#define AUDIOUT 9

#define CLK 1
#define DIO 2

TM1637Display display(CLK, DIO);
uint8_t globalcount = 0;
bool display_tone = true;
uint8_t segs [1] = {0x40};
Oscil <2048, AUDIO_RATE> osc = Oscil <2048, AUDIO_RATE>(SIN2048_DATA);
                   //b       c       d        e        f
float tones [5] = {493.883, 523.251, 587.33, 659.255, 698.456};
const uint8_t d_chr [5] = {0b01111100,    // b
                     0b00111001,    // C
                     0b01011110,    // d
                     0b01111001,    // E
                     0b01110001};   // F

void setup()
{
  display.setBrightness(0x0a);
  display.clear();
  display_tone = false;
  startMozzi(CONTROL_RATE);
  pinMode(AUDIOUT, OUTPUT);
  osc.setFreq(tones[0]);
}

void updateControl(){
  //only change screen when update() returns true
  if (display.update()) {
    if (display_tone) {
      //display the globalcount value
      display.showNumberDec(globalcount);
      //change the oscillator tone
      osc.setFreq(tones[globalcount%5]);
      display_tone = false;
    }
    else {
      //display the note being played (shows up pretty late)
      display.setSegments(&d_chr[globalcount%5],1,0);
      display_tone = true;
      globalcount++;
    }
  }
}

int updateAudio(){
  //shift right for lower volume
  return osc.next() >> 1;
}

void loop(){
  audioHook();
}

