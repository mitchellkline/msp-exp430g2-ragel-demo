#include <msp430.h>
#include <stdio.h>
//#include <string.h>
#include "board.h"
#include <errno.h>

#define RXBUFSIZE 10

/*
 * Allow for one extra character for terminator \0
 */
char rxbuf[RXBUFSIZE + 1];
uint8_t rxbuf_i = 0;

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

int8_t add_to_rxbuf(char c) {
	if (rxbuf_i > (RXBUFSIZE - 1)) {
		return ENOBUFS;
	}
	else {
		rxbuf[rxbuf_i] = c;
		rxbuf[rxbuf_i+1] = '\0';
		rxbuf_i++;
		return 0;
	}
}

void remove_from_rxbuf() {
	if (rxbuf_i > 0) {
		rxbuf_i--;
		rxbuf[rxbuf_i] = '\0';
	}
}

void parse_rxbuf() {
	rxbuf_i = 0;
	rxbuf[rxbuf_i] = '\0';
}

__attribute__((__interrupt__(USCIAB0RX_VECTOR)))
static void
USCIAB0RX_ISR(void) {
	char c = (char)UCA0RXBUF;
	printf("%c",c);
	if (c == '\r') {
		printf("%c", '\n');
		printf("rxbuf = %s\r\n",rxbuf);
		parse_rxbuf();
	}
	else if (c == '\b') {
		remove_from_rxbuf();
		printf(" \b");
	}
	else {
		int8_t err = add_to_rxbuf(c);
		if (err) {
			printf("\r\nERROR %d: rxbuf full. Resetting...\r\n",
				err);
			rxbuf_i = 0;
		}
	}
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

