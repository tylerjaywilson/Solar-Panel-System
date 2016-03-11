#ifndef _COMMSEND_HPP
#define _COMMSEND_HPP

class CommSend{

public:

  /* Transmit buffer */
  unsigned char tx_buffer[20];
  unsigned int num_bytes;
  
  //Inquiry parameters
  char s_deviceSerialNum[6];
  char s_deviceRatedInfo[8];
  char s_deviceGeneralStatusInfo[8];
  char s_deviceWarningStatus[8];
  char s_batteryEqualizedInfo[8];
  
  CommSend();   //Default constructor
  
};

#endif