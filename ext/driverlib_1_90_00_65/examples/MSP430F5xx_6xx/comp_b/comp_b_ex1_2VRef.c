/* --COPYRIGHT--,BSD
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * --/COPYRIGHT--*/
//******************************************************************************
//! LED Toggle in LPM4, Input is compared against 2.0V Internal Reference
//!
//!  Use CompB and internal reference to determine if input 'Vcompare' is high
//!  of low.  When Vcompare exceeds 2.0V CBOUT goes high and when Vcompare is
//!  less than 2.0V then CBOUT goes low. Connect P1.6/CBOUT to P1.0 externally
//!  to see the LED toggle accordingly.
//!
//!              MSP430x552x
//!          ------------------
//!      /|\|                  |
//!       | |                  |
//!       --|RST       P6.0/CB0|<- Vcompare
//!         |        P1.6/CBOUT|--> 'high'(Vcompare>2.0V); 'low'(Vcompare<2.0V)
//!         |                  | |
//!         |              P1.0|_| LED 'ON'(Vcompare>2.0V); 'OFF'(Vcompare<2.0V)
//!         |                  |
//!
//! This example uses the following peripherals and I/O signals.  You must
//! review these and change as needed for your own board:
//! - COMPB peripheral
//! - GPIO Port peripheral
//! - CB0
//! - CBOUT
//!
//! This example uses the following interrupt handlers.  To use this example
//! in your own application you must add these interrupt handlers to your
//! vector table.
//! - None.
//!
//******************************************************************************

#include "driverlib.h"

void main(void)
{
        //Stop WDT
        WDT_A_hold(WDT_A_BASE);


        //Select CBOUT function on P1.6/CBOUT and set P1.6 to output direction
        GPIO_setAsPeripheralModuleFunctionOutputPin(
                GPIO_PORT_P1,
                GPIO_PIN6
                );

        //Initialize the Comparator B module
        /* Base Address of Comparator B,
         * Pin CB0 to Positive(+) Terminal,
         * Reference Voltage to Negative(-) Terminal,
         * Normal Power Mode,
         * Output Filter On with minimal delay,
         * Non-Inverted Output Polarity
         */
        COMP_B_init(COMP_B_BASE,
                    COMP_B_INPUT0,
                    COMP_B_VREF,
                    COMP_B_POWERMODE_NORMALMODE,
                    COMP_B_FILTEROUTPUT_DLYLVL1,
                    COMP_B_NORMALOUTPUTPOLARITY
                    );

        //Set the reference voltage that is being supplied to the (-) terminal
        /* Base Address of Comparator B,
         * Reference Voltage of 2.0 V,
         * Lower Limit of 2.0*(32/32) = 2.0V,
         * Upper Limit of 2.0*(32/32) = 2.0V,
         * Static Mode Accuracy
         */
        COMP_B_setReferenceVoltage(COMP_B_BASE,
                                   COMP_B_VREFBASE2_0V,
                                   32,
                                   32,
                                   COMP_B_ACCURACY_STATIC
                                   );

        //Allow power to Comparator module
        COMP_B_enable(COMP_B_BASE);

        //delay for the reference to settle
        __delay_cycles(75);

        //Enter LPM4
        __bis_SR_register(LPM4_bits);

        //For debug
        __no_operation();
}
