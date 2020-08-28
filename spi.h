/*
 * spi.h
 *
 *  Created on: 26-10-2013
 *      Author: Veenkar
 */

#ifndef SPI_H_
#define SPI_H_

void SPIinit();
char SPIReadReg(char addr);
char SPIReadStatus(char addr);
void SPIWriteBurstReg(char addr, char *buffer, char count);
void SPIWriteReg(char addr, char value);
void Wait(unsigned int cycles);
void PowerupResetCC110L(void);
void SPIReadBurstReg(char addr, char *buffer, char count);
void SPIStrobe(char strobe);



#endif /* SPI_H_ */
