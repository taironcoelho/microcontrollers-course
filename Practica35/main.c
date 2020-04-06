#include <msp430.h> 
#define CUENTAS 1250 //440Hz /2 (Duty cycle 50%)

/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    TA1CTL |= (TASSEL_2 | ID_0 | MC_2 );

    P1DIR &= ~(BIT4);
    P1REN |= BIT4;
    P1OUT |= BIT4;

    P2DIR &= ~(BIT0);
    P2DIR |= BIT0;
    P2OUT |= (BIT0);
    TA1CCR0 = TAR + CUENTAS;

    volatile unsigned int estado = 0;

   while(1){

       if(!(P1IN & BIT4)) {
            estado=1;
       }

       if((TA1CCTL0 & CCIFG) && estado){

           P2OUT ^= BIT0;
           TA1CCTL0 &= ~CCIFG;
           TA1CCR0 += CUENTAS;
       }

   }


}

