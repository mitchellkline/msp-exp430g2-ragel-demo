#include "board.h"
#include <inttypes.h>
#include <string.h>
#ifdef DEBUG
	#include <stdio.h>
#endif

%%{
	machine board;
	write data;
}%%

static enum eboard h2i(uint8_t *i, char h)
{
	if (h >= '0' && h <= '9') {
		*i = h - '0';
	}
	else if (h>= 'A' && h <= 'F') {
		*i = h - 'A' + 0xA;
	}
	else if (h>= 'a' && h <= 'f') {
		*i = h - 'a' + 0xA;
	}
	else {
		return EBOARD_RANGE;
	}
	return EBOARD_SUCCESS;
}

enum eboard parse(char *p) 
{
	char *pe = p + strlen(p);
	int cs;

	uint8_t regaddr = 0;
	uint8_t regval = 0;

	enum {
		UNDEF,
		READ,
		WRITE
	} command = UNDEF;

	%%{
		action set_regaddr {
			uint8_t tmp = 0;
			h2i(&tmp, fc);
			regaddr = (regaddr << 4) + tmp;
		}
		
		action set_regval {
			uint8_t tmp = 0;
			h2i(&tmp, fc);
			regval = (regval << 4) + tmp;
		}

		action set_read {
			command = READ;
		}

		action set_write {
			command = WRITE;
		}

		regaddr_f = (xdigit@set_regaddr)+;
		regval_f = (xdigit@set_regval)+;
		read_cmd_f = 'read' space regaddr_f;
		write_cmd_f = 'write' space regaddr_f space regval_f;
		main := ( read_cmd_f@set_read | write_cmd_f@set_write );

		write init;
		write exec;
	}%%
	
	if ( cs < board_first_final ) {
		return EBOARD_CS;
	}
	else if (command == READ) {
		#ifdef DEBUG
		printf("Register address: %u\r\n", regaddr);
		#endif
		printf("Register value: %u\r\n",*((uint8_t *)regaddr));

		return EBOARD_SUCCESS;
	}
	else if (command == WRITE) {
		#ifdef DEBUG
		printf("Register address: %u\r\n", regaddr);
		printf("Register value: %u\r\n", regval);
		#endif
		return EBOARD_SUCCESS;
	}
	else {
		return EBOARD_UNDEF;
	}

}

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
