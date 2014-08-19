#include <msp430.h>
#include <string.h>
#include "board.h"
#include "uart.h"

#define FCLK 1100000L
#define BAUD_RATE 9600L

#define MODE_STANDBY 0
#define MODE_TRANSMIT 1

#define UART_BUF 50

uint8_t mode = MODE_STANDBY;

UARTConfig cnf;
USCIUARTRegs uartUsciRegs;
USARTUARTRegs uartUsartRegs;

uint8_t uartTxBuf[UART_BUF];
uint8_t uartRxBuf[UART_BUF];

void main(void) {
  /* initialization code */
  // disable watchdog
  WDTCTL = WDTPW | WDTHOLD;

  board_init();

  initUartDriver();
  // Configure UART Module on USCIA0
  cnf.moduleName = USCI_A0;
  // Use UART Pins P1.1 and P1.2
  cnf.portNum = PORT_1;
  cnf.RxPinNum = PIN1;
  cnf.TxPinNum = PIN2;
  // 115200 Baud from 1.1 MHz SMCLK
  cnf.clkRate = FCLK;
  cnf.baudRate = BAUD_RATE;
  cnf.clkSrc = UART_CLK_SRC_SMCLK;
  // 8N1
  cnf.databits = 8;
  cnf.parity = UART_PARITY_NONE;
  cnf.stopbits = 1;
  int res = configUSCIUart(&cnf,&uartUsciRegs);
  if(res != UART_SUCCESS)
  {
      // Failed to initialize UART for some reason
      P1OUT |= P1_LED_RED;
      __no_operation();
  }
  // Configure the buffers that will be used by the UART Driver.
  // These buffers are exclusively for the UART driver's use and should not be touched
  // by the application itself. Note that they may affect performance if they're too
  // small.

  setUartTxBuffer(&cnf, uartTxBuf, UART_BUF);
  setUartRxBuffer(&cnf, uartRxBuf, UART_BUF);
  enableUartRx(&cnf);

  /*********************************/
        
  __enable_interrupt(); // Enable Global Interrupts

   

  while(1)
  {
    // Send the string hello using interrupt driven
    if (mode == MODE_TRANSMIT) {
      uartSendDataInt(&cnf,(unsigned char *)"Hello\r\n", strlen("Hello\r\n"));
    }
    //P1OUT |= P1_LED_GRN;
    __delay_cycles(FCLK >> 1);
    //P1OUT &= ~P1_LED_GRN;
    __delay_cycles(FCLK >> 1);

    int bytesAvailable = numUartBytesReceived(&cnf);
    if(bytesAvailable > 0)
    {
      //P1OUT |= P1_LED_RED;
      unsigned char tempBuf[UART_BUF];
      memset(tempBuf,0,UART_BUF);

      volatile int bytesRead = readRxBytes(&cnf, tempBuf, bytesAvailable, 0);
      if(bytesRead == bytesAvailable)
      {
        // All requested bytes read. Do something with it.

	// If we receive the letter a, we toggle the LED
	if(tempBuf[0] == 'a')
	{
	  P1OUT ^= P1_LED_GRN;
	}
      }
      else
      {
	// Couldn't read all the bytes we requested
	__no_operation();
      }
    }
  }
}

/* *************************************************************
 * Port Interrupt for Button Press 
 * 1. During standby mode: to exit and enter application mode
 * 2. During application mode: to recalibrate temp sensor 
 * *********************************************************** */
#pragma vector=PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{   
  P1IFG = 0; /* clear interrupt */
  mode = (mode == MODE_STANDBY) ? MODE_TRANSMIT : MODE_STANDBY;
}
