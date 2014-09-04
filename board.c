#include "board.h"

void led_init(void)
{
	P1DIR |= (P1_LED_RED | P1_LED_GRN);                          
	P1OUT &= ~(P1_LED_RED | P1_LED_GRN);  
}

/*
 * Configure button as an input and enable interrupts.
 */
void button_init(void)
{
	P1DIR &= ~P1_BUTTON;
	P1OUT |= P1_BUTTON;
	P1REN |= P1_BUTTON;
	P1IES |= P1_BUTTON;
	P1IFG &= ~P1_BUTTON;
	P1IE |= P1_BUTTON;
}

/*
 * 1. Configure pins for USCI UART operation.
 * 2. Select clock for UART.
 * 3. Configure UART using 1.1 MHz SMCLK, 9600 baud, oversample mode
 * 4. Enable receive interrupts.
 */
void uart_init(void)
{
	P1SEL |= P1_TXD | P1_RXD; 
	P1SEL2 |= P1_TXD | P1_RXD;
	UCA0CTL1 |= UCSWRST;  
	UCA0CTL1 |= UCSSEL_2; 
	UCA0BR0 = 7; 
	UCA0BR1 = 0;
	UCA0MCTL = UCBRF_3 | UCOS16;
	UCA0CTL1 &= ~UCSWRST;
	IE2 |= UCA0RXIE;
}

void board_init(void)
{
	led_init();
	button_init();
	uart_init();
}
