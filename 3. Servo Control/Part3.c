/*
 * Part3.c
 *
 *  Created on: Feb 22, 2023
 *  Edited on: Feb 28, 2023
 *      Author: Christian Cipolletta
 *
 *
 */

#include <msp430.h>
#include "GPIO_Driver.h"    // MSP430FR2355 GPIO driver created in Lab 1


void ButtonInit();      // Method to initialize buttons
void ServoInit();       // Method to initialize servo pin
void TimerB3Init();     // Method to initialize TimerB3 which does the duty cycle

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
    ButtonInit();               // Method to initialize buttons
    ServoInit();                // Method to initialize servo pin
    TimerB3Init();              // Method to initialize TimerB3 which does the duty cycle

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM0_bits + LPM3_bits + GIE);             // Enter LPM0 and LPM3
    __enable_interrupt();                               // Enables interrupts
    __no_operation();                         // For debugger
}


/*
 * INITIALIZATION METHODS
 */

// Method to initialize buttons
void ButtonInit() {
    gpioInit(4,1,0);                // Initializes Pin 4.1 (button) as an input
    P4IES |= BIT1;                  // P4.1 High --> Low edge
    P4IE |= BIT1;                   // P4.1 interrupt enabled

    gpioInit(2,3,0);                // Initializes Pin 2.3 (button) as an input
    P2IES |= BIT3;                 // P2.3 High --> Low edge
    P2IE |= BIT3;                   // P2.3 interrupt enabled
}

// Method to initialize servo pin
void ServoInit() {

    P6DIR |= BIT0;                     // P6.0 output
    P6SEL0 |= BIT0;
    P6SEL1 &= ~BIT0;                    // P6.0 options select
}

// Method to initialize TimerB3 which does the duty cycle
void TimerB3Init() {
    // Configure Timer_B3
    TB3CCR0 = 20000;                         // PWM Period of 50Hz

    TB3CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCR1 = 20000*0.075;                   // CCR1 PWM duty cycle

    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                         // Clear P2.3 IFG
    if(TB3CCR1 >= 20000*0.125) {            // If duty cycle is max, stay at max
        TB3CCR1 = 20000*0.125;
    }
    else {
        TB3CCR1 += 100;                     // Else increase duty cycle by 10%
    }
}

// Port 4 interrupt service routine (For reset button)
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 IFG
    if(TB3CCR1 <= 20000*0.03) {             // If duty cycle is at min, stay at min
        TB3CCR1 = 20000*0.03;
    }
    else {
        TB3CCR1 -= 100;                     // Else decrease duty cycle by 10%
    }
}
