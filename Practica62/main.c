#include <msp430.h>

#define TXD BIT2

volatile char mensaje[4]; //Message to send to console
unsigned int i; //Counter

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

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    UCA0TXBUF = mensaje[i++]; // TX next character
    if (i == sizeof mensaje - 1){ // TX over?
        UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
    }
}

#pragma vector = WDT_VECTOR
__interrupt void RTI_WDT(void)
{
    if (!(P1IE & BIT4)){
        P1IFG &= ~(BIT4);
        P1IE |= (BIT4);
    }else if (!(P2IE & BIT3)){
        P2IFG &= ~(BIT3);
        P2IE |= (BIT3);
    } else if(!(P2IE & BIT2)){
        P2IFG &= ~(BIT2);
        P2IE |= (BIT2);
    } else if(!(P2IE & BIT1)){
        P2IFG &= ~(BIT1);
        P2IE |= (BIT1);
    }
    WDTCTL = WDTPW | WDTHOLD;
    IE1 &=  ~(WDTIE);

}

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void)
{
    if(P1IFG & BIT4)
    {
        if(P1IES & BIT4){
            __low_power_mode_off_on_exit();
            strcpy(mensaje, "3\r\n");
            i = 0;
            UC0IE &= ~(UC0IFG);
            UC0IE |= UCA0TXIE;
        }

        P1IES ^= BIT4;
        P1IFG &= ~(BIT4);
        P1IE &= ~(BIT4);
        StartWatchdog();
    }
}

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void)
{

    if(P2IFG & BIT1)
    {
        if(P2IES & BIT1){
            __low_power_mode_off_on_exit();
            strcpy(mensaje, "4\r\n");
            i = 0;
            UC0IE &= ~(UC0IFG);
            UC0IE |= UCA0TXIE;
        }
        P2IES ^= BIT1;
        P2IFG &= ~BIT1;
        P2IE &= ~(BIT1);
        StartWatchdog();
    }
    if(P2IFG & BIT2)
    {
        if(P2IES & BIT2){
            __low_power_mode_off_on_exit();
            strcpy(mensaje, "5\r\n");
            i = 0;
            UC0IE &= ~(UC0IFG);
            UC0IE |= UCA0TXIE;
        }

        P2IES ^= BIT2;
        P2IFG &= ~BIT2;
        P2IE &= ~(BIT2);
        StartWatchdog();
    }

     if(P2IFG & BIT3)
    {
         if(P2IES & BIT3){
             __low_power_mode_off_on_exit();
             strcpy(mensaje, "6\r\n");
             i = 0;
             UC0IE &= ~(UC0IFG);
             UC0IE |= UCA0TXIE;
         }
        P2IES ^= BIT3;
        P2IFG &= ~BIT3;
        P2IE &= ~(BIT3);
        StartWatchdog();
    }
}

void ConfigureMicro(void){
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer

    BCSCTL1 = CALBC1_1MHZ;            // Set DCO to 1 MHz
    DCOCTL = CALDCO_1MHZ;

    P1SEL = TXD ;                // Select TX functionality for P1.1 & P1.2
    P1SEL2 = TXD ;

    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;             // SMLCK 1MHz

    UCA0BR0 = 104;                    // 1MHz 9600 ---> Tabla
    UCA0BR1 = 0;                          //  ---> Tabla

    UCA0MCTL = UCBRS0;                // Modulation UCBRSx = 1 --> Tabla
    UCA0CTL1 &= ~UCSWRST;             // Start USCI state machine

    P1DIR &= ~(BIT4); //S3 - 1.4
    P1REN |=  (BIT4);
    P1OUT |= (BIT4);
    P2DIR &= ~ (BIT1 + BIT2 + BIT3); //S4 - 2.1
    P2REN |=  (BIT1 + BIT2 + BIT3);
    P2OUT |= (BIT1 + BIT2 + BIT3);

    P1IES |= (BIT4);
    P2IES |= (BIT1 + BIT2 + BIT3);

    P1IFG &= ~(BIT4);
    P2IFG &= ~(BIT1 + BIT2 + BIT3); // Clears Interrupt flags

    P1IE |= BIT4;
    P2IE |= (BIT1 + BIT2 + BIT3);
}

void StartWatchdog(void){
    WDTCTL = WDT_MDLY_32;
    IE1 &= ~(WDTIFG);
    IE1 |= WDTIE;
}

void DelayEstable(void){
    WDTCTL = WDT_MDLY_32;
    IFG1 &= ~(WDTIFG);
    while (IFG1 & WDTIFG == 0);
    WDTCTL = WDTPW + WDTHOLD;
    IFG1 &= ~(WDTIFG);
}
