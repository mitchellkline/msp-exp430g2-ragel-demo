#include <msp430.h>
#include <inttypes.h>
#include <errno.h>
#include "rxbuf.h"

/*
 * Allow for one extra character for terminator \0. Rely on compiler to
 * initialize all values in rxbuf to 0.
 */
static char rxbuf[RXBUFSIZE + 1];
static uint8_t rxbuf_i = 0;

/*
 * Add character to buffer.  The buffer is always maintained as a proper C
 * string.  The actual buffer size is one more than RXBUFSIZE, ensuring there is
 * always room to add a null character.
 */
int8_t add_to_rxbuf(char c) 
{
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

void remove_from_rxbuf() 
{
	if (rxbuf_i > 0) {
		rxbuf_i--;
		rxbuf[rxbuf_i] = '\0';
	}
}

void parse_rxbuf() 
{
	clear_rxbuf();
}

void clear_rxbuf() 
{
	rxbuf_i = 0;
	rxbuf[0] = '\0';
}

char *get_rxbuf()
{
	return rxbuf;
}
