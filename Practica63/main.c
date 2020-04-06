#include <msp430.h>

#define RXD        BIT1
#define TXD        BIT2

volatile char mensaje[4]; //Message to send to console
volatile unsigned int i; //Counter

void ConfigureMicro(void);
void StartWatchdog(void);
void DelayEstable(void);

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
     mensaje[i] = UCA0RXBUF;
     if (i == 3) // Does it has 4 characters?
     {
         __low_power_mode_off_on_exit();
         UC0IE &= ~(UC0IFG);
         UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
     } else {
         i++;
     }
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    UCA0TXBUF = mensaje[i]; // TX next character
    if (i == 0){ // TX over?
        UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
    } else {
        i--;
    }
}

void ConfigureMicro(void){
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer

    BCSCTL1 = CALBC1_1MHZ;            // Set DCO to 1 MHz
    DCOCTL = CALDCO_1MHZ;

    P1SEL = RXD + TXD ;                // Select TX and RX functionality for P1.1 & P1.2
    P1SEL2 = RXD + TXD ;

    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;             // SMLCK 1MHz

    UCA0BR0 = 104;                    // 1MHz 9600 ---> Tabla
    UCA0BR1 = 0;                          //  ---> Tabla

    UCA0MCTL = UCBRS0;                // Modulation UCBRSx = 1 --> Tabla
    UCA0CTL1 &= ~UCSWRST;             // Start USCI state machine

    UC0IE &= ~(UC0IFG);
    UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt
}

void DelayEstable(void){
    WDTCTL = WDT_MDLY_32;
    IFG1 &= ~(WDTIFG);
    while (IFG1 & WDTIFG == 0);
    WDTCTL = WDTPW + WDTHOLD;
    IFG1 &= ~(WDTIFG);
}
