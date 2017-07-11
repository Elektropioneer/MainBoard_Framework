# MainBoard_Framework
Mainboard - AT90CAN128 framework

## Program
We are using Eclipse but it can be done with manual compiling aswell. 
The code is uploaded with avrdude

## Working principle
We are creating a structure that holds all the coordinates the robot will go to. We keep track of the current position 
and based on the current position we control actuators and check "crash" sensors.
The board communicates with the odometry board via CAN at 125kbps.


