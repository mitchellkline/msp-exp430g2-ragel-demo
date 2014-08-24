#ifndef __STRBUF_H__
#define __STRBUF_H__
/*
 * Set the size of rxbuf. Actual size will be one more than this to include the
 * null character.
 */
#define STRBUFSIZE 32

/*
 * The stack contains a c-string (always null terminated) and the index of the
 * null terminator
 */
struct strbuf_stack {
	char s[STRBUFSIZE+1];
	unsigned int i;
};

enum estrbuf {
	ESTRBUF_SUCCESS,
	ESTRBUF_FULL,
	ESTRBUF_RANGE,
	ESTRBUF_UNDEF,
	ESTRBUF_CS,
	ESTRBUF_EMPTY
};

/*
 * Add character to buffer.  The buffer is always maintained as a proper C
 * string.
 */
enum estrbuf strbuf_push(struct strbuf_stack *, char);

/*
 * Remove the last character from rxbuf
 */
enum estrbuf strbuf_pop(struct strbuf_stack *, char *);

/*
 * Re-initialize rxbuf.
 */
void strbuf_init(struct strbuf_stack *);


#endif
