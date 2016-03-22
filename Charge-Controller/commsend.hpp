#ifndef _COMMSEND_HPP
#define _COMMSEND_HPP

#define NUM_BYTES_TX_MAX 30			//Max number of bytes in a request(transmit) array
class CommSend{

public:

	//Inquiry parameters
	char s_deviceSerialNum[NUM_BYTES_TX_MAX];			//QID request array
	char s_deviceRatedInfo[NUM_BYTES_TX_MAX];			//QPIRI request array
	char s_deviceGeneralStatusInfo[NUM_BYTES_TX_MAX];	//QPIGS request array
	char s_deviceWarningStatus[NUM_BYTES_TX_MAX];		//QPIWS request array
	char s_batteryEqualizedInfo[NUM_BYTES_TX_MAX];		//QBEQI request array

	void CRCcalc(char*, uint8_t);		//CRC calcuation function
	CommSend();   //Default constructor
};

#endif