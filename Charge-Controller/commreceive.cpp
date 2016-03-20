/*
* Authors: Tyler Wilson
* Date: 02/24/2016
* Notes: This class handles the reception commands from the MPPT-3000 RS232 Charge Controller
* All of the charge controller information can be accessed through the 'get' functions for each charge controller parameter
* 
*/
#include <string>
#include <stdint.h>
#include <stdio.h>
#include <cstring>
#include "commreceive.hpp"

#define RX_LENGTH_MAX 256
#define IDLENGTH 14
#define INT_CONVERT 48

//Default Constructor
CommReceive::CommReceive()
{

	//Initialze buffer to zero
	rx_buffer[0] = {0};
	
	/* Serial number */
	serialNum = "-1";

  	/* QPIRI - Device rated information paramters */
	maxOutputPower = -1;
	nominalBattVoltage = -1;
	nominalChargingCurrent = -1.0;
	absorptionVoltage = -1.0;
	floatVoltage = -1.0;
	battType = -1;
	remoteBattVoltageDetect = -1;
	battTempCompensation = -1.0;
	remoteTempDetect = -1;
	battRatedVoltageSet = -1;
	battInSerial = -1;
	battLowWarningVoltage = -1.0;
	battLowShutdownDetect = -1;

	/* QPIGS - Device general status parameters */
	pvInputVoltage = -1.0;
	battVoltage = -1.0;
	chargingCurrent = -1.0;
	chargingCurrent1 = -1.0;
	chargingCurrent2 = -1.0;
	chargingPower = -1;
	unitTemp = -1;
	remoteBattVoltage = -1.0;
	remoteBattTemp = -1;
	reserved = -1;
	status = -1;

	/* QPIWS - Device warning status paramters */
	overChargeCurrent = -1;
	overTemp = -1;
	battVoltageUnder = -1;
	battVoltageHigh = -1;
	pvHighLoss = -1;
	battTempLow = -1;
	battTempHigh = -1;
	pvLowLoss = -1;
	pvHighDerating = -1;
	tempHighDerating = -1;
	battTempLowAlarm = -1;
	battLowWarning = -1;

	/* QBEQI - Battery equalized information */
	battEqualizedEn = -1;
	battEqualizedTime = -1;
	intervalTime = -1;
	maxCurrent = -1;
	remainingTime = -1;
	battEqualizedVoltage = -1.0;
	battCVChargeTime = -1;
	battEqualizedTimeout = -1;

	//Expected CRC
	expectedCRC = -1;
	receivedCRC = -1;
}

//Return the serial number
std::string CommReceive::getSerialNum()
{
	return serialNum;
}
int CommReceive::getmaxOutputPower()
{
	return maxOutputPower;
}
int CommReceive::getnominalBattVoltage()
{
	return nominalBattVoltage;
}
float CommReceive::getnominalChargingCurrent()
{
	return nominalChargingCurrent;
}
float CommReceive::getabsorptionVoltage()
{
	return absorptionVoltage;
}
float CommReceive::getfloatVoltage()
{
	return floatVoltage;
}
int CommReceive::getbattType()
{
	return battType;
}
int CommReceive::getremoteBattVoltageDetect()
{
	return remoteBattVoltageDetect;
}
float CommReceive::getbattTempCompensation()
{
	return battTempCompensation;
}
int CommReceive::getremoteTempDetect()
{
	return remoteTempDetect;
}
int CommReceive::getbattRatedVoltageSet()
{
	return battRatedVoltageSet;
}
int CommReceive::getbattInSerial()
{
	return battInSerial;
}
float CommReceive::getbattLowWarningVoltage()
{
	return battLowWarningVoltage;
}
int CommReceive::getbattLowShutdownDetect()
{
	return battLowShutdownDetect;
}
float CommReceive::getpvInputVoltage()
{
	return pvInputVoltage;
}
float CommReceive::getbattVoltage()
{
	return battVoltage;
}
float CommReceive::getchargingCurrent()
{
	return chargingCurrent;
}
float CommReceive::getchargingCurrent1()
{
	return chargingCurrent1;
}
float CommReceive::getchargingCurrent2()
{
	return chargingCurrent2;
}
int CommReceive::getchargingPower()
{
	return chargingPower;
}
int CommReceive::getunitTemp()
{
	return unitTemp;
}
float CommReceive::getremoteBattVoltage()
{
	return remoteBattVoltage;
}
int CommReceive::getremoteBattTemp()
{
	return remoteBattTemp;
}
int CommReceive::getstatus()
{
	return status;
}
int CommReceive::getoverChargeCurrent()
{
	return overChargeCurrent;
}
int CommReceive::getoverTemp()
{
	return overTemp;
}
int CommReceive::getbattVoltageUnder()
{
	return battVoltageUnder;
}
int CommReceive::getbattVoltageHigh()
{
	return battVoltageHigh;
}
int CommReceive::getpvHighLoss()
{
	return pvHighLoss;
}
int CommReceive::getbattTempLow()
{
	return battTempLow;
}
int CommReceive::getbattTempHigh()
{
	return battTempHigh;
}
int CommReceive::getpvLowLoss()
{
	return pvLowLoss;
}
int CommReceive::getpvHighDerating()
{
	return pvHighDerating;
}
int CommReceive::gettempHighDerating()
{
	return tempHighDerating;
}
int CommReceive::getbattTempLowAlarm()
{
	return battTempLowAlarm;
}
int CommReceive::getbattLowWarning()
{
	return battLowWarning;
}
int CommReceive::getbattEqualizedEn()
{
	return battEqualizedEn;
}
int CommReceive::getbattEqualizedTime()
{
	return battEqualizedTime;
}
int CommReceive::getintervalTime()
{
	return intervalTime;
}
int CommReceive::getmaxCurrent()
{
	return maxCurrent;
}
int CommReceive::getremainingTime()
{
	return remainingTime;
}
float CommReceive::getbattEqualizedVoltage()
{
	return battEqualizedVoltage;
}
int CommReceive::getbattCVChargeTime()
{
	return battCVChargeTime;
}
int CommReceive::getbattEqualizedTimeout()
{
	return battEqualizedTimeout;
}

//Cacluate the CRC and verify that the CRC is correct
void CommReceive::CRCcalc(unsigned char* tx_buff, uint8_t length)
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

    printf("CRC: %x \n", crc);
}
//Parse the Device serial number
void CommReceive::parseQID(unsigned char *rx_buffer_t)
{
	char *rx_buff_p = (strtok((char *) (rx_buffer_t+1), " ")); //rx_buffer_t+1 is used to remove the beginning '(' of the received data
	std::string id(rx_buff_p, rx_buff_p + IDLENGTH);
	serialNum = id;
	//printf("\nID STRING: %s\n", rx_buff_p);	
}

//Parse the device rating information
void CommReceive::parseQPIRI(unsigned char *rx_buffer_t)
{	
	//Read the next string of data until the 'space' delimiter - char * strtok ( char * str, const char * delimiters );
	char *rx_buff_p = (strtok((char *) (rx_buffer_t+1), " ")); //rx_buffer_t+1 is used to remove the beginning '(' of the received data
	maxOutputPower = atoi(rx_buff_p);
	//printf("\nMax power: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	nominalBattVoltage = atoi(rx_buff_p);
	//printf("\nNominal v: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	nominalChargingCurrent = atof(rx_buff_p);
	//printf("\nNomial i: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	absorptionVoltage = atof(rx_buff_p);
	//printf("\nAbsortion: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	floatVoltage = atof(rx_buff_p);
	//printf("\nFloat v: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	battType = atoi(rx_buff_p);
	//printf("\nBattery Type: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	remoteBattVoltageDetect = atoi(rx_buff_p);
	//printf("\nRemote Batt: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	battTempCompensation = atof(rx_buff_p);
	//printf("\nBattery Temp: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	remoteTempDetect = atoi(rx_buff_p);
	//printf("\nRemote Temp: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	battRatedVoltageSet = atoi(rx_buff_p);
	//printf("\nRated Voltage: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	battInSerial = atoi(rx_buff_p);
	//printf("\nBatt Serial: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	battLowWarningVoltage = atof(rx_buff_p);
	//printf("\nLow Warning: %s\n", rx_buff_p);
	
	rx_buff_p = strtok(NULL, " q");
	battLowShutdownDetect = atoi(rx_buff_p);
	//printf("\nShutdown Detect: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, "\r");
	receivedCRC = atoi(rx_buff_p);
	//printf("\nCRC: %s\n", rx_buff_p);

	//Clear the char array
	memset(&rx_buffer_t[0], 0, sizeof(rx_buffer_t));
}

//Parse the device general status information
void CommReceive::parseQPIGS(unsigned char *rx_buffer_t)
{
	char *rx_buff_p = (strtok((char *) (rx_buffer_t+1), " ")); //rx_buffer_t+1 is used to remove the beginning '(' of the received data
	pvInputVoltage = atof(rx_buff_p);
	//printf("\nPV Voltage: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	battVoltage = atof(rx_buff_p);
	//printf("\nBatt Voltage: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	chargingCurrent = atof(rx_buff_p);
	//printf("\nCharging i: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	chargingCurrent1 = atof(rx_buff_p);
	//printf("\nCharging1 i: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	chargingCurrent2 = atof(rx_buff_p);
	//printf("\nCharging2 i: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	chargingPower = atoi(rx_buff_p);
	//printf("\nCharging Power: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	unitTemp = atoi(rx_buff_p);
	//printf("\nUnit Temp: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	remoteBattVoltage = atof(rx_buff_p);
	//printf("\nRemote Batt V: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	remoteBattTemp = atoi(rx_buff_p);
	//printf("\nRemote Temp: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	reserved = atoi(rx_buff_p);
	//printf("\nReserved: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	status = atoi(rx_buff_p);
	//printf("\nStatus: %s\n", rx_buff_p);

	//rx_buff_p = strtok(NULL, "\r");
	//receivedCRC = atoi(rx_buff_p);
	//printf("\nCRC: %s\n", rx_buff_p);

	//Clear the char array
	memset(&rx_buffer_t[0], 0, sizeof(rx_buffer_t));
}

//Parse the device warning status information
void CommReceive::parseQPIWS(unsigned char *rx_buffer_t)
{
	/* Display the entire string if desired */
	//char *rx_buff_p = (strtok((char *) (rx_buffer_t+1), " ")); //rx_buffer_t+1 is used to remove the beginning '(' of the received data
	//printf("\nPV Voltage: %s\n", rx_buff_p);
	/*************************************/

	overChargeCurrent = (int)(rx_buffer_t[1] - INT_CONVERT);
	//printf("\nCharge Current: %u\n", overChargeCurrent);
	overTemp = (int)(rx_buffer_t[2] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", overTemp);
	battVoltageUnder = (int)(rx_buffer_t[3] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", battVoltageUnder);
	battVoltageHigh = (int)(rx_buffer_t[4] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", battVoltageHigh);
	pvHighLoss = (int)(rx_buffer_t[5] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", pvHighLoss);
	battTempLow = (int)(rx_buffer_t[6] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", battTempLow);
	battTempHigh = (int)(rx_buffer_t[7] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", battTempHigh);
	reserved = (int)(rx_buffer_t[8] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	reserved = (int)(rx_buffer_t[9] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	reserved = (int)(rx_buffer_t[10] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	reserved = (int)(rx_buffer_t[11] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	reserved = (int)(rx_buffer_t[12] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	reserved = (int)(rx_buffer_t[13] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	reserved = (int)(rx_buffer_t[14] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	reserved = (int)(rx_buffer_t[15] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	reserved = (int)(rx_buffer_t[16] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	reserved = (int)(rx_buffer_t[17] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	reserved = (int)(rx_buffer_t[18] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	reserved = (int)(rx_buffer_t[19] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", reserved);
	pvLowLoss = (int)(rx_buffer_t[20] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", pvLowLoss);
	pvHighDerating = (int)(rx_buffer_t[21] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", pvHighDerating);
	tempHighDerating = (int)(rx_buffer_t[22] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", tempHighDerating);
	battTempLowAlarm = (int)(rx_buffer_t[23] - INT_CONVERT);
	//printf("\nCharge Current: %d\n", battTempLowAlarm);

	//Clear the char array
	memset(&rx_buffer_t[0], 0, sizeof(rx_buffer_t));
}

//Parse the battery equalized information
void CommReceive::parseQBEQI(unsigned char *rx_buffer_t)
{
	char *rx_buff_p = (strtok((char *) (rx_buffer_t+1), " ")); //rx_buffer_t+1 is used to remove the beginning '(' of the received data
	battEqualizedEn = atoi(rx_buff_p);
	//printf("\nBatt Equalized En: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	battEqualizedTime = atoi(rx_buff_p);
	//printf("\nBatt Equalized Time: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	intervalTime = atoi(rx_buff_p);
	//printf("\nInterval Time: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	maxCurrent = atoi(rx_buff_p);
	//printf("\nMax Current: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	remainingTime = atoi(rx_buff_p);
	//printf("\nRemaining Time: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	battEqualizedVoltage = atof(rx_buff_p);
	//printf("\nBatt Equalized Voltage: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	battCVChargeTime = atoi(rx_buff_p);
	//printf("\nBatt CV Charge Time: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " U");
	battEqualizedTimeout = atoi(rx_buff_p);
	//printf("\nBatt Equalized Timeout: %s\n", rx_buff_p);

	//Clear the char array
	memset(&rx_buffer_t[0], 0, sizeof(rx_buffer_t));
}
