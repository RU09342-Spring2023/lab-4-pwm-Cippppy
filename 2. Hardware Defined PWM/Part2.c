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

volatile unsigned int redDutyCycle;
volatile unsigned int blueDutyCycle;
volatile unsigned int greenDutyCycle;
volatile unsigned char color;


void LEDInit();         // Method to initialize LEDs
void TimerB3Init();
void TimerB0Init();


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT

    redDutyCycle = 1000-1;
    blueDutyCycle = 1000-1;
    greenDutyCycle = 1000-1;
    color = 0;
    LEDInit();      // Initialize our LEDS
    TimerB3Init();
    TimerB0Init();

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    __bis_SR_register(LPM0_bits | LPM3_bits | GIE);             // Enter LPM0
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

// Method to initialize TimerB3
void TimerB3Init() {
    // Configure Timer_B3
    TB3CCR0 = 1000-1;                         // PWM Period

    TB3CCTL1 = OUTMOD_7;                      // CCR1 reset/set
    TB3CCR1 = redDutyCycle;                   // CCR1 PWM duty cycle

    TB3CCTL2 = OUTMOD_7;                      // CCR2 reset/set
    TB3CCR2 = blueDutyCycle;                  // CCR2 PWM duty cycle

    TB3CCTL3 = OUTMOD_7;                      // CCR3 reset/set
    TB3CCR3 = greenDutyCycle;                 // CCR3 PWM duty cycle

    TB3CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR
}

void TimerB0Init() {
        TB0CCTL0 = CCIE;                          // TB0CCR0 interrupt enabled
        TB0CCR0 = 5;
        TB0CTL = TBSSEL_1 | MC_2 | ID_3 | TBCLR | TBIE;          // ACLK, continuous mode, /8
}


/*
 * INTERRUPT ROUTINES
 */

// TimerB0 interrupt service routine
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer0_B0_ISR(void)
{
    switch(color) {
    case 0 :
        redDutyCycle = 0;
        blueDutyCycle = 1000-1;
        if(greenDutyCycle == 0)
            color = 1;
        else
            greenDutyCycle--;
        break;
    case 1 :
        blueDutyCycle = 1000-1;
        greenDutyCycle = 0;
        if(redDutyCycle == 999)
            color = 2;
        else
            redDutyCycle++;
        break;
    case 2 :
        redDutyCycle = 1000-1;
        greenDutyCycle = 0;
        if(blueDutyCycle == 0)
            color = 3;
        else
            blueDutyCycle--;
        break;
    case 3 :
        redDutyCycle = 1000-1;
        blueDutyCycle = 0;
        if(greenDutyCycle == 999)
            color = 4;
        else
            greenDutyCycle++;
        break;
    case 4 :
        blueDutyCycle = 0;
        greenDutyCycle = 1000-1;
        if(redDutyCycle == 0)
            color = 5;
        else
            redDutyCycle--;
        break;
    case 5 :
        redDutyCycle = 0;
        greenDutyCycle = 1000-1;
        if(blueDutyCycle == 999)
            color = 0;
        else
            blueDutyCycle++;
        break;
    }

    if(TB0CCR0 >= 60000)
        TB0CCR0 = 5;
    if(TB0R >= 60000)
        TB0R = 5;

    TB3CCR1 = redDutyCycle;                   // CCR1 PWM duty cycle
    TB3CCR2 = blueDutyCycle;                  // CCR2 PWM duty cycle
    TB3CCR3 = greenDutyCycle;                 // CCR3 PWM duty cycle
    TB0CCR0 += 5;
}
