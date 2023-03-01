# Servo Control
This code controls the angle of a servo by generating a hardware PWM. The angle increased with the press of one button and is decreased with the press of another.

## Peripherals Used
- Pin 2.3, Button that Increases Servo Angle
  - Pin 2.3 is set to an input, pullup resistor enabled, and interrupt enabled, and falling edge.
- Pin 4.1, Button that Decreases Servo Angle
  - Pin 4.1 is set to an input, pullup resistor enabled, and interrupt enabled, and falling edge.
- Pin 6.0, Servo Pin
  - Pin 6.0 is set to an output, select 0 is set to 1 and select 1 is set to 0
- Timer B3 Peripheral
  - Set to out mode 7, SMCLK, up mode, no interrupt.

## How to use
First, load the code into code composer or another compiler of your choosing. Next, flash the program to the MSP430FR2355. Connect the servo's ground wire to the board's ground, the servo's power wire to the board's 5V port, and the servo's signal wire to the board's pin 6.0. The code will start by putting the servo at approximately 90 degrees. The servo's angle can be incremented by 10 degrees by pressing the pin 2.3 button and decremented by 10 degrees by pressing the pin 4.1 button.
