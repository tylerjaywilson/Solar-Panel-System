#ifndef _UART_HPP
#define _UART_HPP

class UART
{
  int uart_filestream;
  int baud;

public:
  UART();   						//Default constructor
  int 	init(int, int); 			//Initialize uart for communication
  void 	uart_close(int);			//Close the uart communication
  void 	setFilestream(int);			//Set the filestream
  int 	getFilestream(void);		//Get the current filestream
  void 	setBaud(int);				//Set the baud rate
  int 	getBaud(void);				//Get the current baud rate
};

#endif