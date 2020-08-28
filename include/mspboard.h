#ifndef MSPBOARD_H_
#define MSPBOARD_H_


//======================================================

#define LED_OUT         P2OUT
#define LED_DIR         P2DIR
#define LED_1            BIT0
#define LED_2            BIT1
#define LED_3            BIT2
#define LED_4            BIT3

#define SWI_IN          P2IN
#define SWI_DIR         P2DIR
#define SWI_IE          P2IE
#define SWI_IES         P2IES
#define SWI_IFG         P2IFG
#define SWI_REN         P2REN
#define SWI_1           BIT4
#define SWI_2           BIT5

#define GDO0_IN         P2IN	//launcpad
#define GDO0_OUT        P2OUT
#define GDO0_DIR        P2DIR
#define GDO0_IE         P2IE
#define GDO0_IES        P2IES
#define GDO0_IFG        P2IFG
#define GDO0_PIN        BIT6


#define XTAL_SEL    P2SEL
#define XTAL_SEL2   P2SEL2
#define XIN_PIN     BIT6
#define XOUT_PIN    BIT7


#define GDO1_IN         P1IN
#define GDO1_OUT        P1OUT
#define GDO1_DIR        P1DIR
#define GDO1_PIN        BIT6

#define GDO1_CP_IN         P2IN
#define GDO1_CP_OUT        P2OUT
#define GDO1_CP_DIR        P2DIR
#define GDO1_CP_PIN        BIT5

#define GDO2_IN         P1IN  //launchpad
#define GDO2_OUT        P1OUT
#define GDO2_DIR        P1DIR
#define GDO2_PIN        BIT0

#define CSn_OUT         P2OUT // launchpad
#define CSn_DIR         P2DIR
#define CSn_PIN         BIT7


#define SPI_UB0_SEL   P1SEL
#define SPI_UB0_SEL2  P1SEL2
#define SPI_UB0_DIR   P1DIR
#define SPI_UB0_IN    P1IN
#define SPI_UB0_SIMO    BIT7
#define SPI_UB0_SOMI    BIT6
#define SPI_UB0_UCLK    BIT5


//======================================================

/*

#define LED_OUT         P2OUT
#define LED_DIR         P2DIR
#define LED_1            BIT0
#define LED_2            BIT1
#define LED_3            BIT2
#define LED_4            BIT3

#define SWI_IN          P2IN
#define SWI_DIR         P2DIR
#define SWI_IE          P2IE
#define SWI_IES         P2IES
#define SWI_IFG         P2IFG
#define SWI_REN         P2REN
#define SWI_1           BIT4
#define SWI_2           BIT5

#define GDO0_IN         P1IN
#define GDO0_OUT        P1OUT
#define GDO0_DIR        P1DIR
#define GDO0_IE         P1IE
#define GDO0_IES        P1IES
#define GDO0_IFG        P1IFG
#define GDO0_PIN        BIT4

#define GDO1_IN         P1IN
#define GDO1_OUT        P1OUT
#define GDO1_DIR        P1DIR
#define GDO1_PIN          BIT6

#define GDO2_IN         P1IN
#define GDO2_OUT        P1OUT
#define GDO2_DIR        P1DIR
#define GDO2_PIN        BIT3

#define CSn_OUT         P1OUT
#define CSn_DIR         P1DIR
#define CSn_PIN           BIT0


#define SPI_UB0_SEL   P1SEL
#define SPI_UB0_SEL2  P1SEL2
#define SPI_UB0_DIR   P1DIR
#define SPI_UB0_IN    P1IN
#define SPI_UB0_SIMO    BIT7
#define SPI_UB0_SOMI    BIT6
#define SPI_UB0_UCLK    BIT5

*/



#endif /* MSPBOARD_H_*/
