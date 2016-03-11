#ifndef _UART_HPP
#define _UART_HPP

class UART
{

  int uart_filestream;
  int baud;

public:
  UART();   				//Default constructor
  int init(int, int); 	//Initialize uart for communication
  void uart_close(int);		//Close the uart communication
  void setFilestream(int);
  int getFilestream(void);
  void setBaud(int);
  int getBaud(void);
};

#endif