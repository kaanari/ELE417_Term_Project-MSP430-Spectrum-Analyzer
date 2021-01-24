#include <timers.h>
#include <fix_fft.h>
/*
 * adc.c
 *
 *  Created on: Dec 31, 2020
 *      Author: kaanari
 */


volatile unsigned int currentIndex = 0;
volatile unsigned short currentValue = 0;
volatile signed char currentValueByte = 0;
volatile unsigned int value2 = 0;
unsigned char samplesReady = 0;
extern unsigned char busy;
extern unsigned char operationMode;
extern signed char im[1024];
extern signed char real[1024];

void adc_config(){

    ADCCTL0 |= ADCSHT_4 | ADCMSC;
    /* T_sample = 65kHz => (5 Mhz/64) | Continious Mode */

    ADCCTL1 |= ADCSHS_0 | ADCSHP | ADCDIV_0 | ADCSSEL_0 | ADCCONSEQ_2;
    /*  ADCSC Select | SAMPCON is sampling timer  | MODCLK/1 | Select MODCLK | Single Channel Continuous Mode*/

    //ADCCTL2 |= ADCPDIV_0 | ADCRES_1 | ADCDF;
    ADCCTL2 |= ADCPDIV_0 | ADCRES_0 | ADCDF;
    /* ADCCLK = MODCLK/1 (5 Mhz) | 10-bit Mode | Signed Mode (-Vref = 8000h +Vref = 7FC0h  */

    ADCMCTL0 |= ADCSREF_0 | ADCINCH_1;
    /* V+ = Vcc and V- = Vss | Channel A1 selected */

    ADCCTL0 |= ADCON;
    /* ADC ON */

    ADCIE |= ADCIE0;
    /* Interrupt Enable */


}

void adc_on(){
    ADCCTL0 |= ADCON;
    /* ADC ON */

    ADCIE |= ADCIE0;
    /* Interrupt Enable */

    ADCCTL0 |= ADCENC | ADCSC;
    /* ADC Enable Conversation and start conversation */

}

void adc_off(){
    ADCCTL0 &= ~ADCON;
        /* ADC OFF */

    ADCIE &= ~ADCIE0;
    /* Interrupt Disable */
}

void test_adc(void) {

    WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;                   // Disable the GPIO power-on default high-impedance mode
                                            // to activate previously configured port settings
    P1DIR |= 0x01;                          // Set P1.0 to output direction

    clock_config();
    adc_config();


    //TA0CTL |= TAIE; // Enable Timer A interrupt
    TA0CCTL0 |= CCIE; // Enable Timer A interrupt



    volatile unsigned int i = 40000;
    for(i=65000;i>0;i--){
        value2 += 1;
    }


    TA0CCR0 = 4096*1; // Set Counter
    TA0CTL |= TA0CONF;
    // Timer A config

    ADCCTL0 |= ADCENC | ADCSC;
    /* ADC Enable Conversation and start conversation */

    TA0CTL |= TACLR;
    // Timer A reset



    __bis_SR_register(GIE+LPM0_bits);


}

#pragma vector = ADC_VECTOR
__interrupt void ADC_ISR(void)
{


    if(busy){
        currentIndex = 0;
        currentValue = ADCMEM0;

    }else{

        currentValue = 0;
        currentValue = ADCMEM0;
        currentValue = currentValue>>8;
        currentValueByte = currentValue;

        if(operationMode == 1){
            unsigned int newIndex = findDecimationIndex(currentIndex,8);


            real[newIndex] = currentValueByte;
            //real[currentIndex] = currentValueByte;
            currentIndex++;


            if(currentIndex == 256){
                currentIndex = 0;
                samplesReady = 1;
            }
        }else if(operationMode == 2){

        }

    }





}



