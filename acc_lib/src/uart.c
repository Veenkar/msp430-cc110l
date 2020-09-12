/*
 * uart.c
 *
 *  Created on: 26-10-2013
 *      Author: Veenkar
 */
#include "msp430g2553.h"
#include "uart.h"

//===================================FUNKCJE====================================
void UARTinit()
{
	BCSCTL1 = CALBC1_1MHZ; // Set DCO
	DCOCTL = CALDCO_1MHZ;

	P1SEL  |= BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD
	P1SEL2 |= BIT1 + BIT2 ; // P1.1 = RXD, P1.2=TXD

	UCA0CTL1 |= UCSSEL_2; // SMCLK
	UCA0BR0 = 104; // 1MHz 9600
	UCA0BR1 = 0; // 1MHz 9600
	UCA0MCTL = UCBRS0; // Modulation UCBRSx = 1
	UCA0CTL1 &= ~UCSWRST; // **Initialize USCI state machine**

	IE2 |= UCA0RXIE; // Enable USCI_A0 RX interrupt
}

void UARTsendStr(char* str){
	char c;
	int i;
	for(i=0;(c=str[i])!=0;i++)
	{
		UARTsendChar(c);
	}
}

void UARTsendChar(char c){
	while (!(IFG2&UCA0TXIFG));
		UCA0TXBUF = c;
}

void UARTsendHex(char c){
	char c1=(c>>4);
	char c2=(c & 0x0F);


	UARTsendChar('0');
	UARTsendChar('x');

	if(c1<=9)
		UARTsendChar(c1+'0');
	else
		UARTsendChar(c1+'7');

	if(c2<=9)
		UARTsendChar(c2+'0');
	else
		UARTsendChar(c2+'7');


}



//==================================PRZERWANIA==================================
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
	while (!(IFG2&UCA0TXIFG)); // USCI_A0 TX buffer ready?
	UCA0TXBUF = UCA0RXBUF; // Echo back received character
}


//==============================================================================


