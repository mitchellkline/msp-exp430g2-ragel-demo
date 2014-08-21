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

void uart_init(void) {
  P1SEL |= P1_TXD | P1_RXD; // USCI_A0 UART mode
  P1SEL2 |= P1_TXD | P1_RXD;
  UCA0CTL1 |= UCSWRST;  // Put state machine in reset
  UCA0CTL1 |= UCSSEL_2; // Select SMCLK
  // configure for 1.1 MHz clock, 9600 baud, oversampling
  UCA0BR0 = 7; 
  UCA0BR1 = 0;
  UCA0MCTL = UCBRF_3 | UCOS16;
  UCA0CTL1 &= ~UCSWRST;
}

void board_init(void) {
  led_init();
  button_init();
  uart_init();
}
