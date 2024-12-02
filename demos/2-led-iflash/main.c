//Alternate LEDs from Off, Green, and Red
#include <msp430.h>
#include "libTimer.h"
#include "led.h"

int main(void) {
  P1DIR |= LEDS;
  P1OUT &= ~LED_RED;
  P1OUT |= LED_GREEN;

  configureClocks();		/* setup master oscillator, CPU & peripheral clocks */
  enableWDTInterrupts();	/* enable periodic interrupt */
  

 or_sr(0x18);		/* CPU off, GIE on */
}

unsigned char wcount = 0;
unsigned char wcount1 = 0;
/*
everySecond()
{
  static unsigned char led_on = 0;
  switch(led_on){
  case 0:// off -> on
    P1OUT ^= LED_GREEN;
    led_on = 1;
    break;
  case 1:// on -> off
    P1OUT ^= LED_RED;
    P1OUT &= ~LED_GREEN;
    led_on = 0;
    break;
  }
}*/

void
__interrupt_vec(WDT_VECTOR) WDT()	/* 250 interrupts/sec */
{ 
  wcount++;
  wcount1++;
  if (wcount <= 100){
    P1OUT |= LED_RED;
    P1OUT &= ~LED_GREEN; 
    }/*
  if (wcount == 5){
    P1OUT &= ~LED_GREEN;
    P1OUT ^= LED_RED;
    }*/
  if (wcount1 < 5){
    P1OUT ^= LED_GREEN;
    wcount = 0;
    }
} 

