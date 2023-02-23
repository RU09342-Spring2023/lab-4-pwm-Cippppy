/*
 * Part1.c
 *
 *  Created on: Feb 22, 2023
 *  Edited on: Feb 22, 2023
 *      Author: Christian Cipolletta
 *
 *
 */

#include <msp430.h>
#include "GPIO_Driver.h"    // MSP430FR2355 GPIO driver created in Lab 1

#define DUTYCYCLEMAX 999;
#define DUTYCYCLEHALF 500;

void LEDInit();         // Method to initialize LEDs
void ButtonInit();      // Method to initialize buttons
void TimerA0Init();     // Method to initialize TimerA0
void TimerA1Init();     // Method to initialize TimerA1

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    LEDInit();      // Initialize our LEDS
    ButtonInit();   // Initialize our button
    TimerA0Init();  // Initialize Timer0
    TimerA1Init();  // Initialize Timer1

    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}


/*
 * INITIALIZATION METHODS
 */

// Method to initialize LEDs
void LEDInit() {
    gpioInit(1,0,1);                // Initializes Pin 1.0 (red led) as an output
    gpioWrite(1,0,0);               // Sets Pin 1.0 (red led) to low

    gpioInit(6,6,1);                // Initializes Pin 6.6 (green led) as an output
    gpioWrite(6,6,0);               // Sets Pin 6.6 (green led) to low
}

// Method to initialize buttons
void ButtonInit() {
    gpioInit(4,1,0);                // Initializes Pin 4.1 (button) as an input
    P4IES |= BIT1;                  // P4.1 High --> Low edge
    P4IE |= BIT1;                   // P4.1 interrupt enabled

    gpioInit(2,3,0);                // Initializes Pin 2.3 (button) as an input
    P2IES |= BIT3;                 // P2.3 High --> Low edge
    P2IE |= BIT3;                   // P2.3 interrupt enabled
}

// Method to initialize TimerA0
void TimerA0Init() {
    // Configure Timer_A0
    TB0CTL = TBSSEL_2 | MC_1 | TBCLR | TBIE;      // SMCLK, up mode, clear TBR, enable interrupt
    TB0CCTL1 |= CCIE;                             // Enable TB0 CCR1 Interrupt
    TB0CCR0 = DUTYCYCLEMAX;
    TB0CCR1 = DUTYCYCLEHALF;                   // Set CCR1 to the value to set the duty cycle
}

// Initialize Timer1
void TimerA1Init() {
    // Configure Timer_A1
    TB1CTL = TBSSEL_2 | MC_1 | TBCLR | TBIE;      // SMCLK, up, clear TBR, enable interrupt
    TB1CCTL1 |= CCIE;                             // Enable TB1 CCR1 Interrupt
    TB1CCR0 = DUTYCYCLEMAX;
    TB1CCR1 = DUTYCYCLEHALF;                   // Set CCR1 to the value to set the duty cycle
}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                         // Clear P2.3 IFG
    if(TB0CCR1 >= 999) {
        TB0CCR1 = 25;
    }
    else {
        TB0CCR1 += 100;
    }
}

// Port 4 interrupt service routine (For reset button)
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 IFG
    if(TB1CCR1 >= 999) {
        TB1CCR1 = 25;
    }
    else {
        TB1CCR1 += 100;
    }
}


// Timer0_B1 Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
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

// Timer1_B1 Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B1_VECTOR))) TIMER1_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(TB1IV,TB1IV_TBIFG))
    {
        case TB1IV_NONE:
            break;                               // No interrupt

        case TB1IV_TBCCR1:
            P6OUT &= ~BIT6;
            break;                               // CCR1 Set the pin to a 0

        case TB1IV_TBCCR2:
            break;                               // CCR2 not used

        case TB1IV_TBIFG:
            P6OUT |= BIT6;                       // overflow Set the pin to a 1
            break;

        default:
            break;
    }
}
