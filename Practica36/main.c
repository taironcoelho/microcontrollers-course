#include <msp430.h> 
#define N 0x5E88

/**
 * main.c
 */
void ConfigureMicro(void);

int main(void)
{
    ConfigureMicro();

    while (1){
        if(!(P2IN & BIT1)){
            TA1CCR2 = TA1CCR0 * 0.1;
        }
        if(!(P2IN & BIT2)){
            TA1CCR2 = TA1CCR0 * 0.5;
        }
        if(!(P2IN & BIT3)){
            TA1CCR2 = TA1CCR0 * 0.9;
        }
        if(!(P1IN & BIT4)){
            TA1CCR2 = 0;
        }
    }
}

void ConfigureMicro(void){
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    TA1CTL |= TASSEL_2 | ID_0 | MC_1;
    P2DIR |= BIT5 | BIT6;
    P2SEL |= BIT5;
    P2SEL &= ~BIT6;
    P2REN |= BIT1 | BIT2 | BIT3;
    P2OUT |= BIT1 | BIT2 | BIT3;
    P2OUT |= BIT6;

    P1REN |= BIT4;
    P1OUT |= BIT4;

    TA1CCR0 = N;
    TA1CCTL2 |= OUTMOD_7;
}
