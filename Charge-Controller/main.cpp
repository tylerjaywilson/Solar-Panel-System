#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "uart.hpp"
#include "commreceive.hpp"
#include "commsend.hpp"

/* DEFINES */
#define NUM_BYTES6 6
#define NUM_BYTES8 8
#define RX_BUFF_MAX 255
#define QID_LEN 18
#define QPIRI_LEN 56
#define QPIGS_LEN 68
#define QPIWS_LEN 34
#define QBEQI_LEN 35

using namespace std;

int main (int argc, char* argv[])
{
	int uart_filestream = -1; 			//UART filestream for UART communication
	int baud_rate = 2400;				//Default 2400 value;

	UART uart0; 													//Create an object of class UART - Used to establish a UART connection
	uart_filestream = uart0.init(uart_filestream, baud_rate);		//Initialize the UART

	//Menu selection
	char menu_selection = -1;

	while(1)
	{
		CommSend commsend;												//Create an object of class CommSend - Used to send specific requests to the charge controller
		CommReceive commreceive;										//Create an object of class CommReceive - Used for accessing all the charge controller information
		/* THIS CODE IS USED FOR DEBUGGING THE CHARGE CONTROLLER PROTOCOL */

		printf("\nWhat info do you want (Enter the number of your selection): \n\n");
		printf("1. Serial Number\n");
		printf("2. Device Rated Info\n");
		printf("3. Device General Status\n");
		printf("4. Device Warning Status\n");
		printf("5. Battery Equalized Info\n");

		cin >> menu_selection;

		/* Transmit the inquiry */
		int tx_count = -1;
		switch (menu_selection)
		{
			case '1':
				tx_count = write(uart_filestream, &commsend.s_deviceSerialNum[0], NUM_BYTES6);	//write(filestream, starting address of inquiry, number of bytes to transmit)
				break;
			case '2':
				tx_count = write(uart_filestream, &commsend.s_deviceRatedInfo[0], NUM_BYTES8);
				break;
			case '3':
				tx_count = write(uart_filestream, &commsend.s_deviceGeneralStatusInfo[0], NUM_BYTES8);
				break;
			case '4':
				tx_count = write(uart_filestream, &commsend.s_deviceWarningStatus[0], NUM_BYTES8);
				break;
			case '5':
				tx_count = write(uart_filestream, &commsend.s_batteryEqualizedInfo[0], NUM_BYTES8);
				break;
			default:
				printf("Error: Incorrect entry!\n");
		}

		int rx_length, tot_length = 0;
		bool finished_reading = false;

		//Read until we have gathered all of the data from the previous inquiry
		while(!finished_reading)
		{
			/* Receive the information */
			switch (menu_selection)
			{
				case '1':
					rx_length = read(uart_filestream, (void*)(commreceive.rx_buffer+tot_length), RX_BUFF_MAX);	//read(filestream, storage buffer, number of bytes to read (max))
					tot_length += rx_length;
					if(tot_length == QID_LEN)			//The rx_length expectation (18) is hard-coded based on the expected length of the incoming data
					{
						finished_reading = true;
						//Parse the data
						commreceive.parseQID(commreceive.rx_buffer);
					}
					break;
				case '2':
					rx_length = read(uart_filestream, (void*)(commreceive.rx_buffer+tot_length), RX_BUFF_MAX);
					tot_length += rx_length;
					if(tot_length == QPIRI_LEN)
					{
						finished_reading = true;
						//Parse the data
						commreceive.parseQPIRI(commreceive.rx_buffer);
					}
					break;
				case '3':
					rx_length = read(uart_filestream, (void*)(commreceive.rx_buffer+tot_length), RX_BUFF_MAX);
					tot_length += rx_length;
					if(tot_length == QPIGS_LEN)
					{
						finished_reading = true;
						//Parse the data
						commreceive.parseQPIGS(commreceive.rx_buffer);
					}
					break;
				case '4':
					rx_length = read(uart_filestream, (void*)(commreceive.rx_buffer+tot_length), RX_BUFF_MAX);
					tot_length += rx_length;
					if(tot_length == QPIWS_LEN)
					{
						finished_reading = true;
						//Parse the data
						commreceive.parseQPIWS(commreceive.rx_buffer);
					}
					break;
				case '5':
					rx_length = read(uart_filestream, (void*)(commreceive.rx_buffer+tot_length), RX_BUFF_MAX);
					tot_length += rx_length;
					if(tot_length == QBEQI_LEN)
					{
						finished_reading = true;
						//Parse the data
						commreceive.parseQBEQI(commreceive.rx_buffer);
					}
					break;
				default:
					printf("Error: Incorrect entry!\n");
			}
		}	
	}
	return 1;
}
