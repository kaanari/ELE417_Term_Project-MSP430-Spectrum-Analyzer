#include <msp430.h>
/*
 * timers.h
 *
 *  Created on: Dec 31, 2020
 *      Author: kaanari
 */

#ifndef TIMERS_H_
#define TIMERS_H_

#define TA0CONF TASSEL_1 + MC_1 + ID_3

int timerA_config(void);
__interrupt void TimerA_ISR();

#endif /* TIMERS_H_ */
