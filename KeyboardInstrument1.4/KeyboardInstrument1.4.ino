#include "MusicWithoutDelay.h"
#include "Tone.h"

/*/## Keyboard instrument ##/*/

/*
An 8-key keyboard instrument in the scale of C major powered by Arduino.
Capable of 4-tone polyphony on one magnetic buzzer.
*/

/*
As of now, the code seems to manage playing the tones mostly fine. 
However, turning off the tones isn't functioning as intended.
That is pretty much the last step, and also the hardest.
Limits of the MusicWithoutDelay library is showing, as it was mostly intended
for playing predefined melodies.
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
MusicWithoutDelay library. This scale is supposed to make a C major scale.
*/
const int NOTES[8] = {NOTE_E4, NOTE_FS4, NOTE_GS4, NOTE_A4, NOTE_B4, NOTE_CS5, NOTE_DS5, NOTE_E5};

// Music library objects
MusicWithoutDelay SOUNDS[4] = {0, 0, 0, 0};

// Stores whether a specific MusicWithouDelay object is currently in use or not.
// Let's call them buzzers for the sake of simplicity, even though they're all playing on one single buzzer (let's just imagine that our buzzer is a single speaker consisting of 4 buzzers, alright?).
bool isBuzzerInUse[4] = {false, false, false, false};

// Stores which buzzer is in use. The first value of the index is the buzzer that has been in use the longest.
int usedBuzzers[4] = {-1, -1, -1, -1};

// Stores which notes are currently playing on a buzzer (if any). -1 = no note is playing
int currentNotes[4] = {-1, -1, -1, -1};

void buzzerON(int note) {
    int availableBuzzer = -1;
    for (int i = 0; i < 4; i++) {
        if (!isBuzzerInUse[i]) {
            availableBuzzer = i;
            break;
        }
    }

    // If all buzzers are in use, use the oldest one (first one in the usedBuzzers array)
    if (availableBuzzer == -1) {
        availableBuzzer = usedBuzzers[0];
        // Shift usedBuzzers array to remove oldest buzzer
        for (int k = 1; k < 4; k++) {
            usedBuzzers[k-1] = usedBuzzers[k];
        }
        usedBuzzers[3] = -1;
    }

    // Set frequency and mark buzzer as in use
    SOUNDS[availableBuzzer].setFrequency(NOTES[note]);
    isBuzzerInUse[availableBuzzer] = true;
    currentNotes[availableBuzzer] = note;

    for (int i = 0; i < 4; i++) {
        if (usedBuzzers[i] == -1) {
        usedBuzzers[i] = availableBuzzer;
        }
    }
}

// WIP
// Turn the buzzer off. In it's current state, it's seems to not work correctly.
void buzzerOFF(int pin, int notes) {
    for (int i = 0; i < 4; i++) {
        if (buttonPins[pin] < 400 && (currentNotes[i]) == NOTES[notes] || (currentNotes[i]) == NOTES[notes + 1]) {
            usedBuzzers[i] = -1;
            isBuzzerInUse[i] = false;
            SOUNDS[i].setFrequency(0);
        } else if (buttonPins[pin] >= 400 && buttonPins[pin] <= 700 && currentNotes[i] == NOTES[notes + 1]) {
            usedBuzzers[i] = -1;
            isBuzzerInUse[i] = false;
            SOUNDS[i].setFrequency(0);
        } else if (buttonPins[pin] >= 700 && buttonPins[pin] <= 910 && currentNotes[i] == NOTES[notes]) {
            usedBuzzers[i] = -1;
            isBuzzerInUse[i] = false;
            SOUNDS[i].setFrequency(0);
        }
    }
}


void playTone() {
    int j = 0;
    while (j <= 6) {
        for (int i = 0; i < 4; i++) {
            if (buttonPins[i] >= voltage[0] && buttonPins[i] <= voltage[1]) {
                buzzerON(j);
            } else if (buttonPins[i] >= voltage[1] && buttonPins[i] <= voltage[2]) {
                buzzerON(j + 1);
            } else if (buttonPins[i] >= voltage[2] && buttonPins[i] <= voltage[3]) {
                buzzerON(j);
                buzzerON(j + 1);
            }
            j += 2;
        }
    }
}

void stopTone() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            buzzerOFF(i, j);
        }
    }
}

/*
void playTone () {
    int i = 0;
    while (i <= 6) {
        for (int pin = 0; pin < 4; pin++) {
            if (buttonPins[pin] >= voltage[0] && buttonPins[pin] <= voltage[1]) {
                buzzerON(NOTES[i]);
                buzzerOFF(pin, usedBuzzers[0]);
            } else if (buttonPins[pin] >= voltage[1] && buttonPins[pin] <= voltage[2]) {
                buzzerON(NOTES[i+1]);
                buzzerOFF(pin, usedBuzzers[0]);
            } else if (buttonPins[pin] >= voltage[2] && buttonPins[pin] <= voltage[3]) {
                buzzerON(NOTES[i]);
                buzzerON(NOTES[i+1]);
            }
            i += 2;
        }
    }
}
*/

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

        playTone();

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