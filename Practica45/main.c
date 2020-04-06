#include <msp430.h>
#define NCUENTAS 0x8646;

/**
 * main.c
 */

volatile unsigned int id;
volatile unsigned int cont = 0;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

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

    P1IES |= BIT4;
    P2IES |= (BIT1 + BIT2 + BIT3);

    TA0CTL |= TASSEL_2 | ID_3 | MC_2;

    TACCTL1 &= ~(CCIFG);
    TACCTL1 |= CCIE;
    TA0CTL |= TACLR;

    P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);

    P1IFG &= ~(BIT4);
    P2IFG &= ~(BIT1 + BIT2 + BIT3); // Clears Interrupt flags

    P1IE |= BIT4;
    P2IE |= (BIT1 + BIT2 + BIT3);
    __enable_interrupt();

    for (;;){

    }
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void)
{
    switch(TAIV){
        case TA0IV_TACCR1:
            if(cont == 3){
                switch (id){
                       case 1:
                           P2OUT &= ~(BIT4);
                           break;
                       case 2:
                           P2OUT &= ~(BIT5);
                           break;
                       case 3:
                           P2OUT &= ~(BIT6);
                           break;
                       case 4:
                           P2OUT &= ~(BIT7);
                           break;
                   }
                cont = 0;
            } else {
                cont++;
            }
            break;
        case TA0IV_TACCR2:
            break;

        case TA0IV_TAIFG:
            break;
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void)
{
    if(P1IFG & BIT4)
    {
        P2OUT |= (BIT4);
        P1IFG &= ~(BIT4);
        id = 1;
        TA0CCR1 = TA0R + NCUENTAS;
    }
}

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void)
{
    if(P2IFG & BIT1)
    {
        P2OUT |=(BIT5);
        P2IFG &= ~BIT1;
        id = 2;
        TA0CCR1 = TA0R + NCUENTAS;
    }

     if(P2IFG & BIT2)
    {
        P2OUT |=(BIT6);
        P2IFG &= ~BIT2;
        id = 3;
        TA0CCR1 = TA0R + NCUENTAS;
    }

     if(P2IFG & BIT3)
    {
        P2OUT |=(BIT7);
        P2IFG &= ~BIT3;
        id = 4;
        TA0CCR1 = TA0R + NCUENTAS;
    }
}

