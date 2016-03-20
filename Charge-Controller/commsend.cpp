/*
* Authors: Tyler Wilson
* Date: 02/24/2016
* Notes: This class handles the transmission commands to the MPPT-3000 RS232 Charge Controller
* The necessary commands are hard-coded to allow for easy requesting of specific charge controller information
*/
#include <stdint.h>
#include <stdio.h>
#include "commsend.hpp"

#define LEN3 3
#define LEN5 5

//Cacluate the CRC and update the data buffer that will be sent
void CommSend::CRCcalc(char* tx_buff, uint8_t length)
{
	uint16_t crc;
	int crc_temp;
	uint8_t da;
	uint8_t bCRCHign;
    uint8_t bCRCLow;
    uint8_t len = length;
	uint16_t crc_ta[16]=
	{ 
		0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,

		0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef
	};
	crc=0;
	
	while(len--!=0) 
	{
		da=((uint8_t)(crc>>8))>>4; 

		crc<<=4;

		crc^=crc_ta[da^(*tx_buff>>4)]; 

		da=((uint8_t)(crc>>8))>>4; 

		crc<<=4;

		crc^=crc_ta[da^(*tx_buff&0x0f)]; 

		tx_buff++;
	}
	bCRCLow = crc;

    bCRCHign= (uint8_t)(crc>>8);

	if(bCRCLow==0x28||bCRCLow==0x0d||bCRCLow==0x0a)

    {
    	bCRCLow++;
    }
    if(bCRCHign==0x28||bCRCHign==0x0d||bCRCHign==0x0a)

    {
		bCRCHign++;
    }
    crc = ((uint16_t)bCRCHign)<<8;
    crc += bCRCLow;

    //Add the CRC to the end of the tx_buff and a carriage return
    //printf("CRC: %x \n", crc);
    crc_temp = crc;
    tx_buff[0] = (char)((crc_temp >> 8) & 0xFF); //Shift by 8 bits to get the first byte of the CRC
    tx_buff[1] = (char)(crc_temp & 0xFF); //Get the second byte of the CRC;
    tx_buff[2] = 0xD; //Add the carriage return;
}

//Default Constructor
CommSend::CommSend()
{
	//Inquiry parameters
	s_deviceSerialNum[0] = 'Q';
	s_deviceSerialNum[1] = 'I';
	s_deviceSerialNum[2] = 'D';
	CRCcalc(s_deviceSerialNum, LEN3);
	//s_deviceSerialNum[3] = 0xD6;
	//s_deviceSerialNum[4] = 0xEA;
	//s_deviceSerialNum[5] = 0xD;

	s_deviceRatedInfo[0] = 'Q';
	s_deviceRatedInfo[1] = 'P';
	s_deviceRatedInfo[2] = 'I';
	s_deviceRatedInfo[3] = 'R';
	s_deviceRatedInfo[4] = 'I';
	CRCcalc(s_deviceRatedInfo, LEN5);
	//s_deviceRatedInfo[5] = 0xF8;
	//s_deviceRatedInfo[6] = 0x54;
	//s_deviceRatedInfo[7] = 0xD;

	s_deviceGeneralStatusInfo[0] = 'Q';
	s_deviceGeneralStatusInfo[1] = 'P';
	s_deviceGeneralStatusInfo[2] = 'I';
	s_deviceGeneralStatusInfo[3] = 'G';
	s_deviceGeneralStatusInfo[4] = 'S';
	CRCcalc(s_deviceGeneralStatusInfo, LEN5);
	//s_deviceGeneralStatusInfo[5] = 0xB7;
	//s_deviceGeneralStatusInfo[6] = 0xA9;
	//s_deviceGeneralStatusInfo[7] = 0xD;

	s_deviceWarningStatus[0] = 'Q';
	s_deviceWarningStatus[1] = 'P';
	s_deviceWarningStatus[2] = 'I';
	s_deviceWarningStatus[3] = 'W';
	s_deviceWarningStatus[4] = 'S';
	CRCcalc(s_deviceWarningStatus, LEN5);
	//s_deviceWarningStatus[5] = 0xB4;
	//s_deviceWarningStatus[6] = 0xDA;
	//s_deviceWarningStatus[7] = 0xD;


	s_batteryEqualizedInfo[0] = 'Q';
	s_batteryEqualizedInfo[1] = 'B';
	s_batteryEqualizedInfo[2] = 'E';
	s_batteryEqualizedInfo[3] = 'Q';
	s_batteryEqualizedInfo[4] = 'I';
	CRCcalc(s_batteryEqualizedInfo, LEN5);
	//s_batteryEqualizedInfo[5] = 0x2E;
	//s_batteryEqualizedInfo[6] = 0xA9;
	//s_batteryEqualizedInfo[7] = 0xD;	
}
