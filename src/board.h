#ifndef __BOARD_H__
#define __BOARD_H__

#include <msp430.h>

#define     P1_LED_RED            BIT0
#define     P1_LED_GRN            BIT6

#define     P1_BUTTON             BIT3

#define     P1_RXD                BIT1                      // RXD on P1.1
#define     P1_TXD                BIT2                      // TXD on P1.2

void led_init(void);
void button_init(void);
void board_init(void);
#endif
