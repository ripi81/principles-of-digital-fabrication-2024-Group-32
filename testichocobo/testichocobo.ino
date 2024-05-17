#include "MusicWithoutDelay.h"
#include "Tone.h"

const char song[] PROGMEM =  ":o=6:4d, 8b-1, 8g-1, 8e-1, 8d, 8b-1, 8g-1, 4b-1, 4g-1, 4.b-1, 8a-1, 8g-1, 16g-1, 16a-1, 8g-1, 8f-1, 4.g-1, 8f-1, 8g-1, 16g-1, 16b-1, 8d, 8e, 2f";
const char song2[] PROGMEM = ":o=3:2g, 2f, 2g, 2f, 2g, 2f, 2g, 2f";
const char song3[] PROGMEM = "::8p, 8g, 8g, 8g, 8p, 8f, 8f, 8f, 8p, 8g, 8g, 8g, 8p, 8f, 8f, 8f, 8p, 8g, 8g, 8g, 8p, 8f, 8f, 8f, 8p, 8g, 8g, 8g, 8p, 8f, 8f, 8f";

MusicWithoutDelay sound1(song);
MusicWithoutDelay sound2(song2);
MusicWithoutDelay sound3(song3);

void setup() {
  sound1.begin(SQUARE, ENVELOPE0, 0);
  sound2.begin(SQUARE, ENVELOPE0, 0);
  sound3.begin(SQUARE, ENVELOPE0, 0);
}

void loop() {
  sound1.setBPM(140);
  sound2.setBPM(140);
  sound3.setBPM(140);

  sound1.setVolume(127);
  sound2.setVolume(127);
  sound3.setVolume(127);

  sound1.update();
  sound2.update();
  sound3.update();
}
