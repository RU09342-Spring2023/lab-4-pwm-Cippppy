# Servo Control
This code controls the angle of a servo by generating a hardware PWM. The angle increased with the press of one button and is decreased with the press of another.

# Peripherals Used
- Pin 2.3, Button that Increases Servo Angle
Pin 2.3 is set to an input, pullup resistor enabled, and interrupt enabled, and falling edge.
- Pin 4.1, Button that Decreases Servo Angle
Pin 4.1 is set to an input, pullup resistor enabled, and interrupt enabled, and falling edge.
-Pin
- Timer B0 Peripheral
Set to TB0CCR0 interrupt, ACLK, continuous, and divided by 8.
- Timer B1 Peripheral
Set to TB1CCR0 interrupt, ACLK, continuous, and divided by 8.

# Design Considerations
The MSP430FR2355, while powerful, has its limitations. The registers are 16 bit meaning that they only can store values up to 65,535. ACLK, the clock choosen to be used for the timers, has a frequnecy between 32 kHz to 40 kHz. This means that the count, which is incremented every clock cycle that the button is pressed, will reach that value in about 2 seconds. To combat this issue, a clock divider was used that divided the frequency by 8. This allows the button to be delayed by up to 16 seconds and the button will still delay correctly.

# How to use
First, load the code into code composer or another compiler of your choosing. Next, flash the program to the MSP430FR2355. It will begin by blinking the red LED at a count of 8000 or about 2 seconds. If you would like to change the blink speed all you need to do is hold down the Pin 2.3 button. The amount of time you hold it down is the time in between the blinks of the red LED. Lastly, if the blinking speed is not to your liking, you can press 4.1 to reset it to the default or hold down Pin 2.3 again.
