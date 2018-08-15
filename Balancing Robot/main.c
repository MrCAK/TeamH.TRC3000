#include <msp430.h> 
#include <msp430f5529.h>

int DutyCycle = 10;
int DutyCycle1 = 10;
int Period = 52 - 1; //Period set to

int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P1DIR |= BIT2+BIT3;                        // P1.2 and P1.3 in output direction
  P1SEL |= BIT2+BIT3;                        // P1.2 to TA0.1 and P1.3 to TA1.3
  TA0CCR0 = Period;                             // Set out PWM period of 399

  /* Initializing the buttons for interrupt*/
  P1REN |= BIT1;            //P1.1 Resistor enabled
  P1OUT |= BIT1;            //P1.1 pull-up resistor
  P2REN |= BIT1;            //P2.1 Resistor enabled
  P2OUT |= BIT1;            //P2.1 pull-up resistor

  P1IE |= 0x02;          //enable P1.1 interrupt
  P1IES |= 0x02;         //select high to low edge
  P1IFG &= ~0x02;        //clear P1.1 flag
  P2IE |= 0x02;          //enable P2.1 interrupt
  P2IES |= 0x02;         //select high to low edge
  P1IFG &= ~0x02;        //clear P2.1 flag


  __enable_interrupt();
  __bis_SR_register(LPM0_bits);                    // enable interrupts
  __no_operation();

  while (1)
  {

  }
}

//Interrupt for port 1
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
       if((P1IN & BIT1) != BIT1)
       {
              if(DutyCycle<50)
              {
                     int i;
                     DutyCycle = DutyCycle + 5;
                     for(i=0; i<30000; i++);
                     __low_power_mode_3();
              }
       }
       TA0CCTL1 = OUTMOD_7;
         TA0CCR1 = DutyCycle;
         TA0CTL = MC_1 + TASSEL_2 + TACLR;
         P1IE |= BIT1;
         P1IFG &= ~BIT1;


}

//Interrupt for port 2
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
       if((P2IN & BIT1) != BIT1)
       {
              if(DutyCycle1<50)
              {
                     int j;
                     DutyCycle1 = DutyCycle1 + 5;
                     for(j=0; j<30000; j++);
                     __low_power_mode_3();
              }
       }
       TA0CCTL2 = OUTMOD_7;
            TA0CCR2 = DutyCycle1;
            TA0CTL = MC_1 + TASSEL_2 + TACLR;
            P2IE |= BIT1;
            P2IFG &= ~BIT1;


}
