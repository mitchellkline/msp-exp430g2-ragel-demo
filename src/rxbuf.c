//#include <inttypes.h>
#include "rxbuf.h"


/*
 * Add character to buffer.  The buffer is always maintained as a proper C
 * string.  The actual buffer size is one more than RXBUFSIZE, ensuring there is
 * always room to add a null character.
 */
enum erxbuf rxbuf_push(struct rxbuf_stack *buf, char c) 
{
	if (buf->i > (RXBUFSIZE - 1)) {
		return ERXBUF_FULL;
	}
	else {
		buf->s[buf->i] = c;
		buf->s[buf->i+1] = '\0';
		buf->i = buf->i + 1;
		return ERXBUF_SUCCESS;
	}
}

void rxbuf_pop(struct rxbuf_stack *buf) 
{
	if (buf->i > 0) {
		buf->i = buf->i - 1;
		buf->s[buf->i] = '\0';
	}
}

void rxbuf_init(struct rxbuf_stack *buf) 
{
	buf->i = 0;
	buf->s[0] = '\0';
}
