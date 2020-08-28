#include <msp430.h> 
#include "mspboard.h"
#include "uart.h"
#include "spi.h"
#include "cc110l.h"

/*
 * main.c
 */
void main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    UARTinit();
    SPIinit();
    PowerupResetCC110L();
    writeRFSettings();

    char hello[]="   hello ";
    char partn[]=" regread: ";
    char recieve[]="   recieve:   ";

    UARTsendStr(hello);
    UARTsendStr(partn);

    UARTsendHex(SPIReadStatus(0x31));

    sendbyte(0x22);

    while(1){
    	__delay_cycles(1000000);
    	UARTsendStr(recieve);
    	UARTsendHex(RFReceiveByte());
    }

    __bis_SR_register(LPM0_bits + GIE); // Enter LPM0, interrupts enabled
}


