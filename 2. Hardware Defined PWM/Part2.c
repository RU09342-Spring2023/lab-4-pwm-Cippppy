/*
 * Part2.c
 *
 *  Created on: Feb 22, 2023
 *  Edited on: Feb 22, 2023
 *      Author: Christian Cipolletta
 *
 *
 */

#include <msp430.h>
#include "GPIO_Driver.h"    // MSP430FR2355 GPIO driver created in Lab 1


void LEDInit();         // Method to initialize LEDs
void TimerB0Init();


int main(void)
{
    LEDInit();      // Initialize our LEDS
    TimerB0Init();

    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM0_bits);             // Enter LPM0
    __no_operation();                         // For debugger
}


/*
 * INITIALIZATION METHODS
 */

// Method to initialize LEDs
void LEDInit() {

    // RED LED
    P6DIR |= BIT0;                     // P6.0 output
    P6SEL0 |= BIT0;
    P6SEL1 &= ~BIT0;                    // P6.0 options select

    // BLUE LED
    P6DIR |= BIT1;                     // P6.1 output
    P6SEL0 |= BIT1;
    P6SEL1 &= ~BIT1;                    // P6.1 options select

    // GREEN LED
    P6DIR |= BIT2;                     // P6.2 output
    P6SEL0 |= BIT2;
    P6SEL1 &= ~BIT2;                    // P6.2 options select
}

// Method to initialize TimerB0
void TimerB0Init() {
    // Configure Timer_B0
    TB3CCR0 = 1000-1;                         // PWM Period
    TB3CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCR1 = 0;                            // CCR1 PWM duty cycle
    TB3CCTL2 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCR2 = 1000-1;                            // CCR1 PWM duty cycle
    TB3CCTL3 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCR2 = 1000-1;                            // CCR1 PWM duty cycle
    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
}


/*
 * INTERRUPT ROUTINES
 */

// Timer0_B1 Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B3_VECTOR
__interrupt void TIMER0_B3_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B3_VECTOR))) TIMER0_B3_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB0IV,TB0IV_TBIFG))
    {
        case TB0IV_NONE:
            break;                               // No interrupt

        case TB0IV_TBCCR1:
            P1OUT &= ~BIT0;
            break;                               // CCR1 Set the pin to a 0

        case TB0IV_TBCCR2:
            break;                               // CCR2 not used

        case TB0IV_TBIFG:
            P1OUT |= BIT0;                       // overflow Set the pin to a 1
            break;

        default:
            break;
    }
}
