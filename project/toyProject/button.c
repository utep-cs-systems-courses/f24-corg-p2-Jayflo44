#include <msp430.h>
#include "buzzer.h"
#include "button.h"
#include "libTimer.h"
#include "led.h"
#include "notes.h"

// Shared variables related to button state
volatile char p2val;
volatile char p1val;
volatile int buttonUp = 1;  // Initialize as "not pressed"
volatile int blinking = 0;  // Flag to start/stop blinking
volatile int blinkCount = 0;  // Counts the number of blinks

// Song arrays                                                                                                       
// Original song arrays for SW3
int melody1[] = {  NOTE_G4, NOTE_C5, NOTE_B4, NOTE_A4,
    NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4,
    NOTE_B4, NOTE_A4, NOTE_C5, NOTE_G4, // "We wish you a Merry Christmas"
    
    NOTE_C5, NOTE_B4, NOTE_A4,
    NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4,
    NOTE_D5, NOTE_C5, NOTE_B4, NOTE_A4, // "And a Happy New Year"
    
    NOTE_G4, NOTE_C5, NOTE_B4, NOTE_A4,
    NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4,
    NOTE_B4, NOTE_A4, NOTE_C5, NOTE_G4, // Repeats: "We wish you a Merry Christmas"
};
int noteDurations1[] = {300, 300, 300, 300,  // "We wish you"
    400, 200, 300, 300,  // "a Merry"
    300, 300, 400, 500,  // "Christmas"
    
    300, 300, 300, 300,  // "And a Happy"
    400, 200, 300, 300,  // "New Year"
    300, 300, 400, 500,  // End of phrase
    
    300, 300, 300, 300,  // Repeat "We wish you"
    400, 200, 300, 300,  // "a Merry"
    300, 300, 400, 500   // "Christmas"
};
int songLength1 = sizeof(melody1) / sizeof(melody1[0]);

// "Twinkle, Twinkle, Little Star" for SW1
int melody2[] = {
    NOTE_C4, NOTE_C4, NOTE_G4, NOTE_G4, NOTE_A4, NOTE_A4, NOTE_G4, 
    NOTE_F4, NOTE_F4, NOTE_E4, NOTE_E4, NOTE_D4, NOTE_D4, NOTE_C4
};
int noteDurations2[] = { 
    300, 300, 300, 300, 300, 300, 600, 
    300, 300, 300, 300, 300, 300, 600 
};
int songLength2 = sizeof(melody2) / sizeof(melody2[0]);
volatile int currentNoteIndex = 0;      // Index of the current note in the song                                     
volatile int currentNoteDuration = 0;   // Remaining duration for the current note                                   
volatile int playingSong = 0;           // Flag to indicate if a song is playing                                    

void switch_init() {
  P1REN |= SWITCHES;
  P1IE |= SWITCHES;
  P1OUT |= SWITCHES;
  P1DIR &= ~SWITCHES;

  P2REN |= SWITCHES_P2;
  P2IE |= SWITCHES_P2;
  P2OUT |= SWITCHES_P2;
  P2DIR &= ~SWITCHES_P2;
}

void led_init() {
  P1DIR |= LEDS;
  P1OUT &= ~LEDS;
}


void wdt_init() {
  configureClocks();
  enableWDTInterrupts();
  }

void switch_interrupt_handler() {
  // Button SW1: Control RED LED
 if (buttonUp && !(p1val & SW1)) {  // SW1 is pressed
        playSong(2);                   // Play "Twinkle, Twinkle, Little Star"
        buttonUp = 0;
    } else if (p1val & SW1) {          // SW1 is released
        buttonUp = 1;
    }
  // Button SW2: Control GREEN LED
  if (buttonUp && !(p2val & SW2)) {  // SW2 is pressed
    Gs1down();                       // Call function for SW2 press
    buttonUp = 0;
  } else if (p2val & SW2) {          // SW2 is released
    Gs1up();                         // Call function for SW2 release
    buttonUp = 1;
  }

  // Button SW3: Play Song
  if (buttonUp && !(p2val & SW3)) {  // SW3 is pressed
    playSong(1);
    buttonUp = 0;
  } else if (p2val & SW3) {          // SW3 is released
    buttonUp = 1;
  }

  // Button SW4: Control RED and GREEN LEDs Together
  if (buttonUp && !(p2val & SW4)) {  // SW4 is pressed
    Gs3down();                       // Call function for SW4 press (both LEDs on)
    buttonUp = 0;
  } else if (p2val & SW4) {          // SW4 is released
    buttonUp = 1;
  }
  
  if(buttonUp && !(p2val & SW5)){
    Gs4down();
    buttonUp = 0;
  }else if(p2val & SW5){
    Gs4up();
    buttonUp = 1;
  }
}

volatile int *currentMelody;
volatile int *currentNoteDurations;
volatile int currentSongLength;

void playSong(int songNumber) {
    if (!playingSong) { // Start the song only if not already playing
        if (songNumber == 1) {  // Song for SW3
            currentMelody = melody1;
            currentNoteDurations = noteDurations1;
            currentSongLength = songLength1;
        } else if (songNumber == 2) {  // Song for SW1
            currentMelody = melody2;
            currentNoteDurations = noteDurations2;
            currentSongLength = songLength2;
        }

        // Initialize song playback variables
        currentNoteIndex = 0;
        currentNoteDuration = currentNoteDurations[currentNoteIndex];
        playingSong = 1; // Set flag to indicate the song is playing

        int note = currentMelody[currentNoteIndex];
        buzzer_set_period(note == REST ? 0 : 2000000 / note);
    }
}
void Rs1down() {
  playSong(2);  // Turn RED LED on
}

void Rs1up() {
}
void Gs1down() {
    P1OUT |= LED_GREEN;  // Turn GREEN LED on
}

void Gs1up() {
    P1OUT &= ~LED_GREEN;  // Turn GREEN LED off
}

void Gs2down() {
    playSong(1);  // Start playing the song
}
// Function to start blinking on SW4 press
void Gs3down() {
    if (!blinking) {            // Start blinking if not already blinking
        blinking = 1;
        blinkCount = 0;         // Reset blink counter
    }
}
// Function to stop blinking
void Gs3up() {
    blinking = 0;               // Stop blinking
    P1OUT &= ~LED_RED;          // Ensure LED is off
}
void Gs4down() {
  P1OUT |= LED_GREEN;
  P1OUT |= LED_RED;
}

void Gs4up() {
    P1OUT &= ~LED_RED;  // Turn both LEDs off
}
