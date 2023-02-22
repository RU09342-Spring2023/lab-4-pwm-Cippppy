# Lab 4: Timers and PWM
In this lab, you will begin to use one of the most fundamental control techniques for a microcontroller. You can blink an LED, and you can blink it at a particular rate, but so far you have done symmetric waves. This means that you have you output be on and off for the same amount of time. What do you think will happen if you, say, keep the LED turned off for only 10% of the time instead of 50%? And what about those buttons, have you already had issues where your button accidentally triggers your code multiple times with interrupts enabled?

## Goals for the lab:
By the end of this lab, you should:
* Implement Software based PWM using the built in Timer Modules
* Implement Hardware based PWM using P1SEL and the timer modules
* Control an RBG LED and a Servo using PWM

## Deliverables
By the end of the lab, you will need to provide at a minimum well documented main.c files for *EACH* exercise with accompanying README files which explain exactly how to implement your code.

The .c and README files should include the minimum amount of work as well as any extra work that you have done. Each processor should have its project saved in its own folder in each part of the assignment, however you only need one README per part of the assignment.


### Header Files
You may find yourself by the end of this lab not having generated any header files, which in this case, ignore this section. If you have generated more than just a main.c you most likely have yourself a .h file or two. Remember from the first lab that any header files or libraries that you generate need to have their own README.md which tell the user what dependencies there are, how to install them into their projects, etc.

## Documentation
Since you will most likely be using pre-made code to make most of your code, so what I am going to require you to do is comment each line of code and what it is doing. If you are using code from TI themselves, it will most likely have some of the comments there. But I want you to use your own words when writing these comments. Dive into the datasheets, look into the MSP430.h file to see what registers are being manipulated and tell me why you need to configure them.
