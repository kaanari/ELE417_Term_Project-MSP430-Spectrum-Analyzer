#include<msp430.h>
/*
 * adc.h
 *
 *  Created on: Dec 31, 2020
 *      Author: kaanari
 */

#ifndef ADC_H_
#define ADC_H_



extern volatile unsigned int ADC_BUFFER = 0;
extern volatile unsigned int ADC_BUFFER_READY = 0;


void adc_config();
void adc_on();
void adc_off();

void test_adc(void);

__interrupt void ADC_ISR(void);

#endif /* ADC_H_ */
