#include "board.h"

void led_init(void)
{
  P1DIR |= (P1_LED_RED | P1_LED_GRN);                          
  P1OUT &= ~(P1_LED_RED | P1_LED_GRN);  
}

void button_init(void)                 // Configure Push Button 
{
  P1DIR &= ~P1_BUTTON;
  P1OUT |= P1_BUTTON;
  P1REN |= P1_BUTTON;
  P1IES |= P1_BUTTON;
  P1IFG &= ~P1_BUTTON;
  P1IE |= P1_BUTTON;
}

void board_init(void) {
  led_init();
  button_init();

  P1DIR |= P1_TXD;
  P1DIR &= ~P1_RXD;
  P1SEL |= P1_TXD | P1_RXD;
  P1SEL2 |= P1_TXD | P1_RXD;
}
