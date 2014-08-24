#ifndef __STRBUF_H__
#define __STRBUF_H__
#include <inttypes.h>
/*
 * Set the size of rxbuf. Actual size will be one more than this to include the
 * null character.
 */
#define STRBUFSIZE 32

struct strbuf_stack {
	char s[STRBUFSIZE+1];
	uint8_t i;
};

enum estrbuf {
	ESTRBUF_SUCCESS,
	ESTRBUF_FULL,
	ESTRBUF_RANGE,
	ESTRBUF_UNDEF,
	ESTRBUF_CS
};

/*
 * Add character to buffer.  The buffer is always maintained as a proper C
 * string.
 */
enum estrbuf strbuf_push(struct strbuf_stack *, char);

/*
 * Remove the last character from rxbuf
 */
void strbuf_pop(struct strbuf_stack *);

/*
 * Re-initialize rxbuf.
 */
void strbuf_init(struct strbuf_stack *);


#endif
