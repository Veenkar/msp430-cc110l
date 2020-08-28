#include "cc110l.h"
#include "spi.h"
#include "cc110l_reg.h"
#include "mspboard.h"
#include "msp430g2553.h"
#include "uart.h"

//GFSK_38Kbps_18KHz_70KHzRxBW_-0.5dBm
char rxBuffer[CC110L_BUFFER_LENGTH];

void setup_cc110l(void)
{
PowerupResetCC110L();
writeRFSettings();
SPIStrobe(CC110L_SRX);

}
void writeRFSettings(void)
{
    // Write register settings
    SPIWriteReg(CC110L_IOCFG2,   0x29); // GDO2 output pin config.`CHIP_RDYn
    SPIWriteReg(CC110L_IOCFG1,   0x2E); // GDO1 output pin config.`HIGH IMP
    SPIWriteReg(CC110L_IOCFG0,   0x06); // GDO0 output pin config.`high on rx and tx
    SPIWriteReg(CC110L_FIFOTHR,  0x07); // THR.`32 bytes RX ; 33 bytes TX fifo
    SPIWriteReg(CC110L_SYNC1,    0xD3); // Packet automation control.`
    SPIWriteReg(CC110L_SYNC0,    0x91); // Packet automation control.`


    SPIWriteReg(CC110L_PKTLEN,   0x01); // Packet length.`
    SPIWriteReg(CC110L_PKTCTRL1, 0x00); // Packet automation control.`flush when CRC not ok, no APPEND STATUS
    SPIWriteReg(CC110L_PKTCTRL0, 0x04); // Packet automation control.`normal mode ,CRC enabled , fixed packet length

    /*
    SPIWriteReg(CC110L_PKTLEN,   0xFF); // Packet length.`
    SPIWriteReg(CC110L_PKTCTRL1, 0x04); // Packet automation control.`flush when CRC not ok
    SPIWriteReg(CC110L_PKTCTRL0, 0x05); // Packet automation control.`normal mode ,CRC enabled , variable packet length
    */

    SPIWriteReg(CC110L_ADDR,     0x00); // Device address.`
    SPIWriteReg(CC110L_CHANNR,   0x00); // Channel number.
    SPIWriteReg(CC110L_FSCTRL1,  0x06); // Freq synthesizer control.`
    SPIWriteReg(CC110L_FSCTRL0,  0x00); // Freq synthesizer control.`
    SPIWriteReg(CC110L_FREQ2,    0x20); // Freq control word, high byte`
    SPIWriteReg(CC110L_FREQ1,    0x25); // Freq control word, mid byte.`
    SPIWriteReg(CC110L_FREQ0,    0xED); // Freq control word, low byte.`
    SPIWriteReg(CC110L_MDMCFG4,  0xEA); // Modem configuration.`
    SPIWriteReg(CC110L_MDMCFG3,  0x71); // Modem configuration.`
    SPIWriteReg(CC110L_MDMCFG2,  0x13); // Modem configuration.`
    SPIWriteReg(CC110L_MDMCFG1,  0x20); // Modem configuration.`
    SPIWriteReg(CC110L_MDMCFG0,  0xF8); // Modem configuration.`
    SPIWriteReg(CC110L_DEVIATN,  0x33); // Modem dev (when FSK mod en)`??
    SPIWriteReg(CC110L_MCSM2,    0x07); //MainRadio Cntrl State Machine`
    SPIWriteReg(CC110L_MCSM1 ,   0x3F); //MainRadio Cntrl State Machine`stay in tx
    SPIWriteReg(CC110L_MCSM0 ,   0x18); //MainRadio Cntrl State Machine`
    SPIWriteReg(CC110L_FOCCFG,   0x16); // Freq Offset Compens. Config`??
    SPIWriteReg(CC110L_BSCFG,    0x6C); //  Bit synchronization config.`??
    SPIWriteReg(CC110L_AGCCTRL2, 0x43); // AGC control.`
    SPIWriteReg(CC110L_AGCCTRL1, 0x4F); // AGC control.`
    SPIWriteReg(CC110L_AGCCTRL0, 0x91); // AGC control.`
    SPIWriteReg(CC110L_WOREVT1,  0x00); // WOR ??
    SPIWriteReg(CC110L_WOREVT0,  0x00); // WOR ??
    SPIWriteReg(CC110L_WORCTRL,  0xF8); // WOR ??
    SPIWriteReg(CC110L_FREND1,   0x56); // Front end RX configuration.`
    SPIWriteReg(CC110L_FREND0,   0x10); // Front end RX configuration.`
    SPIWriteReg(CC110L_FSCAL3,   0xE9); // Frequency synthesizer cal.`
    SPIWriteReg(CC110L_FSCAL2,   0x2A); // Frequency synthesizer cal.`
    SPIWriteReg(CC110L_FSCAL1,   0x00); // Frequency synthesizer cal.`
    SPIWriteReg(CC110L_FSCAL0,   0x1F); // Frequency synthesizer cal.`
    SPIWriteReg(CC110L_RCCTRL1,  0x00); // Packet automation control.`
    SPIWriteReg(CC110L_RCCTRL0,  0x00); // Packet automation control.`
    SPIWriteReg(CC110L_FSTEST,   0x59); // Frequency synthesizer cal.
    SPIWriteReg(CC110L_PSTEST,   0x7F); // Frequency synthesizer cal.
    SPIWriteReg(CC110L_AGCTEST,  0x3F); // Frequency synthesizer cal.
    SPIWriteReg(CC110L_TEST2,    0x81); // Various test settings.`
    SPIWriteReg(CC110L_TEST1,    0x35); // Various test settings.`
    SPIWriteReg(CC110L_TEST0,    0x09); // Various test settings.`

}

// PATABLE (-0.5 dBm output power)
extern char paTable[] = {0x70};   // -0.5 dBm
extern char paTableLen = 1;

//-----------------------------------------------------------------------------
//  void RFSendPacket(char *txBuffer, char size)
//
//  DESCRIPTION:
//  This function transmits a packet with length up to 63 bytes.  To use this
//  function, GD00 must be configured to be asserted when sync word is sent and
//  de-asserted at the end of the packet, which is accomplished by setting the
//  IOCFG0 register to 0x06, per the CCxxxx datasheet.  GDO0 goes high at
//  packet start and returns low when complete.  The function polls GDO0 to
//  ensure packet completion before returning.
//
//  ARGUMENTS:
//      char *txBuffer
//          Pointer to a buffer containing the data to be transmitted
//
//      char size
//          The size of the txBuffer
//-----------------------------------------------------------------------------



void sendbyte(char a)
{
char Buffer[1];
Buffer[0] = a;    					 // Load four switch inputs
RFSendPacket(Buffer, 1);                 // Send value over RF
}

/*
void sendbyte(char a)
{
char Buffer[3];

Buffer[0] = 2;                           // Packet length
Buffer[1] = 0x01;                        // Packet address
Buffer[2] = a;    					 // Load four switch inputs
RFSendPacket(Buffer, 3);                 // Send value over RF
}
*/


void RFSendPacket(char *txBuffer, char size)
{
  //_EINT();
  GDO0_IE &= ~GDO0_PIN;          // Disable interrupt
  SPIWriteBurstReg(CC110L_TXFIFO, txBuffer, size); // Write TX data
  SPIStrobe(CC110L_STX);           // Change state to TX, initiating data transfer
  while (!(GDO0_IN & GDO0_PIN));// Wait GDO0 to go hi -> sync TX'ed
  while (GDO0_IN&GDO0_PIN);   // Wait GDO0 to clear -> end of pkt
  GDO0_IFG &= ~GDO0_PIN;      // After pkt TX, this flag is set.
  GDO0_IE |= GDO0_PIN;
                                            // Has to be cleared before existing

  
}



//-----------------------------------------------------------------------------
//  char RFReceivePacket(char *rxBuffer, char *length)
//
//  DESCRIPTION:
//  Receives a packet of variable length (first byte in the packet must be the
//  length byte).  The packet length should not exceed the RXFIFO size.  To use
//  this function, APPEND_STATUS in the PKTCTRL1 register must be enabled.  It
//  is assumed that the function is called after it is known that a packet has
//  been received; for example, in response to GDO0 going low when it is
//  configured to output packet reception status.
//
//  The RXBYTES register is first read to ensure there are bytes in the FIFO.
//  This is done because the GDO signal will go high even if the FIFO is flushed
//  due to address filtering, CRC filtering, or packet length filtering.
//
//  ARGUMENTS:
//      char *rxBuffer
//          Pointer to the buffer where the incoming data should be stored
//      char *length
//          Pointer to a variable containing the size of the buffer where the
//          incoming data should be stored. After this function returns, that
//          variable holds the packet length.
//
//  RETURN VALUE:
//      char
//          0x80:  CRC OK
//          0x00:  CRC NOT OK (or no pkt was put in the RXFIFO due to filtering)
//-----------------------------------------------------------------------------
char RFReceiveByte()
{
  char length=1;
  char rxBuffer[1];

  if ((SPIReadStatus(CC110L_RXBYTES) & CC110L_NUM_RXBYTES)>=length)
  {

		  SPIReadBurstReg(CC110L_RXFIFO, rxBuffer, length); // burst?
		  return *rxBuffer;
  }
  return 0;

}

/*
char RFReceivePacket(char *rxBuffer, char *length)
{
  char status[2];
  char pktLen;

  if ((SPIReadStatus(CC110L_RXBYTES) & CC110L_NUM_RXBYTES))
  {
		pktLen = SPIReadReg(CC110L_RXFIFO); // Read length byte
		if (pktLen <= *length)                  // If pktLen size <= rxBuffer
		{
		  SPIReadBurstReg(CC110L_RXFIFO, rxBuffer, pktLen); // Pull data
		  *length = pktLen;                     // Return the actual size
		  SPIReadBurstReg(CC110L_RXFIFO, status, 2);
												// Read appended status bytes
		  return (char)(status[CC110L_LQI_RX]&CC110L_CRC_OK);
		}                                       // Return CRC_OK bit
		else
		{
		  *length = pktLen;                     // Return the large size
		  SPIStrobe(CC110L_SFRX);      // Flush RXFIFO
		  return 0;                             // Error
		}
  }
  else
      return 0;                             // Error
}
*/


/*
#pragma vector=PORT2_VECTOR
__interrupt void port2_isr (void )
{
	GDO0_IFG &= ~ GDO0_PIN;
  char length = CC110L_BUFFER_LENGTH;
 if(GDO0_IFG & GDO0_PIN)
  {
    if(RFReceivePacket(rxBuffer, &length))
      {
      UARTsendHex(rxBuffer[1]);

      rxBuffer[0]=0;                //clear buffer
      rxBuffer[1]=0;
      }
    }

}
*/


#pragma vector=PORT2_VECTOR
__interrupt void port2_isr(void)
{

	char recieve[]="   recieve:   ";
	char c;
	//char length = CC110L_BUFFER_LENGTH;
	if(GDO0_IFG & GDO0_PIN)
	{
		c=RFReceiveByte();
		if(c)
		{
	    	UARTsendStr(recieve);
	    	UARTsendHex(c);
		}
	}
	GDO0_IFG &= ~ GDO0_PIN;

}

