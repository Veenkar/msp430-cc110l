/*
 * spi.h
 *
 *  Created on: 28-10-2013
 *      Author: Veenkar
 */

#ifndef CC110L_H_
#define CC110L_H_

#define CC110L_BUFFER_LENGTH 64

char RFReceiveByte();
void RFSendPacket(char *txBuffer, char size);
void writeRFSettings(void);
void sendbyte(char);
void setup_cc110l(void);



__interrupt void port2_isr (void );


#endif /* CC110L_H_ */
