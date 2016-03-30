/* Author: Tyler Wilson
* Date: 02/24/2016
* Notes: This class handles the uart initialization for the raspberry pi 2
*
*/

#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include <stdio.h>
#include <stdint.h>
#include "uart.hpp"

//Default Constructor
UART::UART()
{
	uart_filestream = -1;
	baud = 2400;		//For the MPPT-3000 Charge controller protocol	
}

/* Initialize the UART depending on the baud rate */
int UART::init(int uart_filestream_t, int baud_t)
{
	//-------------------------
	//----- SETUP USART 0 -----
	//-------------------------
	//At bootup, pins 8 and 10 are already set to UART0_TXD, UART0_RXD (ie the alt0 function) respectively
	int uart_filestream = -1;
	
	//OPEN THE UART
	//The flags (defined in fcntl.h):
	//	Access modes (use 1 of these):
	//		O_RDONLY - Open for reading only.
	//		O_RDWR - Open for reading and writing.
	//		O_WRONLY - Open for writing only.
	//
	//	O_NDELAY / O_NONBLOCK (same function) - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
	//											if there is no input immediately available (instead of blocking). Likewise, write requests can also return
	//											immediately with a failure status if the output can't be written immediately.
	//
	//	O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.
	uart_filestream = open("/dev/ttyAMA0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
	if (uart_filestream == -1)
	{
		//ERROR - CAN'T OPEN SERIAL PORT
		printf("Error - Unable to open UART.  Ensure it is not in use by another application!\n");
	}
	
	//CONFIGURE THE UART
	//The flags (defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html):
	//	Baud rate:- B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200, B230400, B460800, B500000, B576000, B921600, B1000000, B1152000, B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
	//	CSIZE:- CS5, CS6, CS7, CS8
	//	CLOCAL - Ignore modem status lines
	//	CREAD - Enable receiver
	//	IGNPAR = Ignore characters with parity errors
	//	ICRNL - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
	//	PARENB - Parity enable
	//	PARODD - Odd parity (else even)
	struct termios options;
	tcgetattr(uart_filestream, &options);

	if (baud_t == 2400)
		options.c_cflag = B2400 | CS8 | CLOCAL | CREAD;		//<Set baud rate 2400
	else
		printf("Error - Unable to handle this baud rate currently - Add this option to the code\n");

	options.c_iflag = IGNPAR;
	options.c_oflag = 0;
	options.c_lflag = 0;
	tcflush(uart_filestream, TCIFLUSH);
	tcsetattr(uart_filestream, TCSANOW, &options);

	return uart_filestream;
}

/* Close the current UART filestream */
void UART::uart_close(int uart_filestream_t)
{
	close(uart_filestream_t);
}

/* Get and set functions for the baud rate and uart_filestream */
void UART::setBaud(int baud_t)
{
	baud = baud_t;
}
int UART::getBaud()
{
	return baud;
}
void UART::setFilestream(int filestream)
{
	uart_filestream = filestream;
}
int UART::getFilestream()
{
	return uart_filestream;
}