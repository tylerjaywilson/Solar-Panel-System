#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include "uart.hpp"
#include "commreceive.hpp"
#include "commsend.hpp"

using namespace std;

int main (int argc, char* argv[])
{
	int uart_filestream = -1; 			//UART filestream for UART communication
	int baud_rate = 2400;				//Default 2400 value;
	
	UART uart0; 													//Create an object of class UART
	CommSend commsend;												//Create an object of class CommSend
	CommReceive commreceive;										//Create an object of class CommReceive
	uart_filestream = uart0.init(uart_filestream, baud_rate);		//Initialize the UART

	//Menu selection
	char menu_selection = -1;

	while(1)
	{
		printf("What info do you want (Enter the number of your selection): \n\n");
		printf("1. Serial Number\n");
		printf("2. Device Rated Info\n");
		printf("3. Device General Status\n");
		printf("4. Device Warning Status\n");
		printf("5. Battery Equalized Info\n");

		cin >> menu_selection;

		switch (menu_selection)
		{
			case '1':

			case '2':

			case '3':

			case '4':

			case '5':

			default:
				printf("Error: Incorrect entry!\n");
		}

	}
	return 1;
}