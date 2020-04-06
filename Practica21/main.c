#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    //Configuración de Entradas digitales - 0
    P1DIR &= ~(BIT4 ); //S3 - 1.4
    P1REN |=  (BIT4);
    P1OUT |= (BIT4);
    P2DIR &= ~ (BIT1 ); //S4 - 2.1
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
    P2SEL &= ~(BIT6 + BIT1 + BIT2);
    P2SEL &= ~(BIT7+ BIT1 + BIT2);




    for (;;)
    {


        if((P1IN & BIT4 ) != 0){ //led 1
            P2OUT &= ~(BIT4);
        }
        else{
            P2OUT |= (BIT4);
        }

        if((P2IN & BIT1 ) != 0){//led 2
            P2OUT &= ~(BIT5);
        }
        else{

            P2OUT |= (BIT5);

        }

        if((P2IN & BIT2 ) != 0){
            P2OUT &= ~(BIT6);

        }
        else{

            P2OUT |= (BIT6);

        }

        if((P2IN & BIT3 ) != 0){
            P2OUT &= ~(BIT7);

        }
        else{

            P2OUT |= (BIT7);

        }

    }

}
