#include <timers.h>
/*
 * timers.c
 *
 *  Created on: Dec 31, 2020
 *      Author: kaanari
 */
#define TA0CONF TASSEL_1 + MC_2 + ID_3
int valuex = 0;

#define SEND BIT6
#define A0 BIT0
#define A1 BIT1
#define A2 BIT2
#define ACK BIT7


extern unsigned char operationMode;
extern signed char im[1024];
extern signed char real[1024];
unsigned char busy = 0;
volatile unsigned char delayFlag = 0;
extern unsigned char samplesReady;
volatile unsigned char readValue = 0;

volatile unsigned char spectrumType = 0;
int timerA_config(void){

    //TA0CTL |= TAIE; // Enable Timer A interrupt
    TA0CCTL2 |= CCIE; // Enable Timer A interrupt

/*

    volatile unsigned int i = 40000;
    for(i=65000;i>0;i--){
        valuex += 1;
    }
*/

    TA0CCR2 = 4096*1; // Set Counter
    TA0CTL |= TA0CONF;
    // Timer A config

    //ADCCTL0 |= ADCENC | ADCSC;
    /* ADC Enable Conversation and start conversation */

    TA0CTL |= TACLR;
    // Timer A reset
    __bis_SR_register(GIE);
    return 0;
}


int delay_ms(unsigned short ms){

    TA0CCTL1 |= CCIE; // Enable Timer A interrupt



    //volatile unsigned int i = 40000;
    //for(i=65000;i>0;i--){
    //    valuex += 1;
    //}


    if(ms > 0x3FFF){
        int i = 0;
        unsigned int count = ms>>14;
        unsigned int remainder = ms && 0x3FFF;
        for(;i<count;i++){
            delay_ms(0x3FFF);
        }
        delay_ms(remainder);
        return 0;
    }


    TA0CCR1 = ms<<2; // Set Counter
    //TA0CTL |= TA0CONF;
    // Timer A config

    //ADCCTL0 |= ADCENC | ADCSC;
    /* ADC Enable Conversation and start conversation */
    //TA0CTL &= ~TAIFG;
    TA0CTL |= TACLR;
    //A0CCTL1 &= ~CCIFG;
    // Timer A reset

    delayFlag = 0;
    __bis_SR_register(GIE+LPM0_bits);
    //while(!(delayFlag));



    return 0;


}

int spectrum_draw(unsigned short ms){

    TA0CCTL2 |= CCIE; // Enable Timer A interrupt

    TA0CCTL1 & ~CCIE; // Enable Timer A interrupt


    TA0CCR2 = ms<<2; // Set Counter
    //TA0CTL |= TA0CONF;
    // Timer A config


    TA0CTL |= TACLR;
    //A0CCTL1 &= ~CCIFG;
    // Timer A reset

    __bis_SR_register(GIE+LPM0_bits);
    //while(!(delayFlag));



    return 0;


}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void TimerA_ISR()
{

    switch(__even_in_range(TA0IV,TA0IV_TAIFG))
        {
            case TA0IV_NONE:
                break;                                  // No interrupt

            case TA0IV_TACCR1:
                delayFlag = 1;
                __bic_SR_register_on_exit(LPM0_bits);
                break;                                  // CCR1 not used
            case TA0IV_TACCR2:


                P1OUT ^= BIT0;

                /*
                if((P1OUT & BIT0)){
                    draw6x8Str(20, 2, "N", 1, 0);
                }else{
                    draw6x8Str(20, 2, "A", 1, 0);
                }
                */

                if(samplesReady != 1){
                   break;
                }
                valuex += 1;

                busy=1;


                P1OUT ^= BIT0;

                 P1OUT |= ACK;

                unsigned char max_mag = spectrum(real,im);

                while(!SEND);
                 readValue = (P2IN & (A0|A1|A2));
                 //readValue = readValue>>2;
                 P1OUT &= ~ACK;

                 if(readValue){
                     if(readValue == 7){
                         WDTCTL = 0x00;
                     }
                     else if(readValue == 5){
                         __no_operation();
                         __bic_SR_register_on_exit(LPM0_bits);
                         break;
                     }
                     else if(readValue == 2){
                         changePage(1);
                         spectrumType = 1;
                     }else if(readValue == 1){
                         changePage(0);
                         spectrumType = 0;
                     }
                 }

                spectrumScreen(real,max_mag);



                __no_operation();

                for(valuex = 0; valuex<255;valuex++)
                    im[valuex] = 0;

                TA0CTL |= TACLR;

                busy = 0;
                samplesReady = 0;
                break;
            case TA0IV_TAIFG:
                break;                                  // overflow
            default:
                break;
        }



}
