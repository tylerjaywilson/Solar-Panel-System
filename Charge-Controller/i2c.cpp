/* Author: Tyler Wilson
* Date: 03/24/2016
* Notes: This class handles the i2c initialization for the raspberry pi 2
*
*/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>				//Needed for I2C port
#include <fcntl.h>				//Needed for I2C port
#include <sys/ioctl.h>			//Needed for I2C port
#include <linux/i2c-dev.h>		//Needed for I2C port
#include "i2c.hpp"

#define 	NUM_BYTES1 		1
#define 	NUM_BYTES2 		2
#define 	MAX_RX_BUFF 	5

#define 	BITS8 			8

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
	char *filename = (char*)"/dev/i2c-1"; 	//Locate the RaPi i2c protocol location
	if ((filestream = open(filename, O_RDWR)) < 0)	//Verify that we are able to open the I2C bus. If the return value is less than zero then there is an error
	{
		//ERROR HANDLING: you can check errno to see what went wrong
		printf("Failed to open the i2c bus");
		return;
	}
	
	//The I2C address of the slave  - Acquire access/talk to the PSOC
	if (ioctl(filestream, I2C_SLAVE, psoc_addr) < 0)	//If the return value is less than zero then we were unable to access the PSoC
	{
		printf("Failed to acquire bus access and/or talk to slave.\n");
		//ERROR HANDLING; you can check errno to see what went wrong
		return;
	}	
}

// Write an 8 bit value to the PSoC5 slave device - This write value stored in 'buffer' is the register that we want to read from
void I2C::write8(int address, unsigned char *buffer)
{
	unsigned char addressBuff[MAX_RX_BUFF] = {0};
	addressBuff[0] = (unsigned char) address;

	//First write a byte to indicate which memory location we will be writing to.
	if (write(filestream, addressBuff, NUM_BYTES1) != NUM_BYTES1)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
	//Write the desired data to the memory location
	if (write(filestream, buffer, NUM_BYTES1) != NUM_BYTES1)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
}

//Write a 16 bit value to the PSoC5 slave device - This write value stored in buffer contains the register that we want to 
//write to and the value that we are going to write to the aforementioned register.
void I2C::write16(int address, unsigned char *buffer)
{
	unsigned char addressBuff[MAX_RX_BUFF] = {0};
	addressBuff[0] = (unsigned char) address;

	//First write a byte to indicate which memory location we will be writing to.
	if (write(filestream, addressBuff, NUM_BYTES1) != NUM_BYTES1)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
	//Write the desired data to the memory location
	if (write(filestream, buffer, NUM_BYTES2) != NUM_BYTES2)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
}

//Read an 8 bit value from the PSoC5 and return it after the appropriate request has been made.
int8_t I2C::read8(int address)
{	
	unsigned char tx_buffer[MAX_RX_BUFF] = {0};
	unsigned char rx_buffer[MAX_RX_BUFF] = {0};

	//Assign the adress that we want to read from to the transmit buffer
	tx_buffer[0] = (unsigned char) address;

	//Write the address that we want to access to the PSoC
	if (write(filestream, tx_buffer, NUM_BYTES1) != NUM_BYTES1)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
	//Read from the aforementioned address (only one byte)
	if (read(filestream, rx_buffer, NUM_BYTES1) != NUM_BYTES1)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", rx_buffer);
	}

	//Cast the character value to an 8 bit integer and return it.
	return ((int8_t)(rx_buffer[0]));
}

//Read a 16 bit value from the PSoC5 and return it after the appropriate request has been made
int16_t I2C::read16(int address)
{
	unsigned char tx_buffer[MAX_RX_BUFF] = {0};
	unsigned char rx_buffer[MAX_RX_BUFF] = {0};

	//Assign the desired address that we want to access from the PSoC to the transmit buffer
	tx_buffer[0] = (unsigned char) address;

	//printf("Address: %x\n", tx_buffer[0]);

	//Write one byte of data to the PSoC to indicate which memory address from which we are going to read
	if (write(filestream, tx_buffer, NUM_BYTES1) != NUM_BYTES1)		//write() returns the number of bytes actually written, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		/* ERROR HANDLING: i2c transaction failed */
		printf("Failed to write to the i2c bus.\n");
	}
	//Read two bytes of data from the aforementioned memory address
	if (read(filestream, rx_buffer, NUM_BYTES2) != NUM_BYTES2)		//read() returns the number of bytes actually read, if it doesn't match then an error occurred (e.g. no response from the device)
	{
		//ERROR HANDLING: i2c transaction failed
		printf("Failed to read from the i2c bus.\n");
	}
	else
	{
		printf("Data read: %s\n", rx_buffer);
	}

	//Convert the two read characters into integers. Shift the HighByte of data to the left by 8 bits.
	//'And' the converted integer values to get the full 16 bit value. 
	uint16_t lowbyte = (uint16_t) rx_buffer[0];
	uint16_t highbyte = (uint16_t) rx_buffer[1];
	int16_t returnVal = (int16_t) ((highbyte << BITS8) | lowbyte);
	//printf("Low Byte: %x High Byte: %x Combined Value: %x\n", lowbyte, highbyte, returnVal);

	return returnVal;
}
