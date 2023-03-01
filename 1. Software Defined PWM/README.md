# Software PWM: LED Brightness
This code uses a software PWM to control the brightness of two LEDs, one green and one red. There are two buttons that cycle through the determined brightneses, each correlating with one LED. The LEDs have 10 different levels and the buttons cycles through theses levels one at a time, reseting to level 1 after it reaches level 10.

## Peripherals
- Pin 1.0, Red LED
  - Pin 1.0 is set to an output, toggled by Timer B0interrupt, brightness is changed by Pin 2.3.
- Pin 6.6, Green LED
  - Pin 6.6 is set to an output, toggled by Timer B1interrupt, brightness is changed by Pin 4.1.
- Pin 2.3, Blinking Speed Button
  - Pin 2.3 is set to an input, pullup resistor enabled, interrupt enabled, falling edge.
- Pin 4.1, Reset Button
  - Pin 4.1 is set to an input, pullup resistor enabled, interrupt enabled, falling edge.
- Timer B0 Peripheral
  - Set to TB0CCR1 interrupt, SMCLK, up mode.
- Timer B1 Peripheral
  - Set to TB1CCR1 interrupt, SMCLK, up mode.

## How to use
First, load the code into code composer or another compiler of your choosing. Next, flash the program to the MSP430FR2355. It will begin by setting both the green and red LEDs to 50% duty cycle (about 50% brightness). You can press the 2.3 button to increment the red LED duty cycle by 10% and the 4.1 button does the same for the green LED. The duty cycle will increase until it hits 100%. The next button press after 100% will reset the duty cycle to 0%, allowing the pattern to repeat.
