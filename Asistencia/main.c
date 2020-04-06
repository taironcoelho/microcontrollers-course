#include "msp430g2533.h"

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;
    DCOCTL = 0;
    BCSCTL1 = CALBC1_1MHZ + DIVA_1;
    DCOCTL = CALDCO_1MHZ;
    P2DIR = ~BIT7;
    P2SEL = ~(BIT7 + BIT6);

    P2DIR = BIT5;
    P2SEL = (BIT3 + BIT4);

    while(1){

    }
}
