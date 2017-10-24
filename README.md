# MainBoard_Framework
Mainboard - AT90CAN128 framework

## Program
We are using Eclipse but it can be done with manual compiling aswell. 
The code is uploaded with avrdude.

## Working principle
We are creating a structure that holds all the coordinates where the robot will go to. We keep track of the current position and status of the robot
and based on the current position we control actuators and checking "crash" sensors.
The board communicates with the odometry board via CAN at 125kbps.

## Overview of the code 
### Two main folders:
- ***src*** (all the .c files, not including the main.c and sides .c files)
- ***inc*** (all the .h files)

#### ***main.c***
The ***main.c*** is the starting point of our robots firmware and it is used to test algorithms, also it executes one of the sides code (darkside.c).

#### ***darkside.c***
The ***darkside.c*** is an example code for:
-  moving the robot to an x and y coordinate (odometry)
-  reading detection while moving
-  executing an algorithm on every position

#### ***system.c & system.h***
The ***system.c & system.h*** files include all the functions that are used for the system to run. Here we are running:
- timer for 90s
- interrupt for jumper (setup and reading)
- LED debug function
- system_init is where we init all the timers, interfaces (UART, CAN), ext.
In the ***.h*** file we are defining all the functions used in the ***.c*** and giving names to certain pins. 

#### ***odometry.c & odometry.h***
This is where we are defining all the functions that are connected with the odometry board. The working principle is:
- write data into a buffer
- send it to the odometry board via CAN
- retrieve information

#### ***actuator.c & actuator.h***
This is where we are interfacing with the actuator board and where we init all the PWM servos.
The servos are driven by 3x16bit PWM and 1x8bit PWM.
For interfacing with the actuator board we are using UART (for now), and as seen in the ***.h*** file we are using defines to send information to the actuator board.

#### ***can.c & can.h***
This file is used for communicating with the AT90CAN128s CAN interface. 
https://en.wikipedia.org/wiki/CAN_bus

#### ***gpio.c & gpio.h***
This file is used for simplifying work with the GPIOs.
First you need to register a pin and just after that you can read/write to it.

#### ***sensors.c & sensors.h***
This file is used for checking the sensors. (reading a pin)
If the big robot is used, we are including two more sensors.

#### ***usart.c & usart.h***
This file is used for communicating with the USART module of the microcontroller.
https://en.wikipedia.org/wiki/Universal_synchronous_and_asynchronous_receiver-transmitter

