#include <msp430.h>
#include <stdio.h>
#include "board.h"
#include "strbuf.h"
#include "parser.h"

/*
 * Allow for one extra character for terminator \0. Rely on compiler to
 * initialize all values in rxbuf to 0.
 */
struct strbuf_stack rxbuf;

int main(void) 
{
	/* 
	 * disable watchdog
	 */
	WDTCTL = WDTPW | WDTHOLD;

	strbuf_init(&rxbuf);
	board_init();

	/*
	 * UART does not connect if we print anything before establishing a
	 * connection.
	 * printf(	"MSP-EXP430G2 Launchpad UART established.\r\n"
	 *	"Enter command '?' for list of commands.\r\n"
	 *	"> ");
	 */
     
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
		printf("rxbuf = %s\r\n",rxbuf.s);
		#endif
		enum eparser err = parse(rxbuf.s);
		if (err) {
			printf("ERROR %d: Invalid command.\r\n",err);
		}
		strbuf_init(&rxbuf);
	}
	else if (c == '\b') {
		printf(" \b");
		strbuf_pop(&rxbuf, &c);
	}
	else {
		enum estrbuf err = strbuf_push(&rxbuf,c);
		if (err) {
			printf("\r\nERROR %d: strbuf full. Resetting...\r\n",
			err);
			strbuf_init(&rxbuf);
		}
	}
}

/* 
 * Port Interrupt for Button Press 
 */ 
__attribute__((__interrupt__(PORT1_VECTOR))) static void PORT1_ISR(void)
{   
	//P1OUT ^= P1_LED_GRN;
	printf("User button interrupt.\r\n");
	P1IFG &= ~P1_BUTTON;
	/*
	 * __bic_SR_register_on_exit(LPM3_bits);
	 */
}
