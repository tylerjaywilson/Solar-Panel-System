/*
* Authors: Tyler Wilson
* Date: 02/24/2016
* Notes: This class handles the transmission commands to the MPPT-3000 RS232 Charge Controller
* The necessary commands are hard-coded to allow for easy requesting of specific charge controller information
*/
#include <stdint.h>
#include "commsend.hpp"

//Default Constructor
CommSend::CommSend()
{
	//Inquiry parameters
	s_deviceSerialNum[0] = 'Q';
	s_deviceSerialNum[1] = 'I';
	s_deviceSerialNum[2] = 'D';
	s_deviceSerialNum[3] = 0xD6;
	s_deviceSerialNum[4] = 0xEA;
	s_deviceSerialNum[5] = 0xD;

	s_deviceRatedInfo[0] = 'Q';
	s_deviceRatedInfo[1] = 'P';
	s_deviceRatedInfo[2] = 'I';
	s_deviceRatedInfo[3] = 'R';
	s_deviceRatedInfo[4] = 'I';
	s_deviceRatedInfo[5] = 0xF8;
	s_deviceRatedInfo[6] = 0x54;
	s_deviceRatedInfo[7] = 0xD;

	s_deviceGeneralStatusInfo[0] = 'Q';
	s_deviceGeneralStatusInfo[1] = 'P';
	s_deviceGeneralStatusInfo[2] = 'I';
	s_deviceGeneralStatusInfo[3] = 'G';
	s_deviceGeneralStatusInfo[4] = 'S';
	s_deviceGeneralStatusInfo[5] = 0xB7;
	s_deviceGeneralStatusInfo[6] = 0xA9;
	s_deviceGeneralStatusInfo[7] = 0xD;

	s_deviceWarningStatus[0] = 'Q';
	s_deviceWarningStatus[1] = 'P';
	s_deviceWarningStatus[2] = 'I';
	s_deviceWarningStatus[3] = 'W';
	s_deviceWarningStatus[4] = 'S';
	s_deviceWarningStatus[5] = 0xB4;
	s_deviceWarningStatus[6] = 0xDA;
	s_deviceWarningStatus[7] = 0xD;


	s_batteryEqualizedInfo[0] = 'Q';
	s_batteryEqualizedInfo[1] = 'B';
	s_batteryEqualizedInfo[2] = 'E';
	s_batteryEqualizedInfo[3] = 'Q';
	s_batteryEqualizedInfo[4] = 'I';
	s_batteryEqualizedInfo[5] = 0x2E;
	s_batteryEqualizedInfo[6] = 0xA9;
	s_batteryEqualizedInfo[7] = 0xD;	
}