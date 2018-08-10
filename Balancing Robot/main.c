#include <msp430.h> 
#include <msp430f5529.h>

void PWMinit(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  PWMinit();
  __bis_SR_register(GIE);                   // enable interrupts

  while (1)
  {

  }
}

void PWMinit(void){

     P1DIR |= BIT6;                             // P1.6 interrupt debug

     P1DIR |= BIT2+BIT3;                        // P1.2 and P1.3 in output direction
     P1SEL |= BIT2+BIT3;                        // P1.2 to TA0.1 and P1.3 to TA1.3

     /*** Timer0_A Set-Up ***/
     TA0CCR0 = 512-1;                          // Set out PWM period of 512
     TA0CCTL1 = OUTMOD_7;                      // CCR3 reset/set
     TA0CCR1 = 150;                            // The period in microseconds that the power will be on.
     TA0CTL |= TASSEL_2 + MC_1 + TACLR;        // SMCLK, Up Mode (Counts to TA0CCR0)

     /*** Timer1_A with variable duty cycle Set-Up ***/
     TA0CCR1 = 512-1;                           // PWM frequency = 1MHz/(TA0CCR0+1) = 500 kHz
     TA0CCTL3 = OUTMOD_7;                       // CCR3 reset/set
     TA0CCR3 =  200;                            // The period in microseconds that the power will be on.
     TA0CCTL1 = CCIE;                           // CCR0 interrupt enabled
     TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;   // SMCLK, up mode, clear TAR

 }

/*** Timer1_A Interrupt Set-Up ***/
// Timer0 A0 interrupt service routine
#pragma vector=TIMERB0_VECTOR
__interrupt void TIMERA0_ISR (void){
static unsigned int a = 0;
P1OUT ^= BIT0;
a++;
}
  //P1OUT ^= BIT6;

