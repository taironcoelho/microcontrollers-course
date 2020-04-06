#include <msp430.h>

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
    P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);

    P1IES |= BIT4;
    P2IES |= (BIT1 + BIT2 + BIT3);

    P1IFG &= ~(BIT4);
    P2IFG &= ~(BIT1 + BIT2 + BIT3); // Clears Interrupt flags
    P1IE |= BIT4;
    P2IE |= (BIT1 + BIT2 + BIT3);

    TA0CTL |= TASSEL_2 | ID_3;

    TA0CCR0 = 0x8646; // 137500 Hz x 250ms
    TA0CTL |= TACLR;

    TACCTL0 |= CCIE;
    TACCTL0 &= ~(CCIFG);
    __enable_interrupt();

    for (;;){

    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
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
        TA0CTL &= ~(MC0| MC1);
        cont = 0;
    } else {
        cont++;
    }

}

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void)
{
    if(P1IFG & BIT4) // check for p1IFG.3 is set or not
    {
        P2OUT |= (BIT4);
        TA0CCTL0 &=~ CCIFG;
        TA0CTL |= (MC0 | TACLR);
        P1IFG &= ~(BIT4); // clears P1.3 interrupt flag
        id = 1;
    }
}

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void)
{
    if(P2IFG & BIT1) // check for p1IFG.3 is set or not
    {
        P2OUT |=(BIT5);
        TA0CCTL0 &=~ CCIFG;
        TA0CTL |= (MC0 | TACLR);
        P2IFG &= ~BIT1; // clears P1.3 interrupt flag
        id = 2;
    }

     if(P2IFG & BIT2) // check for p1IFG.3 is set or not
    {
        P2OUT |=(BIT6);
        TA0CCTL0 &=~ CCIFG;
        TA0CTL |= (MC0 | TACLR);
        P2IFG &= ~BIT2; // clears P1.3 interrupt flag
        id = 3;
    }

     if(P2IFG & BIT3) // check for p1IFG.3 is set or not
    {
        P2OUT |=(BIT7);
        TA0CCTL0 &=~ CCIFG;
        TA0CTL |= (MC0 | TACLR);
        P2IFG &= ~BIT3; // clears P1.3 interrupt flag
        id = 4;
    }
}

