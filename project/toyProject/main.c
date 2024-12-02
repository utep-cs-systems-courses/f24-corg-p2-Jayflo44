#include <msp430.h>
#include "libTimer.h"
#include "led.h"
#include "buzzer.h"
#include "button.h"
#include "notes.h"
extern volatile int blinking;
extern volatile int blinkCount;
extern volatile int *currentMelody;
extern volatile int *currentNoteDurations;
extern volatile int currentSongLength;
extern volatile int currentNoteIndex;
extern volatile int currentNoteDuration;
extern volatile int playingSong;

int main(void) {                                                                                          
    switch_init();
    led_init();
    buzzer_init();
    configureClocks();
    enableWDTInterrupts();
    or_sr(0x18);  // CPU off, GIE on                                                                                
}

void __interrupt_vec(WDT_VECTOR) WDT() {
    // Handle song playback timing independently of button state
   if (playingSong && --currentNoteDuration <= 0) {
     currentNoteIndex++;
     if (currentNoteIndex < currentSongLength) {
       currentNoteDuration = currentNoteDurations[currentNoteIndex];
       int note = currentMelody[currentNoteIndex];
       buzzer_set_period(note == REST ? 0 : 2000000 / note);
     } else {
       playingSong = 0;  // End of song
       buzzer_set_period(0);  // Turn off buzzer
     }
   }
    
  // Update button states
   p2val = P2IN;
   p1val = P1IN;
   // Set edge select for buttons based on state
   P1IES |= (p1val & SWITCHES);  // Sense down if switch is up
   P1IES &= (p1val | ~SWITCHES); // Sense up if switch is down
   P2IES |= (p2val & SWITCHES_P2);
   P2IES &= (p2val | ~SWITCHES_P2);
   // Call the switch handler to check button actions
   switch_interrupt_handler();
    // Clear interrupt flags
   P1IFG &= ~SWITCHES;
   P2IFG &= ~SWITCHES_P2;
}

static int blinkLimit = 5;
void blinkUpdate() {
    static int blinkCount = 0;                                                                                        
    blinkCount++;                                                                                                     
    if (blinkCount >= blinkLimit) {                                                                                   
        blinkCount = 0;                                                                                               
        P1OUT |= LED_GREEN;                                                                                           
    } else {                                                                                                          
        P1OUT &= ~LED_GREEN;                                                                                          
        }
}


