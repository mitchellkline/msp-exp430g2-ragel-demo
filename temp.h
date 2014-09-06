#ifndef __TEMP_H__
#define __TEMP_H__

#include <inttypes.h>

void temp_init(void);

/*
 * Samples temperature.  Result is a fixed point value in deg C, 2 decimal
 * places.
 */
void temp_sample(int16_t *);

#endif
