//******************************************************************************
//  MSP430FR243x Demo - eUSCI_B0 I2C 4 Hardware I2C slaves
//
//  Description: This demo connects two MSP430's via the I2C bus.
//  This code configures the MSP430 USCI to be addressed as 4 independent I2C
//  slaves. Each slave has its own interrupt flag and data variable to store
//  incoming data.
//  Use with msp430fr243x_uscib0_i2c_15.c
//  ACLK = REFO = 32768Hz, MCLK = SMCLK = default  = DCO = ~1MHz
//
//                                /|\  /|\
//               MSP430FR2433      10k  10k     MSP430FR2433
//                   slave         |    |        master
//             -----------------   |    |   -----------------
//            |     P1.2/UCB0SDA|<-|----|->|P1.2/UCB0SDA     |
//            |                 |  |       |                 |
//            |                 |  |       |                 |
//            |     P1.3/UCB0SCL|<-|------>|P1.3/UCB0SCL     |
//            |                 |          |                 |
//
//  Wei Zhao
//  Texas Instruments Inc.
//  Jan 2014
//  Built with IAR Embedded Workbench v6.20 & Code Composer Studio v6.0.1
//******************************************************************************
#include <msp430.h>
#define RST BIT0
#define SET BIT1
#define MID BIT2
#define RHT BIT4
#define LFT BIT5
#define DOWN BIT6
#define UP BIT7

#define LED1 BIT0
#define LED2 BIT1

unsigned char RXData0=0;
unsigned char RXData1=0;
unsigned char RXData2=0;
unsigned char RXData3=0;

#define SEND BIT6
#define A0 BIT2
#define A1 BIT3
#define A2 BIT4
#define ACK BIT7

volatile unsigned char BUTTONS = 0;

volatile unsigned char TXData;


void main(void) {

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings

    P2DIR &= ~(RST | SET | MID | RHT | LFT | DOWN | UP); // Set P2.3 as input
    P2REN |= (RST | SET | MID | RHT | LFT | DOWN | UP);  // Resistor enable
    P2OUT &= ~(RST | SET | MID | RHT | LFT | DOWN | UP);  // Pull-Down resistor selection
    P2IE  |= (RST | SET | MID | RHT | LFT | DOWN | UP);
    P2IES &= ~(RST | SET | MID | RHT | LFT | DOWN | UP);  // Positive Edge Trigger

    P1DIR &= ~(ACK);
    P1DIR |= (A0 | A1 | A2 | SEND);
    P1IE |= (ACK);
    P1IES &= ~ACK;

    //P1DIR &= ~(RST | SET | MID | RHT | LFT | DOWN | UP); // Set P2.3 as input

    P1DIR |= LED1 | LED2;     // Set P1.0 as output

    P1OUT &= ~(LED1 | LED2);    // Initially turn off the LED
/*
    // Configure Pins for I2C
        P1SEL0 |= BIT2 | BIT3;                                // I2C pins

        // Disable the GPIO power-on default high-impedance mode
        // to activate previously configured port settings
        PM5CTL0 &= ~LOCKLPM5;

        // Configure USCI_B0 for I2C mode
          UCB0CTLW0 = UCSWRST;                      // Software reset enabled
          UCB0CTLW0 |= UCMODE_3 | UCSYNC;           // I2C mode, sync mode
          UCB0I2COA0 = 0x0A | UCOAEN;               // own address is 0x48 + enable
          UCB0CTLW0 &= ~UCSWRST;                    // clear reset register
          UCB0IE |= UCTXIE0 | UCSTPIE;              // transmit,stop interrupt enable
*/
    __bis_SR_register(GIE+LPM0_bits);
    // Interrupt enable and CPU sleep

}


#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
        BUTTONS = BUTTONS << 2;
        P1OUT &= ~(A0 | A1 | A2);
        P1OUT |= (BUTTONS);



        P1OUT |= SEND;


        __delay_cycles(5000);

        while(P1IN & ACK);
        P1OUT &= ~SEND;

        __no_operation();


        P1IFG &= ~ACK;
        BUTTONS = 0;


}

#pragma vector = PORT2_VECTOR
__interrupt void PORT2_ISR(void)
{
/*
    if((P2IFG & RST)){

        P1OUT ^= LED1;
        __delay_cycles(50000);
        BUTTONS |= RST;
        P2IFG &= ~RST;
    }
    */
    if((P2IFG & RST) && !(P2IES & RST) ){

        P2IES |= RST;
        __delay_cycles(5000);
        P2IFG &= ~RST;
    }
    else if((P2IFG & RST) && (P2IES & RST) ){

        P1OUT ^= LED1;
        P2IES &= ~RST;
        __delay_cycles(50000);
        //BUTTONS |= RST;
        BUTTONS = 7;
        P2IFG &= ~RST;
    }

    else if((P2IFG & SET) && !(P2IES & SET) ){

        P2IES |= SET;
        __delay_cycles(5000);
        P2IFG &= ~SET;
    }
    else if((P2IFG & SET) && (P2IES & SET) ){

        P1OUT ^= LED1;
        P2IES &= ~SET;
        __delay_cycles(50000);
        //BUTTONS |= SET;
        BUTTONS = 6;
        P2IFG &= ~SET;
    }

    else if((P2IFG & MID) && !(P2IES & MID) ){

        P2IES |= MID;
        __delay_cycles(5000);
        P2IFG &= ~MID;
    }
    else if((P2IFG & MID) && (P2IES & MID) ){

            P1OUT ^= LED1;
            P2IES &= ~MID;
        __delay_cycles(50000);
        //BUTTONS |= MID;
        BUTTONS = 5;
        P2IFG &= ~MID;
    }

    else if((P2IFG & RHT) && !(P2IES & RHT) ){

        P2IES |= RHT;
        __delay_cycles(5000);
        P2IFG &= ~RHT;
    }
    else if((P2IFG & RHT) && (P2IES & RHT) ){

        P1OUT ^= LED1;
        P2IES &= ~RHT;
        __delay_cycles(50000);
        //BUTTONS |= RHT;
        BUTTONS = 4;
        P2IFG &= ~RHT;
    }

    else if((P2IFG & LFT) && !(P2IES & LFT) ){


        P2IES |= LFT;
        __delay_cycles(5000);
        P2IFG &= ~LFT;
    }
    else if((P2IFG & LFT) && (P2IES & LFT) ){

        P1OUT ^= LED1;
        P2IES &= ~LFT;
        __delay_cycles(50000);
        //BUTTONS |= LFT;
        BUTTONS = 3;
        P2IFG &= ~LFT;
    }

    else if((P2IFG & DOWN) && !(P2IES & DOWN) ){


        P2IES |= DOWN;
        __delay_cycles(5000);
        P2IFG &= ~DOWN;
    }
    else if((P2IFG & DOWN) && (P2IES & DOWN) ){

        P1OUT ^= LED2;
        P2IES &= ~DOWN;
        __delay_cycles(50000);
        //BUTTONS |= DOWN;
        BUTTONS = 2;
        P2IFG &= ~DOWN;
    }

    else if((P2IFG & UP) && !(P2IES & UP) ){


        P2IES |= UP;
        __delay_cycles(5000);
        P2IFG &= ~UP;
    }
    else if((P2IFG & UP) && (P2IES & UP) ){

        P1OUT ^= LED1;
        P2IES &= ~UP;
        __delay_cycles(50000);
        //BUTTONS |= UP;
        BUTTONS = 1;
        P2IFG &= ~UP;
    }



}






#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCIB0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCIB0_ISR (void)
#else
#error Compiler not supported!
#endif
{
  switch(__even_in_range(UCB0IV, USCI_I2C_UCBIT9IFG))
  {
    case USCI_NONE: break;                  // Vector 0: No interrupts
    case USCI_I2C_UCALIFG: break;           // Vector 2: ALIFG
    case USCI_I2C_UCNACKIFG: break;         // Vector 4: NACKIFG
    case USCI_I2C_UCSTTIFG: break;          // Vector 6: STTIFG
    case USCI_I2C_UCSTPIFG:                 // Vector 8: STPIFG
      TXData = 0;
      UCB0IFG &= ~UCSTPIFG;                 // Clear stop condition int flag
      break;
    case USCI_I2C_UCRXIFG3: break;          // Vector 10: RXIFG3
    case USCI_I2C_UCTXIFG3: break;          // Vector 14: TXIFG3
    case USCI_I2C_UCRXIFG2: break;          // Vector 16: RXIFG2
    case USCI_I2C_UCTXIFG2: break;          // Vector 18: TXIFG2
    case USCI_I2C_UCRXIFG1: break;          // Vector 20: RXIFG1
    case USCI_I2C_UCTXIFG1: break;          // Vector 22: TXIFG1
    case USCI_I2C_UCRXIFG0: break;          // Vector 24: RXIFG0
    case USCI_I2C_UCTXIFG0:
       UCB0TXBUF = BUTTONS;
       BUTTONS = 0;

       break;                               // Vector 26: TXIFG0
    case USCI_I2C_UCBCNTIFG: break;         // Vector 28: BCNTIFG
    case USCI_I2C_UCCLTOIFG: break;         // Vector 30: clock low timeout
    case USCI_I2C_UCBIT9IFG: break;         // Vector 32: 9th bit
    default: break;
  }
}



