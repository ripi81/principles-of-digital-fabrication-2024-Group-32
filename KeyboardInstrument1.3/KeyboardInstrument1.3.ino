#include "MusicWithoutDelay.h"
#include "Tone.h"

// LEDs
const int LED_COUNT = 10;
const int LEDS[LED_COUNT] = {3, 4, 5, 6, 7, 8, 9, 10, 12, 13};

// Button pins
const int buttonPinCount = 4;
int buttonPins[buttonPinCount] = {};
const int BUTTONS[buttonPinCount] = {A1, A2, A3, A4};

// Voltage thresholds for pins of the buttons
int voltage[buttonPinCount] = {400, 800, 910, 1000};

// Notes
/*
For some reason the notes are 4 semitones lower than they appear (for example, NOTE_E4 is actually C4). Probably an oversight with the 
MusicWithoutDelay library. This scale is supposed to make a C major scale.
*/
const int NOTES[8] = {NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_A4, NOTE_B4, NOTE_CS5, NOTE_DS5, NOTE_E5};

// Music library objects
MusicWithoutDelay SOUNDS[4] = {0, 0, 0, 0};

// Controls the LEDs
void ledControl(int ledIndex, int value) {
  digitalWrite(LEDS[ledIndex], value);
}

// Turn the buzzers on
void buzzerON(int button) {
  SOUNDS[round(button) / 2].setFrequency(NOTES[button]);
}

// Turn the buzzers off
void buzzerOFF(int button) {
  SOUNDS[round(button) / 2].setFrequency(0);
}

void isButtonON() {
  
  /*
  Goes through the buttons and turns buttons and 
  leds ON/OFF depending whether the button is pressed or not.
  */

  for (int i = 0; i < buttonPinCount; i++) {
    for (int j = 0; j < buttonPinCount-1; j++) {
      if (j == buttonPinCount-2) {
        if (buttonPins[i] >= voltage[j] && buttonPins[i] < voltage[j+1]) {
          // Buttons 1 and 2 are from same pin
          
          // Button 1
          buzzerON(2*i);
          ledControl((2*i), HIGH);
          
          // Button 2
          buzzerON(2*i+1);
          ledControl((2*i+1), HIGH);
        } else {
          // Button 1
          buzzerOFF(2*i);
          ledControl((2*i), LOW);
          
          // Button 2
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
  Sets up the MusicWithoutDelay objects. Tones will be played on the buzzer. 
  The buzzer is connected to pin 11, as the library instructs to do. 
  */
  SOUNDS[0].begin(SQUARE, ENVELOPE0, 0);
  SOUNDS[1].begin(SQUARE, ENVELOPE0, 0);
  SOUNDS[2].begin(SQUARE, ENVELOPE0, 0);
  SOUNDS[3].begin(SQUARE, ENVELOPE0, 0);
  
  // LEDs
  pinMode(LEDS[0], OUTPUT);
  pinMode(LEDS[1], OUTPUT);
  pinMode(LEDS[2], OUTPUT);
  pinMode(LEDS[3], OUTPUT);
  pinMode(LEDS[4], OUTPUT);
  pinMode(LEDS[5], OUTPUT);
  pinMode(LEDS[6], OUTPUT);
  pinMode(LEDS[7], OUTPUT);
  pinMode(LEDS[8], OUTPUT);
  pinMode(LEDS[9], OUTPUT);

  // ON/OFF switch for the keyboard. Not working as intended.
  pinMode(2, INPUT);

  Serial.begin(9600);
}

void loop() {
  // If the switch is turned on
  if (digitalRead(2) == HIGH) {
    buttonPins[0] = analogRead(BUTTONS[0]);
    buttonPins[1] = analogRead(BUTTONS[1]);
    buttonPins[2] = analogRead(BUTTONS[2]);
    buttonPins[3] = analogRead(BUTTONS[3]);
    
    isButtonON();

    SOUNDS[0].update();
    SOUNDS[1].update();
    SOUNDS[2].update();
    SOUNDS[3].update();
    
    // Monitor voltages on each pin
    Serial.print("Nappi 1: ");
    Serial.println(buttonPins[0]);
    Serial.print("Nappi 2: ");
    Serial.println(buttonPins[1]);
    Serial.print("Nappi 3: ");
    Serial.println(buttonPins[2]);
    Serial.print("Nappi 4: ");
    Serial.println(buttonPins[3]);
    Serial.println();
  }
}