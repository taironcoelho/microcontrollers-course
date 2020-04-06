#include <msp430.h>
#define NCUENTASLED3 0xF424;
#define NCUENTASLED4 0xD6D8;
#define NCUENTASLED5 0xC350;
#define NCUENTASLED6 0x8647;

volatile unsigned int id;

volatile unsigned int checkLed3 = 0;
volatile unsigned int checkLed4 = 0;
volatile unsigned int checkLed5 = 0;
volatile unsigned int checkLed6 = 0;

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

    TA0CTL |= TASSEL_2 | ID_3 | MC_2;
    TA0CTL |= TACLR;

    TA1CTL |= TASSEL_2 | ID_3 | MC_2;
    TA1CTL |= TACLR;

    P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);

    P1IES |= BIT4;
    P2IES |= (BIT1 + BIT2 + BIT3);

    P1IFG &= ~(BIT4);
    P2IFG &= ~(BIT1 + BIT2 + BIT3); // Clears Interrupt flags

    P1IE |= BIT4;
    P2IE |= (BIT1 + BIT2 + BIT3);

    TA1CCTL0 &= ~(CCIFG);
    TA1CCTL1 &= ~(CCIFG);
    TA0CCTL1 &= ~(CCIFG);
    TA0CCTL0 &= ~(CCIFG);

    TA0CCTL1 |= CCIE;
    TA0CCTL0 |= CCIE;
    TA1CCTL0 |= CCIE;
    TA1CCTL1 |= CCIE;
    __enable_interrupt();

    for (;;){

    }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A0_ISR(void)
{
    if(checkLed3){
        P2OUT ^= (BIT4);
        TA0CCR0 += NCUENTASLED3;
    }
}

#pragma vector = TIMER1_A0_VECTOR
__interrupt void Timer1_A0_ISR(void)
{
    if(checkLed5){
        P2OUT ^= (BIT6);
        TA1CCR0 += NCUENTASLED5;
    }
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void)
{
    switch(TAIV){
        case TA0IV_TACCR1:
            if(checkLed4){
                P2OUT ^= (BIT5);
                TA0CCR1 += NCUENTASLED3;
            }
            break;
        case TA0IV_TACCR2:
            break;

        case TA0IV_TAIFG:
            break;
    }
}
#pragma vector = TIMER1_A1_VECTOR
__interrupt void Timer1_A1_ISR(void)
{
    switch (TA1IV){
        case TA1IV_TACCR1:
            if(checkLed6){
                P2OUT ^= (BIT7);
                TA1CCR1 += NCUENTASLED6;
            }
            break;
        case TA1IV_TACCR2:
            break;

        case TA1IV_TAIFG:
            break;
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void)
{
    if(P1IFG & BIT4)
    {
        P2OUT &= ~(BIT4);
        P1IFG &= ~(BIT4);
        TA0CCR0 = TA0R + NCUENTASLED3;
        checkLed3 = !checkLed3;
    }
}

#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void)
{
    if(P2IFG & BIT1)
    {
        P2OUT &= ~(BIT5);
        P2IFG &= ~BIT1;
        TA0CCR1 = TA0R + NCUENTASLED4;
        checkLed4 = !checkLed4;
    }
    if(P2IFG & BIT2)
    {
        P2OUT &= ~(BIT6);
        P2IFG &= ~BIT2;
        TA1CCR0 = TA1R + NCUENTASLED5;
        checkLed5 = !checkLed5;
    }

     if(P2IFG & BIT3)
    {
        P2OUT &= ~(BIT7);
        P2IFG &= ~BIT3;
        TA1CCR1 = TA1R + NCUENTASLED6;
        checkLed6 = !checkLed6;
    }
}

