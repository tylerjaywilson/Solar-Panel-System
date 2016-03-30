# Project: Solar-Panel-System
# Author: Tyler Wilson 
# Date: March 30, 2016

This project was completed to fulfill the requirements for ECE 6140 (Advanced Electrical Energy)
at USU (Utah State Univeristy). The code was designed and implemented on the RaspberryPi2.
This code does the following:

1. Establishes Communication (UART) with the MPPT3000 PCM60X Charge Controller
	a) Establishes the protocol specified in the PCM60X-RS232 Protocol datasheet
	b) Creates 'Set' functions to easily set charge controller parameters
	c) Creates 'Get' functions to easily read updated charge controller values (updated every 1 second)

2. Establishes Communication (I2C) with the Cypress PSoC5 Microcontoller
	a) Creates an I2C connection with the PSoC5 as the slave device
	b) Gets current and voltage readings from the PSoC5
	c) Creates 'Set' functions to easily alter PSoC5 behavior
	d) Creates 'Get' functions to easily read current PSOC5 values
