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

//Default Constructor
I2C::I2C()
{
	
}

/* Initialize the UART depending on the baud rate */
void I2C::i2c_init()
{
	int file_i2c;
	int length;
	unsigned char buffer[60] = {0};

	
	//----- OPEN THE I2C BUS -----
	char *filename = (char*)"/dev/i2c-1";
	if ((file_i2c = open(filename, O_RDWR)) < 0)
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}
	
	int addr = 0x5a;          //<<<<<The I2C address of the slave - Must change this to match the PSoC
	if (ioctl(file_i2c, I2C_SLAVE, addr) < 0)
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return;
	}
	
	
	//----- READ BYTES -----
	length = 4;			//<<< Number of bytes to read - Change this to match the PSoC
	if (read(file_i2c, buffer, length) != length)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", buffer);
	}

	
	//----- WRITE BYTES -----
	buffer[0] = 0x01;
	buffer[1] = 0x02;
	length = 2;			//<<< Number of bytes to write
	if (write(file_i2c, buffer, length) != length)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
}