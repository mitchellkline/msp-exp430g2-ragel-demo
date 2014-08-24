#include <msp430.h>
#include <stdio.h>
#include "board.h"
#include "rxbuf.h"

int main(void) 
{
	/* 
	 * disable watchdog
	 */
	WDTCTL = WDTPW | WDTHOLD;

	board_init();
	     
	__enable_interrupt();

	while(1) {
		/*
		 * __bis_SR_register(LPM3_bits | GIE);
		 */
	}
	return 0;
}

/* 
 * USCI UART RX interrupt handler
 *
 * Get the character from the buffer. Always echo to the terminal.
 *	
 * A carraige return indicates the end of a command.  Send a line feed
 * to acknowledge receipt of the command. Optionally, we echo the rxbuf
 * state.  Finally, we parse rxbuf.
 * 
 * If we receive the backspace key, we first update the terminal to
 * delete the last character.  We then remove the character from rxbuf.
 * 
 * If we didn't receive a special character, we add it to rxbuf and
 * check for errors.
 */
__attribute__((__interrupt__(USCIAB0RX_VECTOR))) static void USCIAB0RX_ISR(void) 
{
	char c = (char)UCA0RXBUF;
	printf("%c",c);
	
	if (c == '\r') {
		printf("\n");
		#ifdef DEBUG
		printf("rxbuf = %s\r\n",rxbuf);
		#endif
		enum eboard err = parse(rxbuf);
		if (err) {
			printf("ERROR %d: Invalid command.\r\n",err);
		}
		clear_rxbuf();
	}
	else if (c == '\b') {
		printf(" \b");
		remove_from_rxbuf();
	}
	else {
		enum erxbuf err = add_to_rxbuf(c);
		if (err) {
			printf("\r\nERROR %d: rxbuf full. Resetting...\r\n",
			err);
			clear_rxbuf();
		}
	}
}

/* 
 * Port Interrupt for Button Press 
 */ 
__attribute__((__interrupt__(PORT1_VECTOR))) static void PORT1_ISR(void)
{   
	P1OUT ^= P1_LED_GRN;
	P1IFG &= ~P1_BUTTON;
	/*
	 * __bic_SR_register_on_exit(LPM3_bits);
	 */
}
