#include <msp430.h>

#define NCUENTAS05      62500 //0.5s
#define DO4             1911
#define RE4             1703
#define MI4             1517
#define FA4             1432
#define DO5             956
#define SI4             1012
#define LA4             1136
#define SOL4            1125

#define RXD             BIT1
#define TXD             BIT2

#define ESTADO_INIT     0
#define SIMPLE_BAJAS    1
#define SIMPLE_ALTAS    2
#define CONTINUO_BAJAS  3
#define CONTINUO_ALTAS  4
#define REMOTO          5

#define EV_S2           BIT0
#define EV_S2_ESTADO    BIT1
#define EV_S2_NOTAS     BIT2
#define EV_S3           BIT3
#define EV_S4           BIT4
#define EV_S5           BIT5
#define EV_S6           BIT6
#define EV_UART         BIT7

#define SET(ev) (eventos |= (ev))
#define RESET(ev) (eventos &= ~(ev))
#define TEST(ev) (eventos & (ev))

volatile unsigned char eventos, estados;
volatile unsigned int c;
volatile unsigned int cZumb;
volatile unsigned char command;
volatile char mensaje[5]; //Message to send to console
unsigned int i; //Counter

void configureMicro(void);
void StartWatchdog(void);
void PlaySong(int nota);
void StartZumb(void);

int main(void)
{
   configureMicro();
   __enable_interrupt();

   while(1)
   {
      __low_power_mode_0();
       while(eventos){
           switch (estados){
           case ESTADO_INIT:
               estados = SIMPLE_BAJAS;
               UC0IE &= ~(UC0IFG);
               UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
               strcpy(mensaje, "-SMP");
               i = 0;
           case SIMPLE_BAJAS:
               if(TEST(EV_S3)){
                   PlaySong(FA4);
                   RESET(EV_S3);
               } else if(TEST(EV_S4)){
                   PlaySong(MI4);
                   RESET(EV_S4);
               } else if(TEST(EV_S5)){
                   PlaySong(RE4);
                   RESET(EV_S5);
               } else if(TEST(EV_S6)){
                   PlaySong(DO4);
                   RESET(EV_S6);
               }
               if(TEST(EV_S2_ESTADO)){
                   estados = CONTINUO_BAJAS;
                   UC0IE &= ~(UC0IFG);
                   UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
                   strcpy(mensaje, "-CNT");
                   i = 0;
                   P1OUT |= (BIT6);
                   RESET(EV_S2_ESTADO);
               } else if(TEST(EV_S2_NOTAS)){
                   estados = SIMPLE_ALTAS;
                   RESET(EV_S2_NOTAS);
               } else if(TEST(EV_UART)){
                   estados = REMOTO;
                   UC0IE &= ~(UC0IFG);
                   UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
                   strcpy(mensaje, "-RMT");
                   i = 0;
                   TA0CCR1 = TA0R + NCUENTAS05;
                   TA0CCTL1 &= ~(CCIFG);
                   TA0CCTL1 |= (CCIE);
                   RESET(EV_UART);
               }

           break;
           case SIMPLE_ALTAS:
                if(TEST(EV_S3)){
                    PlaySong(DO5);
                    RESET(EV_S3);
                } else if(TEST(EV_S4)){
                    PlaySong(SI4);
                    RESET(EV_S4);
                } else if(TEST(EV_S5)){
                    PlaySong(LA4);
                    RESET(EV_S5);
                } else if(TEST(EV_S6)){
                    PlaySong(SOL4);
                    RESET(EV_S6);
                }

                if(TEST(EV_S2_ESTADO)){
                      estados = CONTINUO_BAJAS;
                      UC0IE &= ~(UC0IFG);
                      UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
                      strcpy(mensaje, "-CNT");
                      i = 0;
                      P1OUT |= (BIT6);
                      RESET(EV_S2_ESTADO);
                } else if(TEST(EV_S2_NOTAS)){
                  estados = SIMPLE_BAJAS;
                  RESET(EV_S2_NOTAS);
                } else if(TEST(EV_UART)){
                    estados = REMOTO;
                    UC0IE &= ~(UC0IFG);
                    UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
                    strcpy(mensaje, "-RMT");
                    i = 0;
                    TA0CCR1 = TA0R + NCUENTAS05;
                    TA0CCTL1 &= ~(CCIFG);
                    TA0CCTL1 |= (CCIE);
                    RESET(EV_UART);
                }
           break;
           case CONTINUO_BAJAS:
               if(TEST(EV_S3)){
                     PlaySong(FA4);
                     RESET(EV_S3);
                 } else if(TEST(EV_S4)){
                     PlaySong(MI4);
                     RESET(EV_S4);
                 } else if(TEST(EV_S5)){
                     PlaySong(RE4);
                     RESET(EV_S5);
                 } else if(TEST(EV_S6)){
                     PlaySong(DO4);
                     RESET(EV_S6);
                 }
               if(TEST(EV_S2_ESTADO)){
                     estados = SIMPLE_BAJAS;
                     UC0IE &= ~(UC0IFG);
                     UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
                     strcpy(mensaje, "-SMP");
                     i = 0;
                     P1OUT &= ~(BIT6);
                     RESET(EV_S2_ESTADO);
               } else if(TEST(EV_S2_NOTAS)){
                 estados = CONTINUO_ALTAS;
                 RESET(EV_S2_NOTAS);
               } else if(TEST(EV_UART)){
                   RESET(EV_UART);
               }
           break;
           case CONTINUO_ALTAS:
               if(TEST(EV_S3)){
                   PlaySong(DO5);
                   RESET(EV_S3);
               } else if(TEST(EV_S4)){
                   PlaySong(SI4);
                   RESET(EV_S4);
               } else if(TEST(EV_S5)){
                   PlaySong(LA4);
                   RESET(EV_S5);
               } else if(TEST(EV_S6)){
                   PlaySong(SOL4);
                   RESET(EV_S6);
               }

               if(TEST(EV_S2_ESTADO)){
                     estados = SIMPLE_BAJAS;
                     UC0IE &= ~(UC0IFG);
                     UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
                     strcpy(mensaje, "-SMP");
                     i = 0;
                     P1OUT &= ~(BIT6);
                     RESET(EV_S2_ESTADO);
               } else if(TEST(EV_S2_NOTAS)){
                   estados = CONTINUO_BAJAS;
                   RESET(EV_S2_NOTAS);
               } else if(TEST(EV_UART)){
                   RESET(EV_UART);
               }
           break;
           case REMOTO:
               if(TEST(EV_UART)){
                   estados = SIMPLE_BAJAS;
                   UC0IE &= ~(UC0IFG);
                   UC0IE |= UCA0TXIE; // Enable USCI_A0 TX interrupt
                   strcpy(mensaje, "-SMP");
                   i = 0;
                   P1OUT &= ~(BIT6);
                   TA0CCTL1 &= ~(CCIE);

                   RESET(EV_UART);
               } else if(TEST(EV_S2_NOTAS)){
                   RESET(TEST(EV_S2_NOTAS));
               } else if(TEST(EV_S2_ESTADO)){
                   RESET(EV_S2_ESTADO);
               }

                if(TEST(EV_S3)){;
                    RESET(EV_S3);
                } else if(TEST(EV_S4)){
                    RESET(EV_S4);
                } else if(TEST(EV_S5)){
                    RESET(EV_S5);
                } else if(TEST(EV_S6)){
                    RESET(EV_S6);
                }
            break;
           }
       }
   }
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR (void)
{
    if(c >= 3){
        RESET(EV_S2);
        SET(EV_S2_ESTADO);
        TA0CCTL0 &= ~CCIE;
    }else {
        TA0CCR0 += NCUENTAS05;
        c++;
    }
}

#pragma vector = TIMER0_A1_VECTOR
__interrupt void Timer0_A1_ISR(void)
{
    switch(TAIV){
        case TA0IV_TACCR1:
                P1OUT ^= (BIT6);
                TA0CCR1 += NCUENTAS05;
            break;
        case TA0IV_TACCR2:
            if(cZumb>= 1){
                TA1CTL &= ~(MC0| MC1);
                TA0CCTL2 &= ~(CCIE);
                P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);
                P1OUT &= ~BIT0;
            } else {
                cZumb++;
                TA0CCR2 +=NCUENTAS05;
            }

            break;

        case TA0IV_TAIFG:
            break;
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void RTI_P1(void){

    if(P1IFG & BIT3)
    {
        if(P1IES & BIT3){
            c = 0;
            SET(EV_S2);
            TA0CCR0 = TA0R + NCUENTAS05;
            TA0CCTL0 &= ~CCIFG;
            TA0CCTL0 |= CCIE;

        } else {
            if(c<3){
                TA0CCTL0 &= ~CCIE;
                RESET(EV_S2);
                SET(EV_S2_NOTAS);
            }
        }

        P1IES ^= BIT3;
        P1IFG &= ~(BIT3);
        P1IE &= ~(BIT3);
        StartWatchdog();
    }
    if(P1IFG & BIT4)
    {
        if(P1IES & BIT4){
            SET(EV_S3);
        } else {
            if((estados == CONTINUO_BAJAS) || (estados == CONTINUO_ALTAS)){
                TA1CTL &= ~(MC0| MC1);
                P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);
                P1OUT &= ~BIT0;
            }
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

    if(P2IFG & BIT1)
    {
        if(P2IES & BIT1){
            SET(EV_S4);
        } else {
            if((estados == CONTINUO_BAJAS) || (estados == CONTINUO_ALTAS)){
                TA1CTL &= ~(MC0| MC1);
                P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);
                P1OUT &= ~BIT0;
            }
        }

        P2IES ^= BIT1;
        P2IFG &= ~BIT1;
        P2IE &= ~(BIT1);
        StartWatchdog();
    }

    if(P2IFG & BIT2)
    {
        if(P2IES & BIT2){
            SET(EV_S5);
        } else {
            if((estados == CONTINUO_BAJAS) || (estados == CONTINUO_ALTAS)){
                TA1CTL &= ~(MC0| MC1);
                P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);
                P1OUT &= ~BIT0;
            }
        }

        P2IES ^= BIT2;
        P2IFG &= ~BIT2;
        P2IE &= ~(BIT2);
        StartWatchdog();
    }

    if(P2IFG & BIT3)
    {
        if(P2IES & BIT3){
            SET(EV_S6);
        } else {
            if((estados == CONTINUO_BAJAS) || (estados == CONTINUO_ALTAS)){
                TA1CTL &= ~(MC0| MC1);
                P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);
                P1OUT &= ~BIT0;
            }
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

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCI0TX_ISR(void)
{
    UCA0TXBUF = mensaje[i++]; // TX next character
    if (i == sizeof mensaje - 1){ // TX over?
        UC0IE &= ~UCA0TXIE; // Disable USCI_A0 TX interrupt
    }
}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    int validChar = 0;
    if (UCA0RXBUF == '0') // '0' received?
    {
        SET(EV_UART);
    }

    if(estados == REMOTO){
        if(UCA0RXBUF == '1'){
                validChar = 1;
                PlaySong(DO4);
            } else if(UCA0RXBUF == '2'){
                validChar = 1;
                PlaySong(RE4);
            } else if(UCA0RXBUF == '3'){
                validChar = 1;
                PlaySong(MI4);
            } else if(UCA0RXBUF == '4'){
                validChar = 1;
                PlaySong(FA4);
            } else if(UCA0RXBUF == '5'){
                validChar = 1;
                PlaySong(SOL4);
            } else if(UCA0RXBUF == '6'){
                validChar = 1;
                PlaySong(LA4);
            } else if(UCA0RXBUF == '7'){
                validChar = 1;
                PlaySong(SI4);
            } else if(UCA0RXBUF == '8'){
                validChar = 1;
                PlaySong(DO5);
            }
    }

    if(eventos || validChar){
        __low_power_mode_off_on_exit();
    }
}

#pragma vector = WDT_VECTOR
__interrupt void RTI_WDT(void)
{
    if(!(P1IE & BIT3)){
        P1IFG &= ~(BIT3);
        P1IE |= (BIT3);
    } else if (!(P1IE & BIT4)){
        P1IFG &= ~(BIT4);
        P1IE |= (BIT4);
    }else if (!(P2IE & BIT1)){
        P2IFG &= ~(BIT1);
        P2IE |= (BIT1);
    }else if (!(P2IE & BIT2)){
       P2IFG &= ~(BIT2);
       P2IE |= (BIT2);
    }else if (!(P2IE & BIT3)){
       P2IFG &= ~(BIT3);
       P2IE |= (BIT3);
    }
    WDTCTL = WDTPW | WDTHOLD;
    IE1 &=  ~(WDTIE);
}

void configureMicro(void){
    WDTCTL = WDTPW + WDTHOLD;

    BCSCTL1 = CALBC1_1MHZ;            // Set DCO to 1 MHz
    DCOCTL = CALDCO_1MHZ;

    P1SEL = RXD + TXD ;                // Select TX and RX functionality for P1.1 & P1.2
    P1SEL2 = RXD + TXD ;

    UCA0CTL1 |= UCSWRST;
    UCA0CTL1 |= UCSSEL_2;             // Use SMCLK: 1MHz

    UCA0BR0 = 104;                    // 1MHz 9600 ---> user manual
    UCA0BR1 = 0;

    UCA0MCTL = UCBRS0;                // Modulation UCBRSx = 1
    UCA0CTL1 &= ~UCSWRST;             // Start USCI state machine

    UC0IE &= ~(UC0IFG);
    UC0IE |= UCA0RXIE; // Enable USCI_A0 RX interrupt

    //Configuración de Entradas digitales - 0
    P1DIR &= ~(BIT3 + BIT4); //
    P1REN |=  (BIT3 + BIT4);
    P1OUT |= (BIT3 + BIT4);
    P2DIR &= ~ (BIT1+ BIT2 + BIT3);
    P2REN |= (BIT1 + BIT2 +BIT3);
    P2OUT |= (BIT1 + BIT2 + BIT3);

    //Configuración de Salidas digitales - 1
    P1DIR |= (BIT0 + BIT6);
    P2DIR |= (BIT0 + BIT4 + BIT5 + BIT6 + BIT7);
    P2DIR &=~ (BIT1 + BIT2 + BIT3);
    P2SEL &= ~(BIT6 + BIT7);
    P2SEL |= BIT0;

    P1OUT &= ~(BIT0 + BIT6);
    P2OUT &= ~(BIT4 + BIT5 + BIT6 + BIT7);

    P1IES |= (BIT3 + BIT4);
    P2IES |= (BIT1 + BIT2 + BIT3);

    P1IFG &= ~(BIT3 + BIT4);
    P2IFG &= ~(BIT1 + BIT2 + BIT3); // Clears Interrupt flags

    P1IE |= (BIT3 + BIT4);
    P2IE |= (BIT1 + BIT2 + BIT3);

    TA1CTL |= TASSEL_2 | ID_0;
    TA1CTL |= TACLR;

    TA0CTL |= TASSEL_2 | ID_3 | MC_2;
    TA0CTL |= TACLR;
}

void StartWatchdog(void){
    WDTCTL = WDT_MDLY_32;
    IE1 &= ~(WDTIFG);
    IE1 |= WDTIE;
}

void PlaySong(int nota){
    switch(nota){
    case FA4:
        P1OUT &= ~BIT0;
        P2OUT |= (BIT4);
        TA1CCR0 = FA4;
        //TA1CCR1 = FA4/2;
        TA1CCTL0 |= OUTMOD_4;
        TA1CTL |= (MC0 | TACLR);

        if((estados != CONTINUO_BAJAS) && (estados != CONTINUO_ALTAS)){
            StartZumb();
        }
        break;
    case MI4:
        P1OUT &= ~BIT0;
        P2OUT |= (BIT5);
        TA1CCR0 = MI4;
        TA1CCTL0 |= OUTMOD_4;
        TA1CTL |= (MC0 | TACLR);

        if((estados != CONTINUO_BAJAS) && (estados != CONTINUO_ALTAS)){
            StartZumb();
        }
        break;
    case RE4:
        P1OUT &= ~BIT0;
        P2OUT |= (BIT6);
        TA1CCR0 = RE4;
        TA1CCTL0 |= OUTMOD_4;
        TA1CTL |= (MC0 | TACLR);

        if((estados != CONTINUO_BAJAS) && (estados != CONTINUO_ALTAS)){
            StartZumb();
        }
        break;
    case DO4:
        P1OUT &= ~BIT0;
        P2OUT |= (BIT7);
        TA1CCR0 = DO4;
        TA1CCTL0 |= OUTMOD_4;
        TA1CTL |= (MC0 | TACLR);

        if((estados != CONTINUO_BAJAS) && (estados != CONTINUO_ALTAS)){
            StartZumb();
        }
        break;
    case DO5:
        if(estados != REMOTO){
            P1OUT |= BIT0;
        }

        P2OUT |= (BIT4);
        TA1CCR0 = DO5;
        TA1CCTL0 |= OUTMOD_4;
        TA1CTL |= (MC0 | TACLR);

        if((estados != CONTINUO_BAJAS) && (estados != CONTINUO_ALTAS)){
            StartZumb();
        }
        break;
    case SI4:
        if(estados != REMOTO){
            P1OUT |= BIT0;
        }
        P2OUT |= (BIT5);
        TA1CCR0 = SI4;
        TA1CCTL0 |= OUTMOD_4;
        TA1CTL |= (MC0 | TACLR);

        if((estados != CONTINUO_BAJAS) && (estados != CONTINUO_ALTAS)){
            StartZumb();
        }
        break;
    case LA4:
        if(estados != REMOTO){
            P1OUT |= BIT0;
        }
        P2OUT |= (BIT6);
        TA1CCR0 = LA4;
        TA1CCTL0 |= OUTMOD_4;
        TA1CTL |= (MC0 | TACLR);

        if((estados != CONTINUO_BAJAS) && (estados != CONTINUO_ALTAS)){
            StartZumb();
        }
        break;
    case SOL4:
        if(estados != REMOTO){
            P1OUT |= BIT0;
        }
        P2OUT |= (BIT7);
        TA1CCR0 = SOL4;
        TA1CCTL0 |= OUTMOD_4;
        TA1CTL |= (MC0 | TACLR);

        if((estados != CONTINUO_BAJAS) && (estados != CONTINUO_ALTAS)){
            StartZumb();
        }
        break;
    }
}

void StartZumb(void){
    cZumb=0;
    TA0CCR2 = TA0R + NCUENTAS05;
    TA0CCTL0 &= ~CCIFG;
    TA0CCTL2 |= CCIE;
}
