#include <msp430.h>

#define ESTADO_A 0
#define ESTADO_B 1
#define ESTADO_C 2
#define EV_S3 BIT0
#define EV_S6 BIT1

#define SET(ev) (eventos |= (ev))
#define RESET(ev) (eventos &= ~(ev))
#define TEST(ev) (eventos & (ev))

volatile unsigned char eventos, estados;
void configureMicro(void);
void StartWatchdog(void);

int main(void)
{
   configureMicro();
   estados = ESTADO_A;
   while(1)
   {
       __low_power_mode_0();
       while(eventos){
           switch (estados){
           case ESTADO_A:
               if (TEST(EV_S3)){
                   P2OUT |= (BIT4);
                   P2OUT |= (BIT6);
                   estados = ESTADO_B;
                   RESET(EV_S3);
               } else if (TEST(EV_S6)){
                   RESET(EV_S6);
               }

           break;
           case ESTADO_B:
               if (TEST(EV_S3)){
                   P2OUT &= ~(BIT4);
                   P2OUT &= ~(BIT6);
                   P2OUT |= (BIT5);
                   P2OUT |= (BIT7);
                   estados = ESTADO_C;
                   RESET(EV_S3);

               } else if (TEST(EV_S6)){
                   P2OUT &= ~(BIT4);
                   P2OUT &= ~(BIT5);
                   P2OUT &= ~(BIT6);
                   P2OUT &= ~(BIT7);
                   estados = ESTADO_A;
                   RESET(EV_S6);
               }
           break;
           case ESTADO_C:
               if (TEST(EV_S3)){
                  RESET(EV_S3);
               } else if (TEST(EV_S6)){
                   P2OUT &= ~(BIT5);
                   P2OUT &= ~(BIT7);
                   P2OUT |= (BIT4);
                   P2OUT |= (BIT6);
                   estados = ESTADO_B;
                   RESET(EV_S6);
               }
           break;
           }
       }
   }
}

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void){

    if(P1IFG & BIT4)
    {
        if(P1IES & BIT4){
            SET(EV_S3);
        }

        P1IES ^= BIT4;
        P1IFG &= ~(BIT4);
        P1IE &= ~(BIT4);
        StartWatchdog();
    }
    if(eventos){
        __low_power_mode_off_on_exit();
    }
}
#pragma vector = PORT2_VECTOR
__interrupt void RTI_P2(void){

    if(P2IFG & BIT3)
    {
        if(P2IES & BIT3){
            SET(EV_S6);
        }
        P2IES ^= BIT3;
        P2IFG &= ~BIT3;
        P2IE &= ~(BIT3);
        StartWatchdog();
    }
    if(eventos){
        __low_power_mode_off_on_exit();
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
    }

    WDTCTL = WDTPW | WDTHOLD;
    IE1 &=  ~(WDTIE);
}

void configureMicro(void){
    WDTCTL = WDTPW + WDTHOLD;

       //Configuración de Entradas digitales - 0
       P1DIR &= ~(BIT4); //S3 - 1.4
       P1REN |=  (BIT4);
       P1OUT |= (BIT4);
       P2DIR &= ~ (BIT3);
       P2REN |= (BIT3);
       P2OUT |= (BIT3);

       //Configuración de Salidas digitales - 1
       P2DIR |= (BIT4 + BIT5 + BIT6 + BIT7);
       P2SEL &= ~(BIT6 + BIT7);

       P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);

       P1IES |= (BIT4);
       P2IES |= (BIT3);

       P1IFG &= ~(BIT4);
       P2IFG &= ~(BIT3); // Clears Interrupt flags

       P1IE |= BIT4;
       P2IE |= (BIT3);
}

void StartWatchdog(void){
    WDTCTL = WDT_MDLY_32;
    IE1 &= ~(WDTIFG);
    IE1 |= WDTIE;
}
