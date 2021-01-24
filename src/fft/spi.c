#include <spi.h>
#include <stdint.h>

//******************************************************************************
//   MSP430FR243x Demo - eUSCI_A0, SPI 4-Wire Slave multiple byte RX/TX
//
//   Description: SPI master communicates to SPI slave sending and receiving
//   3 different messages of different length. SPI slave will enter LPM0
//   while waiting for the messages to be sent/receiving using SPI interrupt.
//   ACLK = NA, MCLK = SMCLK = DCO 16MHz.
//
//
//                   MSP430FR2633
//                 -----------------
//            /|\ |             RST |<- Master's GPIO (To reset slave)
//             |  |                 |
//             ---|RST          P1.4|<- Data In (UCA0SIMO)
//                |                 |
//                |             P1.5|-> Data Out (UCA0SOMI)
//                |                 |
//                |             P1.6|<- Serial Clock In (UCA0CLK)
//                |                 |
//                |             P1.7|<- Master's GPIO (UCA0STE)
//
//   Nima Eskandari and Ryan Meredith
//   Texas Instruments Inc.
//   January 2018
//   Built with CCS V7.3
//******************************************************************************

/*
 * spi.c
 *
 *  Created on: Dec 31, 2020
 *      Author: kaanari
 *      UCA1SIMO: SPI Slave Input
 *      UCA1SOMI: SPI Slave Output
 *      UCA1CLK: SPI CLK
 */
uint8_t uca0_rx_val = 0;
volatile unsigned int spi_speed = 0;

int spi_test(){
    spi_config(0);

}



int spi_config(unsigned int spi_mode){

    switch(spi_mode){

        case 0: // Set as Slave

            P1SEL0 |= BIT4 | BIT5 | BIT6;             // P2.4-5-6 SPI Module Select
            P1SEL1 &= ~(BIT4 | BIT5 | BIT6);          // P2.4-5-6 SPI Module Select

            UCA0CTLW0 = UCSWRST;                      // **Put state machine in reset**

            UCA0CTLW0 |= CTL0_SPI_SLAVE;               // Slave Mode Setting
            UCA0CTLW0 &= ~UCSWRST;                     // **Initialize USCI state machine**
            UCA0IE |= UCRXIE;                        // Enable USCI1 RX interrupt

            return SUCCESS;

        case 1: // Set as Master

            P1SEL0 |= BIT4 | BIT5 | BIT6;             // P2.4-5-6 SPI Module Select
            P1SEL1 &= ~(BIT4 | BIT5 | BIT6);          // P2.4-5-6 SPI Module Select

            UCA0CTLW0 = UCSWRST;                      // **Put state machine in reset**

            UCA0CTLW0 |= CTL0_SPI_SLAVE;               // Slave Mode Setting
            UCA0CTLW0 &= ~UCSWRST;                     // **Initialize USCI state machine**
            //UCA0IE |= UCRXIE;                        // Enable USCI1 RX interrupt

            return SUCCESS;


        default:
            return SPI_SET_ERROR; // Wrong SPI_Mode


    }

}

void SendUCA0Data(uint8_t val)
{
    while (!(UCA0IFG & UCTXIFG));              // USCI_A0 TX buffer ready?
    UCA0TXBUF = val;
    __no_operation();
}


#pragma vector=USCI_A0_VECTOR
__interrupt void USCI0RX_ISR (void)
{

    spi_speed++;
    if(spi_speed == 0){
        __no_operation();
    }
    uca0_rx_val = UCA0RXBUF;
    uint8_t uca1_rx_val = 5;
    UCA0IFG &= ~UCRXIFG;
    SendUCA0Data(uca1_rx_val);
    __no_operation();
}

