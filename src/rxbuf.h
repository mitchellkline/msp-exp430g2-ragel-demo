#ifndef __RXBUF_H__
#define __RXBUF_H__

/*
 * Set the size of rxbuf. Actual size will be one more than this to include the
 * null character.
 */
#define RXBUFSIZE 30

extern char rxbuf[RXBUFSIZE + 1];

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
enum erxbuf add_to_rxbuf(char);

/*
 * Remove the last character from rxbuf
 */
void remove_from_rxbuf(void);

/*
 * Re-initialize rxbuf.
 */
void clear_rxbuf(void);


#endif
