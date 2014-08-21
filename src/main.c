#include <msp430.h>
#include <stdio.h>
#include <string.h>
#include "board.h"

uint16_t counter;

int main(void) {
  /* initialization code */
  // disable watchdog
  WDTCTL = WDTPW | WDTHOLD;

  board_init();
  counter = 0;
       
  //__enable_interrupt(); // Enable Global Interrupts


  while(1) {
    __bis_SR_register(LPM3_bits+GIE);
  }
  return 0;
}

/* *************************************************************
 * Port Interrupt for Button Press 
 * 1. During standby mode: to exit and enter application mode
 * 2. During application mode: to recalibrate temp sensor 
 * *********************************************************** */
__attribute__ ((__interrupt__(PORT1_VECTOR)))
static void
PORT1_ISR(void)
{   
  P1OUT ^= P1_LED_GRN;
  printf("Hello world %d!\r\n", counter++);
  __bic_SR_register_on_exit(LPM3_bits);
  P1IFG &= ~P1_BUTTON;
}

int putchar(int c) {
  while(!(IFG2 & UCA0TXIFG));
  UCA0TXBUF = (unsigned char) c;

  return((unsigned char)c);
}

