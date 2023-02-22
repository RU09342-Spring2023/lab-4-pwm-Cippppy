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

const int DUTYCYCLEMAX = 12000;

volatile unsigned int dutyCycleRed;
volatile unsigned char redLEDState = 0x00;
volatile unsigned char greenLEDState = 0x00;
volatile unsigned int dutyCycleGreen;

void LEDInit();
void ButtonInit();
void TimerA0Init();
void TimerA1Init();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    dutyCycleRed = DUTYCYCLEMAX;
    dutyCycleGreen = DUTYCYCLEMAX;
    LEDInit();      // Initialize our LEDS
    ButtonInit();   // Initialize our button
    TimerA0Init(); // Initialize Timer0
    TimerA1Init(); // Initialize Timer1
    // Disable the GPIO power-on default high-impedance mode
    // to activate previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;
    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
}


/*
 * INITIALIZATION METHODS
 */

void LEDInit() {
    gpioInit(1,0,1);                // Initializes Pin 1.0 (red led) as an output
    gpioWrite(1,0,0);               // Sets Pin 1.0 (red led) to low

    gpioInit(6,6,1);                // Initializes Pin 6.6 (green led) as an output
    gpioWrite(6,6,0);               // Sets Pin 6.6 (green led) to low
}

void ButtonInit() {
    gpioInit(4,1,0);                // Initializes Pin 4.1 (button) as an input
    P4IES |= BIT1;                  // P4.1 High --> Low edge
    P4IE |= BIT1;                   // P4.1 interrupt enabled

    gpioInit(2,3,0);                // Initializes Pin 2.3 (button) as an input
    P2IES |= BIT3;                  // P2.3 High --> Low edge
    P2IE |= BIT3;                   // P2.3 interrupt enabled
}

void TimerA0Init() {
    // Configure Timer_A0
    TB0CTL = TBSSEL_2 | MC_2 | TBCLR | TBIE;      // SMCLK, continuous mode, clear TBR, enable interrupt
    TB0CCTL1 |= CCIE;                             // Enable TB0 CCR1 Interrupt
    TB0CCR1 = dutyCycleRed/2;                          // Set CCR1 to the value to set the duty cycle
}

void TimerA1Init() {
    // Configure Timer_A1
    TB1CTL = TBSSEL_2 | MC_2 | TBCLR | TBIE;      // SMCLK, continuous mode, clear TBR, enable interrupt
    TB1CCTL1 |= CCIE;                             // Enable TB0 CCR1 Interrupt
    TB1CCR1 = dutyCycleGreen/2;                          // Set CCR1 to the value to set the duty cycle
}


/*
 * INTERRUPT ROUTINES
 */

// Port 2 interrupt service routine
#pragma vector=PORT2_VECTOR
__interrupt void Port_2(void)
{
    P2IFG &= ~BIT3;                         // Clear P2.3 IFG
    switch(redLEDState){
    case 0 :
        dutyCycleRed =  DUTYCYCLEMAX*0.5;
        redLEDState++;
        break;
    case 1 :
        dutyCycleRed =  DUTYCYCLEMAX*0.6;
        redLEDState++;
        break;
    case 2 :
        dutyCycleRed =  DUTYCYCLEMAX*0.7;
        redLEDState++;
        break;
    case 3 :
        dutyCycleRed =  DUTYCYCLEMAX*0.8;
        redLEDState++;
        break;
    case 5 :
        dutyCycleRed =  DUTYCYCLEMAX*0.9;
        redLEDState++;
        break;
    case 6 :
        dutyCycleRed =  DUTYCYCLEMAX*1.0;
        redLEDState++;
        break;
    case 7 :
        dutyCycleRed =  DUTYCYCLEMAX*0.0;
        redLEDState++;
        break;
    case 8 :
        dutyCycleRed =  DUTYCYCLEMAX*0.1;
        redLEDState++;
        break;
    case 9 :
        dutyCycleRed =  DUTYCYCLEMAX*0.2;
        redLEDState++;
        break;
    case 10 :
        dutyCycleRed =  DUTYCYCLEMAX*0.3;
        redLEDState++;
        break;
    case 11 :
        dutyCycleRed =  DUTYCYCLEMAX*0.4;
        redLEDState = 0;
        break;
    }
}

// Port 4 interrupt service routine (For reset button)
#pragma vector=PORT4_VECTOR
__interrupt void Port_4(void)
{
    P4IFG &= ~BIT1;                         // Clear P4.1 IFG
    dutyCycleGreen *= 2;
}

// Timer0_B3 Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER0_B1_VECTOR))) TIMER0_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    TB0CCR1 += dutyCycleRed;
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

// Timer1_B3 Interrupt Vector (TBIV) handler
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=TIMER1_B1_VECTOR
__interrupt void TIMER1_B1_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(TIMER1_B1_VECTOR))) TIMER1_B1_ISR (void)
#else
#error Compiler not supported!
#endif
{
    TB1CCR1 += dutyCycleGreen;
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
