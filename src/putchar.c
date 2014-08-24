#include <msp430.h>
#include <stdio.h>

int putchar(int c) {
	while(!(IFG2 & UCA0TXIFG));
	UCA0TXBUF = (unsigned char) c;

	return((unsigned char)c);
}

