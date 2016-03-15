#ifndef _COMMSEND_HPP
#define _COMMSEND_HPP

#define NUM_BYTES_TX_MAX 30
class CommSend{

public:

	//Inquiry parameters
	char s_deviceSerialNum[NUM_BYTES_TX_MAX];
	char s_deviceRatedInfo[NUM_BYTES_TX_MAX];
	char s_deviceGeneralStatusInfo[NUM_BYTES_TX_MAX];
	char s_deviceWarningStatus[NUM_BYTES_TX_MAX];
	char s_batteryEqualizedInfo[NUM_BYTES_TX_MAX];

	void CRCcalc(char*, uint8_t);
	CommSend();   //Default constructor
};

#endif