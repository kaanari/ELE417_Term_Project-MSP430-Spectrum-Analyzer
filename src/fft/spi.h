#include <msp430.h>
#include <errors.h>
/*
 * spi.h
 *
 *  Created on: Dec 31, 2020
 *      Author: kaanari
 */

#ifndef SPI_H_
#define SPI_H_

#define CTL0_SPI_SLAVE UCMSB | UCSYNC | UCMODE_0;
 // 3-pin, 8-bit SPI slave, First Send MSB

#define CTL0_SPI_MASTER UCMSB + UCSYNC + UCMST + UCSSEL_1
 // 3-pin, 8-bit SPI slave, First Send MSB, 8 Mhz Clock Select

int spi_config(unsigned int spi_modi);

__interrupt void USCI1RX_ISR (void);


#endif /* SPI_H_ */
