/*
 * spi.c
 *
 *  Created on: 26-10-2013
 *      Author: Veenkar
 */

#include "mspboard.h"
#include "msp430g2553.h"
#include "cc110l_reg.h"


void SPIinit(){

	  XTAL_SEL &= ~XIN_PIN & ~XOUT_PIN;	//launchpad
	  XTAL_SEL2 &= ~XIN_PIN & ~XOUT_PIN; //<=
	  GDO2_DIR &= ~GDO2_PIN;
	  GDO1_CP_DIR &= ~GDO1_CP_PIN;

	  SPI_UB0_SEL |= SPI_UB0_SIMO| SPI_UB0_SOMI| SPI_UB0_UCLK;
	  SPI_UB0_SEL2 |= SPI_UB0_SIMO| SPI_UB0_SOMI| SPI_UB0_UCLK;
	                                            // SPI option select
	  SPI_UB0_DIR |= SPI_UB0_SIMO | SPI_UB0_UCLK;
	  CSn_OUT |= CSn_PIN;
	  CSn_DIR |= CSn_PIN;         // /CS disable

	  UCB0CTL1 |= UCSWRST;                      // **Disable USCI state machine**
	  UCB0CTL0 |= UCMODE_0+UCMST+UCCKPH+UCMSB+UCSYNC;    // 3-pin, 8-bit SPI master
	  UCB0CTL1 |= UCSSEL_2;                     // SMCLK
	  UCB0BR0 = 1 && 0xff;				//0x02;              // UCLK/2
	  UCB0BR1 = 1 >> 8;
	                                              // SPI TXD out direction
	  UCB0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**


	  GDO0_IES  |=   GDO0_PIN;       // Int on falling edge (end of pkt)
	  GDO0_IFG  &=  ~GDO0_PIN;      // Clear flag
	  GDO0_IE   |=   GDO0_PIN;        // Enable int on end of packet



}


char SPIReadReg(char addr)
{
  char x;

  CSn_OUT &= ~CSn_PIN;               // /CS enable
  while((GDO2_IN & GDO2_PIN));       // Wait for chip ready
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = (addr | CC110L_READ_SINGLE);// Send address
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = 1;                            // Dummy write so we can read data
  while (UCB0STAT & UCBUSY);                // Wait for TX to complete
  x = UCB0RXBUF;                            // Read data
  CSn_OUT |= CSn_PIN;         // /CS disable
  return x;

}


void SPIStrobe(char strobe)
{
  CSn_OUT &= ~CSn_PIN;        // /CS enable
  while((GDO2_IN & GDO2_PIN));       // Wait for chip ready
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = strobe;                       // Send strobe
  // Strobe addr is now being TX'ed
  while (UCB0STAT & UCBUSY);                // Wait for TX to complete
  CSn_OUT |= CSn_PIN;         // /CS disable
}

void SPIReadBurstReg(char addr, char *buffer, char count)
{
  char i;
  CSn_OUT &= ~CSn_PIN;        // /CS enable
  while((GDO2_IN & GDO2_PIN));       // Wait for chip ready
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = (addr | CC110L_READ_BURST);// Send address
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  for (i = 0; i < count; i++)
  {
    UCB0TXBUF = 1;                          //Initiate next data RX, meanwhile..
	while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
    buffer[i] = UCB0RXBUF;                  // Store data from last data RX
  }
  while (UCB0STAT & UCBUSY);                // Wait for TX to complete
  CSn_OUT |= CSn_PIN;         // /CS disable
}



char SPIReadStatus(char addr)
{
  char x;

  CSn_OUT &= ~CSn_PIN;               // /CS enable
  while((GDO2_IN & GDO2_PIN));       // Wait for chip ready
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = (addr | CC110L_READ_BURST);// Send address
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = 1;                            // Dummy write so we can read data
  while (UCB0STAT & UCBUSY);                // Wait for TX to complete
  x = UCB0RXBUF;                            // Read data
  CSn_OUT |= CSn_PIN;         // /CS disable
  return x;

}


void SPIWriteReg(char addr, char value)
{
  CSn_OUT &= ~CSn_PIN;        // /CS enable
  while((GDO2_IN & GDO2_PIN));       // Wait for chip ready
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = addr;                         // Send address
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = value;                        // Send data
  while (UCB0STAT & UCBUSY);                // Wait for TX to complete
  CSn_OUT |= CSn_PIN;         // /CS disable
}


char SPIWriteRegCheck(char addr, char value)
{
  CSn_OUT &= ~CSn_PIN;        // /CS enable
  while((GDO2_IN & GDO2_PIN));       // Wait for chip ready
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = addr;                         // Send address
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = value;                        // Send data
  while (UCB0STAT & UCBUSY);                // Wait for TX to complete
  CSn_OUT |= CSn_PIN;         // /CS disable
  return SPIReadReg(addr);
}




void SPIWriteBurstReg(char addr, char *buffer, char count)
{
  unsigned int i;

  CSn_OUT &= ~CSn_PIN;        // /CS enable
  while((GDO2_IN & GDO2_PIN));       // Wait for chip ready
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = addr | CC110L_WRITE_BURST; // Send address
  for (i = 0; i < count; i++)
  {
    while (!(IFG2&UCB0TXIFG));              // Wait for TXBUF ready
    UCB0TXBUF = buffer[i];                  // Send data
  }
  while (UCB0STAT & UCBUSY);                // Wait for TX to complete
  CSn_OUT |= CSn_PIN;         // /CS disable
}

void Wait(unsigned int cycles)
{
  while(cycles>15)                          // 15 cycles consumed by overhead
    cycles = cycles - 6;                    // 6 cycles consumed each iteration
}

void PowerupResetCC110L(void)
{
  CSn_OUT |= CSn_PIN;
  Wait(60);
  CSn_OUT &= ~CSn_PIN;
  Wait(60);
  CSn_OUT |= CSn_PIN;
  Wait(60);
  CSn_OUT &= ~CSn_PIN;        // /CS enable
  while (!(IFG2&UCB0TXIFG));                // Wait for TXBUF ready
  UCB0TXBUF = CC110L_SRES;               // Send strobe
  // Strobe addr is now being TX'ed
  while (UCB0STAT & UCBUSY);                // Wait for TX to complete
  CSn_OUT |= CSn_PIN;         // /CS disable
}




