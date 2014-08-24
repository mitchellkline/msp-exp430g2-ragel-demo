//#include <inttypes.h>
#include "strbuf.h"


/*
 * Add character to buffer.  The buffer is always maintained as a proper C
 * string.  The actual buffer size is one more than RXBUFSIZE, ensuring there is
 * always room to add a null character.
 */
enum estrbuf strbuf_push(struct strbuf_stack *buf, char c) 
{
	if (buf->i > (STRBUFSIZE - 1)) {
		return ESTRBUF_FULL;
	}
	else {
		buf->s[buf->i] = c;
		buf->s[buf->i+1] = '\0';
		buf->i = buf->i + 1;
		return ESTRBUF_SUCCESS;
	}
}

void strbuf_pop(struct strbuf_stack *buf) 
{
	if (buf->i > 0) {
		buf->i = buf->i - 1;
		buf->s[buf->i] = '\0';
	}
}

void strbuf_init(struct strbuf_stack *buf) 
{
	buf->i = 0;
	buf->s[0] = '\0';
}
