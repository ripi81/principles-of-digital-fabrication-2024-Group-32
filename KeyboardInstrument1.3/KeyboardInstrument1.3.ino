#include "MusicWithoutDelay.h"
#include "Tone.h"

/*/## Keyboard instrument ##/*/

/*
An 8-key keyboard instrument in the scale of C major powered by Arduino.
Capable of 4-tone polyphony on one magnetic buzzer.
*/

/*
Current issues:
  - Can't play two notes on the same pin simultanously
    - The issue seems to be that the code tries to play both of the notes on the same object. Problem is addressed in the unfinished version 1.4 code.
    - The second key on the pin always overrides the first one. 
*/

// LEDs
const int LED_COUNT = 10;
const int LEDS[LED_COUNT] = {3, 4, 5, 6, 7, 8, 9, 10, 12, 13};

// Button pins
const int BUTTON_PIN_COUNT = 4;
// The voltage of each pin is monitored in main loop();.
int buttonPins[BUTTON_PIN_COUNT] = {};
// The reason as for why A1 is the first pin used instead of A0 is because A0 seemed not to be working correctly.
const int BUTTONS[BUTTON_PIN_COUNT] = {A1, A2, A3, A4};

// Voltage tresholds for pins of the buttons
const int voltage[BUTTON_PIN_COUNT] = {400, 700, 910, 950};


// Notes
/*
For some reason the notes are 4 semitones lower than they appear (for example, NOTE_E4 is actually C4). Probably an oversight with the 
MusicWithoutDelay library. The scale is supposed to make a C major scale.
*/
const int NOTES[8] = {NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_A4, NOTE_B4, NOTE_CS5, NOTE_DS5, NOTE_E5};

//Music library objects
MusicWithoutDelay SOUNDS[4] = {0, 0, 0, 0};

void ledControl(int ledIndex, int value) {
  digitalWrite(LEDS[ledIndex], value);
}

void buzzerON(int button) {
  SOUNDS[round(button) / 2].setFrequency(NOTES[button]);
}

void buzzerOFF(int button) {
  SOUNDS[round(button) / 2].setFrequency(0);
}

void isButtonON() {
  //Goes through the buttons and turn ON/OFF buttons and 
  //leds depending whether the button is pressed or not.
  
  for (int i = 0; i < BUTTON_PIN_COUNT; i++) {
    for (int j = 0; j < BUTTON_PIN_COUNT-1; j++) {
      if (j == BUTTON_PIN_COUNT-2) {
        if (buttonPins[i] >= voltage[j] && buttonPins[i] < voltage[j+1]) {
          //Button 1 and 2 are from same pin
          
          //Button 1
          buzzerON(2*i);
          ledControl((2*i), HIGH);
          
          //Button 2
          buzzerON(2*i+1);
          ledControl((2*i+1), HIGH);
        } else {
          //Button 1
          buzzerOFF(2*i);
          ledControl((2*i), LOW);
          
          //Button 2
          buzzerOFF(2*i+1);
          ledControl((2*i+1), LOW);
        }
      } else {
        if (buttonPins[i] >= voltage[j] && buttonPins[i] < voltage[j+1]) {
          buzzerON(2*i + j);
          ledControl((2*i + j), HIGH);
          break;
        }
      }
    }
  }
}

void setup() {
  /*
  Initializes MusicWithoutDelay objects. Tones will be played on the buzzer.
  The buzzer is connected to pin 11, as the library instructs to do.
  */
  for (int i = 0; i < 4; i++) {
      SOUNDS[i].begin(SQUARE, ENVELOPE0, 0);
  }

  // Initializes LEDs

  for (int i = 0; i < LED_COUNT; i++) {
      pinMode(LEDS[i], OUTPUT);
  }

  // ON/OFF switch for the keyboard. Not working as intended.
  //pinMode(2, INPUT);

  Serial.begin(9600);
}

void loop() {
  // If the switch is turned on
  //if (digitalRead(2) == HIGH) {
      buttonPins[0] = analogRead(BUTTONS[0]);
      buttonPins[1] = analogRead(BUTTONS[1]);
      buttonPins[2] = analogRead(BUTTONS[2]);
      buttonPins[3] = analogRead(BUTTONS[3]);

      for (int i = 0; i < 4; i++) {
        SOUNDS[i].update();
      }

      isButtonON();

      // Monitor voltages on each pin
      Serial.print("Pin 1: ");
      Serial.println(buttonPins[0]);
      Serial.print("Pin 2: ");
      Serial.println(buttonPins[1]);
      Serial.print("Pin 3: ");
      Serial.println(buttonPins[2]);
      Serial.print("Pin 4: ");
      Serial.println(buttonPins[3]);
      Serial.println();
  //}
}
