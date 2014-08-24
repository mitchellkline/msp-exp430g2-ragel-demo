#ifndef __RXBUF_H__
#define __RXBUF_H__

#define RXBUFSIZE 10

/*
 * Add character to buffer.  The buffer is always maintained as a proper C
 * string.  The actual buffer size is one more than RXBUFSIZE, ensuring there is
 * always room to add a null character.
 */
int8_t add_to_rxbuf(char);

void remove_from_rxbuf(void);

void parse_rxbuf(void);

void clear_rxbuf(void);

char *get_rxbuf(void);
#endif
