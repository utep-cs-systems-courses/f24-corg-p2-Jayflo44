#ifndef BUTTON_H
#define BUTTON_H

// Button definitions
#define SW1 BIT3                /* switch1 is P1.3 */
#define SW2 BIT0                /* switch on P2.0 */
#define SW3 BIT1                /* switch on P2.1 */
#define SW4 BIT2                /* switch on P2.2 */
#define SW5 BIT3                /* switch on P2.3 */
#define SWITCHES SW1            /* switch on P1 */
#define SWITCHES_P2 (SW2 | SW3 | SW4 | SW5)  /* switches on Port 2 */
// Button and utility function declarations
// Shared variables for song control
extern volatile int currentNoteIndex;
extern volatile int currentNoteDuration;
extern volatile int playingSong;
extern int melody[];
extern int noteDurations[];
extern int songLength;

// Shared variables for button states
extern volatile char p2val;
extern volatile char p1val;
extern volatile int buttonDown;

void switch_init();
void led_init();
void wdt_init();
void Gs1down();
void Gs1up();
void Gs2down();
void Gs2up();
void Gs3down();
void Gs3up();
void Gs4down();
void Gs4up();
void playSong();
void Rs1down();
void Rs1up();
void switch_interrupt_handler();
void wdt_init();
#endif // BUTTON_H
