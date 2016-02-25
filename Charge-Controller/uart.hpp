#ifndef _UART_HPP
#define _UART_HPP

class UART{

  int uart_filestream;
  int baud;
  uint8_t tx_buffer[20];
  uint8_t *p_tx_buffer;

public:
  UART();   //Default constructor
  
};

#endif