#include <msp430.h>

void configureMicro(void);
void StartWatchdog(void);

int main(void)
{
    configureMicro();
    __enable_interrupt();

    while(1){

    }
}

#pragma vector = WDT_VECTOR
__interrupt void RTI_WDT(void)
{
    if (!(P1IE & BIT4)){
           P1IFG &= ~(BIT4);
           P1IE |= (BIT4);
       }else if (!(P2IE & BIT1)){
           P2IFG &= ~(BIT5);
           P2IE |= (BIT1);
       }else if (!(P2IE & BIT2)){
           P2IFG &= ~(BIT6);
           P2IE |= (BIT2);
       }else if (!(P2IE & BIT3)){
           P2IFG &= ~(BIT7);
           P2IE |= (BIT3);
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
            P2OUT ^= (BIT4);
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
            P2OUT ^= (BIT5);
        }

        P2IES ^= BIT1;
        P2IFG &= ~BIT1;
        P1IE &= ~(BIT1);

        StartWatchdog();
    }
    if(P2IFG & BIT2)
    {
        if(P2IES & BIT2){
            P2OUT ^= (BIT6);
        }

        P2IES ^= BIT2;
        P2IFG &= ~BIT2;
        P1IE &= ~(BIT2);

        StartWatchdog();
    }

     if(P2IFG & BIT3)
    {
        if(P2IES & BIT3){
            P2OUT ^= (BIT7);
        }

        P2IES ^= BIT3;
        P2IFG &= ~BIT3;
        P2IE &= ~(BIT3);

        StartWatchdog();
    }
}

void configureMicro(void){
    WDTCTL = WDTPW | WDTHOLD;  // stop watchdog timer

    //Configuración de Entradas digitales - 0
    P1DIR &= ~(BIT4); //S3 - 1.4
    P1REN |=  (BIT4);
    P1OUT |= (BIT4);
    P2DIR &= ~ (BIT1 + BIT2 + BIT3); //S4 - 2.1
    P2REN |=  (BIT1 + BIT2 + BIT3);
    P2OUT |= (BIT1 + BIT2 + BIT3);

    //Configuración de Salidas digitales - 1
    P2DIR |= (BIT4 + BIT5 + BIT6 + BIT7);
    P2SEL &= ~(BIT6 + BIT7);

    P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);

    P1IFG &= ~(BIT4);
    P2IFG &= ~(BIT1 + BIT2 + BIT3); // Clears Interrupt flags

    P1IES |= BIT4;
    P2IES |= (BIT1 + BIT2 + BIT3);

    P1IE |= BIT4;
    P2IE |= (BIT1 + BIT2 + BIT3);
}

void StartWatchdog(void){
    WDTCTL = WDT_MDLY_32;
    IE1 &= ~(WDTIFG);
    IE1 |= WDTIE;
}
