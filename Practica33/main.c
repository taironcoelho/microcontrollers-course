#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //Configuración de Entradas digitales - 0
    P1DIR &= ~(BIT4 + BIT5); //S3 - 1.4
    P1REN |=  (BIT4 + BIT5);
    P1OUT |= (BIT4 + BIT5);

    P2DIR &= ~ (BIT0); //S4 - 2.1
       P2REN |=  (BIT0);
       P2OUT |= (BIT0);

    P2DIR &= ~ (BIT1); //S4 - 2.1
    P2REN |=  (BIT1);
    P2OUT |= (BIT1);
    P2DIR &= ~ (BIT2); //S5 - 2.2
    P2REN |=  (BIT2);
    P2OUT |= (BIT2);
    P2DIR &= ~ (BIT3); //S6 - 2.3
    P2REN |=  (BIT3);
    P2OUT |= (BIT3);

    //Configuración de Salidas digitales - 1
    P2DIR |= BIT4; //Led 3 - 2.4
    P2DIR |= BIT5; //Led 4 - 2.5
    P2DIR |= BIT6; //Led 5 - 2.6
    P2DIR |= BIT7; //Led 6 -  2.7
    P2SEL &= ~(BIT6);
    P2SEL &= ~(BIT7);

    TACTL |= TASSEL_2 | ID_3 | MC_1 | TAIE;
    TACCR0 = 0x8646; //250ms
    TACTL |= TACLR;

    volatile unsigned int cont = 0;

    volatile unsigned int estadoant1 = 0;
    volatile unsigned int estadoant2 = 0;
    volatile unsigned int estadoant3 = 0;
    volatile unsigned int estadoant4 = 0;


    for (;;)
    {

        if(P1IN & BIT5){
            P2OUT |=BIT4;
        }
    }

}
