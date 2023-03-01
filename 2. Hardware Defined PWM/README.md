# Hardware PWM: Fading RGB LED
This code uses a hardware PWM to make an RGB LED fade between 6 distinct colors. The colors in order are red, orange, green, cyan, blue, and purple. 

## Peripherals Used
- Pin 6.0, Red LED
Pin 6.0 is set to an output, select 0 is high, select 1 is low, toggled by Timer B3 interrupt.
- Pin 6.1, Blue LED
Pin 6.1 is set to an output, select 0 is high, select 1 is low, toggled by Timer B3 interrupt.
- Pin 6.2, Green LED
Pin 6.1 is set to an output, select 0 is high, select 1 is low, toggled by Timer B3 interrupt.
- Timer B0 Peripheral
Set to TB0CCR0 interrupt, ACLK, continuous, and divided by 8.
- Timer B3 Peripheral
Set to TB1CCR0 interrupt, SMCLK, up mode, out mode 7.

## Design Considerations
The registers of the MSP430FR2355 are limited to only 16-bits, or a decimal value of 65,535. This causes issues as the timer B0 register counts up because it will reach that value eventually even with a clock divider. To counteract this, the register is reset to 5 ever time it reachest 65,535. Also, timer B0's TB0CCR0 register has an offset of 5 because if it was any smaller, the fading of the LEDs would be too quick to see.

## How to use
First, load the code into code composer or another compiler of your choosing. Next, flash the program to the MSP430FR2355. Attach the anode of the common anode RGB LED to the 3.3V pin of the board. After, connect the LED pins (and needed resistors) to their corresponding pins on the board. After all the pins are connected correctly, the RGB will start fading and will do so as long as it is plugged in and powered.

