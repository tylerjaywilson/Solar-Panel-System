/* Author: Tyler Wilson
* Date: 03/24/2016
* Notes: This class handles the i2c initialization for the raspberry pi 2
*
*/
#include <stdio.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include "i2c.hpp"

#define NUM_BYTES1 1
#define NUM_BYTES2 2
#define MAX_RX_BUFF 5

#define BITS8 8

//Default Constructor
I2C::I2C()
{
	filestream = -1;
	psoc_addr = 8;
}

/* Initialize the I2C depending address of the PSoC5 slave device */
void I2C::init()
{
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((filestream = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}
	
	//<<<<<The I2C address of the slave 
	if (ioctl(filestream, I2C_SLAVE, psoc_addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return;
	}	
}

// Write an 8 bit value to the PSoC5 slave device - This write value stored in 'buffer' is the register that we want to read from
void I2C::write8(unsigned char *buffer)
{
	if (write(filestream, buffer, NUM_BYTES1) != NUM_BYTES1)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
}

//Write a 16 bit value to the PSoC5 slave device - This write value stored in buffer contains the register that we want to 
//write to and the value that we are going to write to the aforementioned register.
void I2C::write16(unsigned char *buffer)
{
	if (write(filestream, buffer, NUM_BYTES2) != NUM_BYTES2)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
}

//Read an 8 bit value from the PSoC5 and return it after the appropriate request has been made.
int I2C::read8()
{
	unsigned char buffer[MAX_RX_BUFF] = {0};

	if (read(filestream, buffer, NUM_BYTES1) != NUM_BYTES1)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}

	//Convert the character value to an integer and return it.
	return ((int)(buffer[0]));
}

//Read a 16 bit value from the PSoC5 and return it after the appropriate request has been made
int I2C::read16()
{
	unsigned char buffer[MAX_RX_BUFF] = {0};

	if (read(filestream, buffer, NUM_BYTES2) != NUM_BYTES2)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}

	//Convert the two read characters into integers. Shift the HighByte of data to the left by 8 bits.
	//'And' the converted integer values to get the full 16 bit value. 
	int lowbyte = (int) buffer[0];
	int highbyte = (int) buffer[1];
	int returnVal = (highbyte << BITS8) & lowbyte;

	return returnVal;
}
