#include <msp430.h>
#include <string.h>
#include "board.h"
#include "uart.h"

#define FCLK 1048576L

UARTConfig cnf;
USCIUARTRegs uartUsciRegs;
USARTUARTRegs uartUsartRegs;

uint8_t uartTxBuf[50];
int8_t uartRxBuf[50];

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
  cnf.baudRate = 2400L;
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

  setUartTxBuffer(&cnf, uartTxBuf, 50);
  setUartRxBuffer(&cnf, uartRxBuf, 50);
  enableUartRx(&cnf);

  /*********************************/
        
  __enable_interrupt(); // Enable Global Interrupts

   

  while(1)
  {
    // Send the string hello using interrupt driven
    //uartSendDataInt(&cnf,(unsigned char *)"Hello\r\n", strlen("Hello\r\n"));
    P1OUT |= P1_LED_GRN;
    __delay_cycles(FCLK >> 1);
    P1OUT &= ~P1_LED_GRN;
    __delay_cycles(FCLK >> 1);

    int bytesAvailable = numUartBytesReceived(&cnf);
    if(bytesAvailable > 0)
    {
      P1OUT |= P1_LED_RED;
      unsigned char tempBuf[50];
      memset(tempBuf,0,50);

      volatile int bytesRead = readRxBytes(&cnf, tempBuf, bytesAvailable, 0);
      if(bytesRead == bytesAvailable)
      {
        // All requested bytes read. Do something with it.

	// If we receive the letter a, we toggle the LED
	if(tempBuf[0] == 'a')
	{
	  P1OUT |= P1_LED_RED;
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


