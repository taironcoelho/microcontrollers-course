#include <msp430.h> 

#define RXD        BIT1

void ConfigureMicro(void);
void DelayEstable(void);
/**
 * main.c
 */
int main(void)
{
    ConfigureMicro();
	__enable_interrupt();

	DelayEstable();

	while(1){
	    __low_power_mode_0();
	}
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    if (UCA0RXBUF == '3') // '3' received?
    {
        P2OUT ^= BIT4;
    }
    if (UCA0RXBUF == '4') // '4' received?
    {
        P2OUT ^= BIT5;
    }
    if (UCA0RXBUF == '5') // '5' received?
    {
        P2OUT ^= BIT6;
    }
    if (UCA0RXBUF == '6') // '6' received?
    {
        P2OUT ^= BIT7;
    }
}

void ConfigureMicro(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    BCSCTL1 = CALBC1_1MHZ;            // Set DCO to 1 MHz
    DCOCTL = CALDCO_1MHZ;

    P1SEL = RXD;                // Select TX and RX functionality for P1.1 & P1.2
    P1SEL2 = RXD;

    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;             // Use SMCLK: 1MHz

    UCA0BR0 = 104;                    // 1MHz 9600 ---> user manual
    UCA0BR1 = 0;

    UCA0MCTL = UCBRS0;                // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;             // Start USCI state machine

    UC0IE &= ~(UC0IFG);
    UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt

    P2DIR |= (BIT4 + BIT5 + BIT6 + BIT7);
    P2SEL &= ~(BIT6 + BIT7);
    P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);
}

void DelayEstable(void){
    WDTCTL = WDT_MDLY_32;
    IFG1 &= ~(WDTIFG);
    while (IFG1 & WDTIFG == 0);
    WDTCTL = WDTPW + WDTHOLD;
    IFG1 &= ~(WDTIFG);
}
