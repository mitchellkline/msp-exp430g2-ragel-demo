#include <msp430.h>
#include <stdio.h>
//#include <string.h>
#include "board.h"

int main(void) {
  /* initialization code */
  // disable watchdog
  WDTCTL = WDTPW | WDTHOLD;

  board_init();
       
  __enable_interrupt(); // Enable Global Interrupts

  while(1) {
    //__bis_SR_register(LPM3_bits | GIE);
  }
  return 0;
}

__attribute__((__interrupt__(USCIAB0RX_VECTOR)))
static void
USCIAB0RX_ISR(void) {
    char c = (char)UCA0RXBUF;
    printf("%c", c); 
}

/* *************************************************************
 * Port Interrupt for Button Press 
 * *********************************************************** */
__attribute__((__interrupt__(PORT1_VECTOR)))
static void
PORT1_ISR(void)
{   
  P1OUT ^= P1_LED_GRN;
  P1IFG &= ~P1_BUTTON;
  //__bic_SR_register_on_exit(LPM3_bits);
}

int putchar(int c) {
  while(!(IFG2 & UCA0TXIFG));
  UCA0TXBUF = (unsigned char) c;

  return((unsigned char)c);
}

