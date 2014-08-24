//#include <inttypes.h>
#include "rxbuf.h"
/*
 * Allow for one extra character for terminator \0. Rely on compiler to
 * initialize all values in rxbuf to 0.
 */
char rxbuf[RXBUFSIZE + 1];
static unsigned int rxbuf_i = 0;



/*
 * Add character to buffer.  The buffer is always maintained as a proper C
 * string.  The actual buffer size is one more than RXBUFSIZE, ensuring there is
 * always room to add a null character.
 */
enum erxbuf add_to_rxbuf(char c) 
{
	if (rxbuf_i > (RXBUFSIZE - 1)) {
		return ERXBUF_FULL;
	}
	else {
		rxbuf[rxbuf_i] = c;
		rxbuf[rxbuf_i+1] = '\0';
		rxbuf_i++;
		return ERXBUF_SUCCESS;
	}
}

void remove_from_rxbuf() 
{
	if (rxbuf_i > 0) {
		rxbuf_i--;
		rxbuf[rxbuf_i] = '\0';
	}
}

void clear_rxbuf() 
{
	rxbuf_i = 0;
	rxbuf[0] = '\0';
}
