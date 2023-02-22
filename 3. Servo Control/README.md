# Servo Control
Servos can be controlled with PWM, determining which angle should be taken by how wide the duty cycle is. You will need to build a manual control for a servo using the two buttons on the development board.

## Task
You can chose whichever pin you want to control your Servo (but I might recommend one with hardware PWM control). Your will use the two buttons on your development board to control the position of your servo. The button on the left side of the board should turn the servo counterclockwise, the button on the right side of the board should turn it clockwise.

The servo will have a limit to the amount of degree it can rotate, so make sure you take a look at that before coding.

The servo will need to be most likely powered from the power supply on the bench. If you do this, you need to make sure you connect common ground between the supply and your board. Otherwise, your system will not work or you risk damaging your board.

## Deliverables
You will need to upload the .c file and a README explaining your code and any design decisions made.
