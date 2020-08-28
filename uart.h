/*
 * uart.h
 *
 *  Created on: 26-10-2013
 *      Author: Veenkar
 */

#ifndef UART_H_
#define UART_H_

//===================================FUNKCJE====================================
void UARTinit();
void UARTsendChar(char c);
void UARTsendStr(char* str);
void UARTsendHex(char c);

//==================================PRZERWANIA==================================
__interrupt void USCI0RX_ISR(void);

//==============================================================================

#endif /* UART_H_ */
