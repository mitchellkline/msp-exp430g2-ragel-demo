#ifndef __RXBUF_H__
#define __RXBUF_H__
#include <inttypes.h>
/*
 * Set the size of rxbuf. Actual size will be one more than this to include the
 * null character.
 */
#define RXBUFSIZE 32

struct rxbuf_stack {
	char s[RXBUFSIZE+1];
	uint8_t i;
};

enum erxbuf {
	ERXBUF_SUCCESS,
	ERXBUF_FULL,
	ERXBUF_RANGE,
	ERXBUF_UNDEF,
	ERXBUF_CS
};

/*
 * Add character to buffer.  The buffer is always maintained as a proper C
 * string.
 */
enum erxbuf rxbuf_push(struct rxbuf_stack *, char);

/*
 * Remove the last character from rxbuf
 */
void rxbuf_pop(struct rxbuf_stack *);

/*
 * Re-initialize rxbuf.
 */
void rxbuf_init(struct rxbuf_stack *);


#endif
