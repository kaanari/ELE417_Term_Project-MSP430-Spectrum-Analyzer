#include<clock.h>
/*
 * clock.c
 *
 *  Created on: Dec 31, 2020
 *      Author: kaanari
 */


void clock_config(){

    CSCTL2 &= ~FLLD_1; // Disable FDOCLK/2 => Enablee FDCOCLK/1 = MCLK = 4Mhz

    CSCTL1 &= ~DCORSEL_1;
    CSCTL1 |= DCORSEL_2; // 4 Mhz MCLK
    //CSCTL1 |= DCORSEL_3; // 8 Mhz MCLK

}



void initClockTo16MHz()
{
    // Configure one FRAM waitstate as required by the device datasheet for MCLK
    // operation beyond 8MHz _before_ configuring the clock system.
    FRCTL0 = FRCTLPW | NWAITS_1;

    __bis_SR_register(SCG0);    // disable FLL
    CSCTL3 |= SELREF__REFOCLK;  // Set REFO as FLL reference source
    CSCTL0 = 0;                 // clear DCO and MOD registers
    CSCTL1 &= ~(DCORSEL_7);     // Clear DCO frequency select bits first
    CSCTL1 |= DCORSEL_5;        // Set DCO = 16MHz
    CSCTL2 = FLLD_0 + 487;      // set to fDCOCLKDIV = (FLLN + 1)*(fFLLREFCLK/n)
                                //                   = (487 + 1)*(32.768 kHz/1)
                                //                   = 16 MHz

    __delay_cycles(3);
    __bic_SR_register(SCG0);                        // enable FLL
    while(CSCTL7 & (FLLUNLOCK0 | FLLUNLOCK1));      // FLL locked
}
