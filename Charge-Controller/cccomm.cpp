/*
* Authors: Tyler Wilson
* Date: 03/28/2016
* Notes: This class handles the send and receive commands from the MPPT-3000 RS232 Charge Controller
* All of the charge controller information can be accessed through the 'get' functions for each charge controller parameter
* All of the charge controller parameters can be set through the 'set' functions for each charge controller parameter
*/

#include <string>
#include <stdint.h>
#include <stdio.h>
#include <cstring>
#include <stdlib.h>
#include "cccomm.hpp"

#define RX_LENGTH_MAX 256	//The max receive buffer size is set to 256
#define IDLENGTH 14			//The device ID length is expected to be 14
#define INT_CONVERT 48		//Converts character integers to actual integer values

/* These #define's indicate the number of bytes within any given character array */
#define LEN2 2
#define LEN3 3
#define LEN5 5
#define LEN6 6
#define LEN8 8
#define LEN9 9
#define LEN10 10

#define AGM 0
#define FLOODED 1
#define CUSTOM 2

#define ENABLE 1
#define DISABLE 0

#define TRIPDIGIT 100
#define DOUBDIGIT 10

#define INTEGER_BASE 10

//Default Constructor
CCComm::CCComm()
{
	/*****************SEND VARIABLES*****************/

	/*******Inquiry parameters*****/
	//QID
	s_deviceSerialNum[0] = 'Q';
	s_deviceSerialNum[1] = 'I';
	s_deviceSerialNum[2] = 'D';
	CRCcalc(s_deviceSerialNum, LEN3);	//Calculate the CRC and append the CRC and carriage return to the array
	
	//QPIRI
	s_deviceRatedInfo[0] = 'Q';
	s_deviceRatedInfo[1] = 'P';
	s_deviceRatedInfo[2] = 'I';
	s_deviceRatedInfo[3] = 'R';
	s_deviceRatedInfo[4] = 'I';
	CRCcalc(s_deviceRatedInfo, LEN5);	//Calculate the CRC and append the CRC and carriage return to the array

	//QPIGS
	s_deviceGeneralStatusInfo[0] = 'Q';
	s_deviceGeneralStatusInfo[1] = 'P';
	s_deviceGeneralStatusInfo[2] = 'I';
	s_deviceGeneralStatusInfo[3] = 'G';
	s_deviceGeneralStatusInfo[4] = 'S';
	CRCcalc(s_deviceGeneralStatusInfo, LEN5);	//Calculate the CRC and append the CRC and carriage return to the array
	
	//QPIWS
	s_deviceWarningStatus[0] = 'Q';
	s_deviceWarningStatus[1] = 'P';
	s_deviceWarningStatus[2] = 'I';
	s_deviceWarningStatus[3] = 'W';
	s_deviceWarningStatus[4] = 'S';
	CRCcalc(s_deviceWarningStatus, LEN5);	//Calculate the CRC and append the CRC and carriage return to the array

	//QBEQI
	s_batteryEqualizedInfo[0] = 'Q';
	s_batteryEqualizedInfo[1] = 'B';
	s_batteryEqualizedInfo[2] = 'E';
	s_batteryEqualizedInfo[3] = 'Q';
	s_batteryEqualizedInfo[4] = 'I';
	CRCcalc(s_batteryEqualizedInfo, LEN5);	//Calculate the CRC and append the CRC and carriage return to the array
	/******END OF INQUIRY PARAMETERS************/

	/*******Setting Parameters********/
	//PBT
	s_setBattType[0] = 'P';		//PBT setting array
	s_setBattType[1] = 'B';
	s_setBattType[2] = 'T';
	s_setBattType[3] = '0';		//Default battery type - 00 - AGM
	s_setBattType[4] = '0';	
	CRCcalc(s_setBattType, LEN5);	//Calculate the CRC and append the CRC and carriage return to the array

	//PBAV
	s_setBattAbsorbtionChargingVoltage[0] = 'P';	//PBAV setting array
	s_setBattAbsorbtionChargingVoltage[1] = 'B';
	s_setBattAbsorbtionChargingVoltage[2] = 'A';
	s_setBattAbsorbtionChargingVoltage[3] = 'V';
	s_setBattAbsorbtionChargingVoltage[4] = '1';	//Set to default absorbtion voltage - 14.10
	s_setBattAbsorbtionChargingVoltage[5] = '4';
	s_setBattAbsorbtionChargingVoltage[6] = '.';
	s_setBattAbsorbtionChargingVoltage[7] = '1';
	s_setBattAbsorbtionChargingVoltage[8] = '0';
	CRCcalc(s_setBattAbsorbtionChargingVoltage, LEN9);	//Calculate the CRC and append the CRC and carriage return to the array

	//PBFV
	s_setBattFloatingChargingVoltage[0] = 'P';		//PBFV setting array
	s_setBattFloatingChargingVoltage[1] = 'B';
	s_setBattFloatingChargingVoltage[2] = 'F';
	s_setBattFloatingChargingVoltage[3] = 'V';
	s_setBattFloatingChargingVoltage[4] = '1';		//Set the default floating voltage - 13.50
	s_setBattFloatingChargingVoltage[5] = '3';
	s_setBattFloatingChargingVoltage[6] = '.';
	s_setBattFloatingChargingVoltage[7] = '5';
	s_setBattFloatingChargingVoltage[8] = '0';
	CRCcalc(s_setBattFloatingChargingVoltage, LEN9);	//Calculate CRC and append with carriage return

	//PBRV
	s_setRatedBattVoltage[0] = 'P';		//PBRV setting array
	s_setRatedBattVoltage[1] = 'B';
	s_setRatedBattVoltage[2] = 'R';
	s_setRatedBattVoltage[3] = 'V';
	s_setRatedBattVoltage[4] = '0';		//Set the default to auto-sensing - 00
	s_setRatedBattVoltage[5] = '0';
	CRCcalc(s_setRatedBattVoltage, LEN6);	//Append CRC

	//MCHGC
	s_setMaxChargingCurrent[0] = 'M';	//MCHGC setting array
	s_setMaxChargingCurrent[1] = 'C';
	s_setMaxChargingCurrent[2] = 'H';
	s_setMaxChargingCurrent[3] = 'G';
	s_setMaxChargingCurrent[4] = 'C';
	s_setMaxChargingCurrent[5] = '0';	//Set the default to 60 A
	s_setMaxChargingCurrent[6] = '6';
	s_setMaxChargingCurrent[7] = '0';
	CRCcalc(s_setMaxChargingCurrent, LEN8);	//Append CRC

	//PRBD
	s_enRemoteBatteryVoltageDetect[0] = 'P';		//PRBD setting array
	s_enRemoteBatteryVoltageDetect[1] = 'R';
	s_enRemoteBatteryVoltageDetect[2] = 'B';
	s_enRemoteBatteryVoltageDetect[3] = 'D';
	s_enRemoteBatteryVoltageDetect[4] = '0';		//Set the default to disabled for the remote batt voltage detect
	s_enRemoteBatteryVoltageDetect[5] = '0';
	CRCcalc(s_enRemoteBatteryVoltageDetect, LEN6);	//Append CRC

	//PBLV
	s_setBattLowWarningVoltage[0] = 'P';		//PBLV setting array
	s_setBattLowWarningVoltage[1] = 'B';
	s_setBattLowWarningVoltage[2] = 'L';
	s_setBattLowWarningVoltage[3] = 'V';
	s_setBattLowWarningVoltage[4] = '1';		//Set the default battery low warning voltage to 11.25 V
	s_setBattLowWarningVoltage[5] = '1';
	s_setBattLowWarningVoltage[6] = '.';
	s_setBattLowWarningVoltage[7] = '2';
	s_setBattLowWarningVoltage[8] = '5';
	CRCcalc(s_setBattLowWarningVoltage, LEN9);		//Append CRC

	//PBLSE
	s_setBattLowShutdownDetectEn[0] = 'P';			//PBLSE setting array
	s_setBattLowShutdownDetectEn[1] = 'B';
	s_setBattLowShutdownDetectEn[2] = 'L';
	s_setBattLowShutdownDetectEn[3] = 'S';
	s_setBattLowShutdownDetectEn[4] = 'E';
	s_setBattLowShutdownDetectEn[5] = '1';			//Set the default to enabled for low battery shutdown detect
	CRCcalc(s_setBattLowShutdownDetectEn, LEN6);	//Append CRC

	//PBEQE
	s_setBattEqualizationEn[0] = 'P';		//PBEQE setting array
	s_setBattEqualizationEn[1] = 'B';
	s_setBattEqualizationEn[2] = 'E';
	s_setBattEqualizationEn[3] = 'Q';
	s_setBattEqualizationEn[4] = 'E';
	s_setBattEqualizationEn[5] = '0';		//Set the default to disabled for the battery equalization
	CRCcalc(s_setBattEqualizationEn, LEN6);		//Append CRC

	//PBEQT
	s_setBattEqualizedTime[0] = 'P';		//PBEQT setting array
	s_setBattEqualizedTime[1] = 'B';
	s_setBattEqualizedTime[2] = 'E';
	s_setBattEqualizedTime[3] = 'Q';
	s_setBattEqualizedTime[4] = 'T';
	s_setBattEqualizedTime[5] = '0';		//Set the default to 60 minutes for the battery equalized time
	s_setBattEqualizedTime[6] = '6';
	s_setBattEqualizedTime[7] = '0';
	CRCcalc(s_setBattEqualizedTime, LEN8);	//Append CRC

	//PBEQP
	s_setPeriodBattEqualization[0] = 'P';			//PBEQP setting array
	s_setPeriodBattEqualization[1] = 'B';
	s_setPeriodBattEqualization[2] = 'E';
	s_setPeriodBattEqualization[3] = 'Q';
	s_setPeriodBattEqualization[4] = 'P';
	s_setPeriodBattEqualization[5] = '0';		//Default period of battery equalization is 30 days
	s_setPeriodBattEqualization[6] = '3';
	s_setPeriodBattEqualization[7] = '0';
	CRCcalc(s_setPeriodBattEqualization, LEN8);	//Append CRC

	//PBEQMC
	s_setMaxCurrentBatteryEqualization[0] = 'P';	//PBEQMC setting array
	s_setMaxCurrentBatteryEqualization[1] = 'B';
	s_setMaxCurrentBatteryEqualization[2] = 'E';
	s_setMaxCurrentBatteryEqualization[3] = 'Q';
	s_setMaxCurrentBatteryEqualization[4] = 'M';
	s_setMaxCurrentBatteryEqualization[5] = 'C';
	s_setMaxCurrentBatteryEqualization[6] = '0';	//Set the default max current of batt equalization to 15 A
	s_setMaxCurrentBatteryEqualization[7] = '1';
	s_setMaxCurrentBatteryEqualization[8] = '5';
	CRCcalc(s_setMaxCurrentBatteryEqualization, LEN9);	//Append CRC

	//PBEQV
	s_setBattEqualizedVoltage[0] = 'P';			//PBEQV setting array
	s_setBattEqualizedVoltage[1] = 'B';
	s_setBattEqualizedVoltage[2] = 'E';
	s_setBattEqualizedVoltage[3] = 'Q';
	s_setBattEqualizedVoltage[4] = 'V';
	s_setBattEqualizedVoltage[5] = '1';			//Set the default batt equalized voltage to 14.60 V
	s_setBattEqualizedVoltage[6] = '4';
	s_setBattEqualizedVoltage[7] = '.';
	s_setBattEqualizedVoltage[8] = '6';
	s_setBattEqualizedVoltage[9] = '0';
	CRCcalc(s_setBattEqualizedVoltage, LEN10);	//Append CRC

	//PBCVT
	s_setBattCVChargeTime[0] = 'P';				//PBCVT setting array
	s_setBattCVChargeTime[1] = 'B';
	s_setBattCVChargeTime[2] = 'C';
	s_setBattCVChargeTime[3] = 'V';
	s_setBattCVChargeTime[4] = 'T';
	s_setBattCVChargeTime[5] = '1';				//Default charge time set to 150 minutes
	s_setBattCVChargeTime[6] = '5';
	s_setBattCVChargeTime[7] = '0';
	CRCcalc(s_setBattCVChargeTime, LEN8);	//Append CRC

	//PBEQOT
	s_setTimeBatteryEqualizedTimeout[0] = 'P';		//PBEQOT setting array
	s_setTimeBatteryEqualizedTimeout[1] = 'B';
	s_setTimeBatteryEqualizedTimeout[2] = 'E';
	s_setTimeBatteryEqualizedTimeout[3] = 'Q';
	s_setTimeBatteryEqualizedTimeout[4] = 'O';
	s_setTimeBatteryEqualizedTimeout[5] = 'T';
	s_setTimeBatteryEqualizedTimeout[6] = '1';		//Default equalized timeout to 180 minutes
	s_setTimeBatteryEqualizedTimeout[7] = '8';
	s_setTimeBatteryEqualizedTimeout[8] = '0';
	CRCcalc(s_setTimeBatteryEqualizedTimeout, LEN9);	//Apend CRC

	//PF
	/*
	PF<cr>: Setting control parameter to default value
	Computer: PF<CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	All Device parameters set to default value.

	Item	 								|	Default
	--------------------------------------------------------------
	Battery type            				|	0 - AGM
	Battery voltage 						|	00 - Auto sensing
	Max charging current 					|	060 - 60A
	BTS temperature compensation 			|	00.0 - 0mV
	Remote battery voltage detect 			|	00 - Disable
	Absorption voltage 						|	14.10V
	Floating voltage 						|	13.50V
	Battery equalized enable/disable 		| 	disable
	Battery equalized time 					|	60minutes
	Interval time of battery equalization 	|	30 Days
	The max current of battery equalization	|	15A
	Battery equalized voltage per unit		|	14.60V
	Battery C.V. charge time 				| 	150minutes
	*/
	s_setControlParameterDefault[0] = 'P';			//PF setting array
	s_setControlParameterDefault[1] = 'F';			//Reset to all default parameters
	CRCcalc(s_setControlParameterDefault, LEN2);	//Append CRC	
	

	/******************RECEIVE VARIABLES********************/

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


//Cacluate the CRC and verify that the CRC is correct - This code was received from 
//the Charge Controller MPPT-3000 company.
void CCComm::CRCcalc(char* tx_buff, uint8_t length)
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

    //print the calculated CRC value
    //printf("CRC: %x \n", crc);
}

//Parse the Device serial number
/*
	QID<cr>: The device serial number inquiry
	Computer: QID <CRC><cr>
	Device: (XXXXXXXXXXXXXX <CRC><cr>
*/
void CCComm::parseQID(unsigned char *rx_buffer_t)
{
	char *rx_buff_p = (strtok((char *) (rx_buffer_t+1), " ")); //rx_buffer_t+1 is used to remove the beginning '(' of the received data
	std::string id(rx_buff_p, rx_buff_p + IDLENGTH);
	serialNum = id;
	//printf("\nID STRING: %s\n", rx_buff_p);	
}

//Parse the device rating information
/*
	QPIRI<cr>: Device Rated Information inquiry
	Computer: QPIRI<CRC><cr>
	Device: (BBBB CC DD.D EE.EE FF.FF GG HH II.I JJ KKKK L MM.MM N<CRC><cr>
*/
void CCComm::parseQPIRI(unsigned char *rx_buffer_t)
{	
	//Read the next string of data until the 'space' delimiter - char * strtok ( char * str, const char * delimiters );
	char *rx_buff_p = (strtok((char *) (rx_buffer_t+1), " ")); //rx_buffer_t+1 is used to remove the beginning '(' of the received data
	maxOutputPower = atoi(rx_buff_p);
	//printf("\nMax power: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");	//read the next set of data until the next 'space'
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
/*
	QPIGS<cr>: Device general status parameters inquiry
	Computer: QPIGS <CRC><cr>
	Device: (BBB.B CC.CC DD.DD EE.EE FF.FF GGGG ±HHH II.II ±JJJ KKKK b7b6b5b4b3b2b1b0 <CRC><cr>
*/
void CCComm::parseQPIGS(unsigned char *rx_buffer_t)
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
/*
	QPIWS<cr>: Device Warning Status inquiry
	Computer: QPIWS<CRC> <cr>
	Device: (a1a2.....a14a15-a30<CRC><cr>

	a1,..., a30 is the warning status. If the warning happened, the relevant bit will set to 1, else the
	relevant bit will set 0. The following table is the warning code.

	bit | 			Warning	 			|	Description		|	
	---------------------------------------------------------
	a1 		Over charge current 			Fault
	a2 		Over temperature 				Fault
	a3 		Battery voltage under 			Fault
	a4 		Battery voltage high 			Fault
	a5 		PV high loss 					Fault
	a6 		Battery temperature too low 	Fault
	a7 		Battery temperature too high 	Fault
	a8 		Reserved 						Reserved
	a9 		Reserved 						Reserved
	a10 	Reserved 						Reserved
	a11 	Reserved 						Reserved
	a12 	Reserved 						Reserved
	a13 	Reserved 						Reserved
	a14 	Reserved 						Reserved
	a15 	Reserved 						Reserved
	a16 	Reserved 						Reserved
	a17 	Reserved 						Reserved
	a18 	Reserved 						Reserved
	a19 	Reserved 						Reserved
	a20 	PV low loss 					Warning
	a21 	PV high derating 				Warning
	a22 	Temperature high derating 		Warning
	a23 	Battery temperature low alarm 	Warning
	a30 	Battery low warning 			Just for AS400 card

*/
void CCComm::parseQPIWS(unsigned char *rx_buffer_t)
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
/*
	QBEQI<cr>: The battery equalized information
	Computer: QBEQI<CRC><cr>
	Device: (B CCC DDD EEE FFF GG.GG HHH III<CRC><cr>
*/
void CCComm::parseQBEQI(unsigned char *rx_buffer_t)
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

/*
	Parse the ACKNAK response to a setting command
*/
bool CCComm::parseACKNAK(unsigned char *rx_buffer_t)
{
	//Check to see if an ACKNOWLEDGED was received
	if((rx_buffer[1] == 'A') && (rx_buffer_t[2] == 'C') && (rx_buffer_t[3] == 'K'))
		return true;
	else	//A NOT ACKNOWLEDGED was received
		return false;
}

/**************Get functions for charge controller parameters*************/

std::string CCComm::getSerialNum()
{
	return serialNum;
}
int CCComm::getmaxOutputPower()
{
	return maxOutputPower;
}
int CCComm::getnominalBattVoltage()
{
	return nominalBattVoltage;
}
float CCComm::getnominalChargingCurrent()
{
	return nominalChargingCurrent;
}
float CCComm::getabsorptionVoltage()
{
	return absorptionVoltage;
}
float CCComm::getfloatVoltage()
{
	return floatVoltage;
}
int CCComm::getbattType()
{
	return battType;
}
int CCComm::getremoteBattVoltageDetect()
{
	return remoteBattVoltageDetect;
}
float CCComm::getbattTempCompensation()
{
	return battTempCompensation;
}
int CCComm::getremoteTempDetect()
{
	return remoteTempDetect;
}
int CCComm::getbattRatedVoltageSet()
{
	return battRatedVoltageSet;
}
int CCComm::getbattInSerial()
{
	return battInSerial;
}
float CCComm::getbattLowWarningVoltage()
{
	return battLowWarningVoltage;
}
int CCComm::getbattLowShutdownDetect()
{
	return battLowShutdownDetect;
}
float CCComm::getpvInputVoltage()
{
	return pvInputVoltage;
}
float CCComm::getbattVoltage()
{
	return battVoltage;
}
float CCComm::getchargingCurrent()
{
	return chargingCurrent;
}
float CCComm::getchargingCurrent1()
{
	return chargingCurrent1;
}
float CCComm::getchargingCurrent2()
{
	return chargingCurrent2;
}
int CCComm::getchargingPower()
{
	return chargingPower;
}
int CCComm::getunitTemp()
{
	return unitTemp;
}
float CCComm::getremoteBattVoltage()
{
	return remoteBattVoltage;
}
int CCComm::getremoteBattTemp()
{
	return remoteBattTemp;
}
int CCComm::getstatus()
{
	return status;
}
int CCComm::getoverChargeCurrent()
{
	return overChargeCurrent;
}
int CCComm::getoverTemp()
{
	return overTemp;
}
int CCComm::getbattVoltageUnder()
{
	return battVoltageUnder;
}
int CCComm::getbattVoltageHigh()
{
	return battVoltageHigh;
}
int CCComm::getpvHighLoss()
{
	return pvHighLoss;
}
int CCComm::getbattTempLow()
{
	return battTempLow;
}
int CCComm::getbattTempHigh()
{
	return battTempHigh;
}
int CCComm::getpvLowLoss()
{
	return pvLowLoss;
}
int CCComm::getpvHighDerating()
{
	return pvHighDerating;
}
int CCComm::gettempHighDerating()
{
	return tempHighDerating;
}
int CCComm::getbattTempLowAlarm()
{
	return battTempLowAlarm;
}
int CCComm::getbattLowWarning()
{
	return battLowWarning;
}
int CCComm::getbattEqualizedEn()
{
	return battEqualizedEn;
}
int CCComm::getbattEqualizedTime()
{
	return battEqualizedTime;
}
int CCComm::getintervalTime()
{
	return intervalTime;
}
int CCComm::getmaxCurrent()
{
	return maxCurrent;
}
int CCComm::getremainingTime()
{
	return remainingTime;
}
float CCComm::getbattEqualizedVoltage()
{
	return battEqualizedVoltage;
}
int CCComm::getbattCVChargeTime()
{
	return battCVChargeTime;
}
int CCComm::getbattEqualizedTimeout()
{
	return battEqualizedTimeout;
}

/************************Set functions***************************/

//Set the battery type
/*
	PBT<TT><cr>: Setting battery type
	Computer: PBT<TT ><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	Set device working range in line mode, 00 for AGM, 01 for Flooded battery
	
	TT(Battery Type)	|	AA.A(Absorption)	|	FF.F(Floating)
	--------------------------------------------------------------
						|	12V 24V 48V 		|	12V 24V 48V
	00 AGM 				|	14.1 28.2 56.4 		|	13.5 27.0 54.0
	01 Flooded 			|	14.6 29.2 58.4 		|	13.5 27.0 54.0
	02 Customized 		|						|
*/
void CCComm::setBattType(int battType)
{
	if(battType == AGM)		//Set to AGM
	{
		s_setBattType[3] = '0';
		s_setBattType[4] = '0';
		CRCcalc(s_setBattType, LEN5);
	}
	else if(battType == FLOODED)	//Set to Flooded
	{
		s_setBattType[3] = '0';
		s_setBattType[4] = '1';
		CRCcalc(s_setBattType, LEN5);
	}
	else					//Set to customized
	{
		s_setBattType[3] = '0';
		s_setBattType[4] = '2';
		CRCcalc(s_setBattType, LEN5);
	}
}

//Set the battery absorbtion charging voltage
/*
	Computer: PBAV<AA.AA ><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	AA.AA - C.V voltage per cell
	
	Battery Type 	|	AA.AA(Absorption)
					|	12V/24V/48V
	-------------------------------------
	Customized 		|	aa.aa
	-------------------------------------

	aa.aa - Voltage set by user (12.00V~15.00V), active on customized battery type.
*/
void CCComm::setBattAbsorbtionChargingVoltage(float chargeVolt)
{
	char chargeVolt_p[5];

	//Convert the float to a character pointer
	sprintf(chargeVolt_p, "%f", chargeVolt);

	//Update the array with the new voltage value
	s_setBattAbsorbtionChargingVoltage[4] = chargeVolt_p[0];
	s_setBattAbsorbtionChargingVoltage[5] = chargeVolt_p[1];
	s_setBattAbsorbtionChargingVoltage[6] = chargeVolt_p[2];
	s_setBattAbsorbtionChargingVoltage[7] = chargeVolt_p[3];
	s_setBattAbsorbtionChargingVoltage[8] = chargeVolt_p[4];
	
	CRCcalc(s_setBattAbsorbtionChargingVoltage, LEN9);

	//for(int i=0; i<LEN9; i++)
	//	printf("%c", s_setBattAbsorbtionChargingVoltage[i]);
}

//Set the battery floating charging voltage
/*
	PBFV< FF.FF><cr>: Setting battery floating charging voltage
	Computer: PBFV<FF.FF><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	FF.FF - Floating voltage per cell
	
	Battery Type 	|	FF.FF(Floating)
					|	12V/24V/48V
	-----------------------------------
	Customized 		|	ff.ff
	-----------------------------------

	ff.ff - Voltage set by user(12.00V~15.00V) , activated on customized battery type.
*/
void CCComm::setBattFloatingChargingVoltage(float floatVolt)
{
	char floatVolt_p[5];

	//Convert the float to a character pointer
	sprintf(floatVolt_p, "%f", floatVolt);

	//Update the array with the new voltage value
	s_setBattFloatingChargingVoltage[4] = floatVolt_p[0];
	s_setBattFloatingChargingVoltage[5] = floatVolt_p[1];
	s_setBattFloatingChargingVoltage[6] = floatVolt_p[2];
	s_setBattFloatingChargingVoltage[7] = floatVolt_p[3];
	s_setBattFloatingChargingVoltage[8] = floatVolt_p[4];
	
	CRCcalc(s_setBattFloatingChargingVoltage, LEN9);

	//for(int i=0; i<LEN9; i++)
	//	printf("%c", s_setBattFloatingChargingVoltage[i]);
}

//Set the rated battery voltage
/*
	PBRV<NN><cr>: Setting rated battery voltage
	Computer: PBRV<NN><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	00: Enable battery voltage auto sensing
	01: Set rated battery voltage 12V
	02: Set rated battery voltage 24V
	03: Set rated battery voltage 36V (Reserved)
	04: Set rated battery voltage 48V
*/
void CCComm::setRatedBattVoltage(int battV)
{
	char const *battV_p;
	std::string volt;

	volt = std::to_string(battV);
	battV_p = volt.c_str();

	//Update the array with the new voltage value
	s_setRatedBattVoltage[4] = '0';
	s_setRatedBattVoltage[5] = battV_p[0];
	
	CRCcalc(s_setRatedBattVoltage, LEN6);

	//for(int i=0; i<LEN6; i++)
	//	printf("%c", s_setRatedBattVoltage[i]);
}

//set the max charging current
/*
	MCHGC<NNN><cr>: Setting max charging current
	Computer: MCHGC<NNN><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	NNN is from 010 ~ 060 for MPPT-3000-Standard, unit is A.
*/
void CCComm::setMaxChargingCurrent(int chargeCurr)
{
	char const *chargeCurr_p;
	std::string curr;

	curr = std::to_string(chargeCurr);
	chargeCurr_p = curr.c_str();

	//Update the array with the new current value
	s_setMaxChargingCurrent[5] = '0';
	s_setMaxChargingCurrent[6] = chargeCurr_p[0];
	s_setMaxChargingCurrent[7] = chargeCurr_p[1];
	
	CRCcalc(s_setMaxChargingCurrent, LEN8);

	//for(int i=0; i<LEN8; i++)
	//	printf("%c", s_setMaxChargingCurrent[i]);
}

//set the remote battery voltage detect enable/disable
/*
	PRBD<NN><cr>: Enable/disable remote battery voltage detect
	Computer: PRBD<NN><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	00 - Disable remote battery voltage detect
	01 - Enable remote battery voltage detect
*/
void CCComm::setEnRemoteBatteryVoltageDetect(int endis)
{
	if(endis == DISABLE)	//Disable
	{
		//Update the array with the new value
		s_enRemoteBatteryVoltageDetect[4] = '0';
		s_enRemoteBatteryVoltageDetect[5] = '0';	
	}
	else			//Enable
	{
		//Update the array with the new value
		s_enRemoteBatteryVoltageDetect[4] = '0';
		s_enRemoteBatteryVoltageDetect[5] = '1';	
	}
		
	CRCcalc(s_enRemoteBatteryVoltageDetect, LEN6);

	//for(int i=0; i<LEN6; i++)
	//	printf("%c", s_enRemoteBatteryVoltageDetect[i]);
}

//set the battery low warning voltage
/*
	PBLV<nn.nn><cr>: Set battery low warning voltage
	Computer: PBLV< nn.nn ><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	nn.nn 10.00~12.50V
*/
void CCComm::setBattLowWarningVoltage(float lowVolt)
{
	char lowVolt_p[5];

	//Convert the float to a character pointer
	sprintf(lowVolt_p, "%f", lowVolt);

	//Update the array with the new voltage value
	s_setBattLowWarningVoltage[4] = lowVolt_p[0];
	s_setBattLowWarningVoltage[5] = lowVolt_p[1];
	s_setBattLowWarningVoltage[6] = lowVolt_p[2];
	s_setBattLowWarningVoltage[7] = lowVolt_p[3];
	s_setBattLowWarningVoltage[8] = lowVolt_p[4];
	CRCcalc(s_setBattLowWarningVoltage, LEN9);

	//for(int i=0; i<LEN9; i++)
	//	printf("%c", s_setBattLowWarningVoltage[i]);
}

//set the battery low shutdown detect enable/disable
/*
	PBLSEn<cr>: Set battery low shutdown detect enable/disable
	Computer: PBLSEn <CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	n: 0 means disable, 1 means enable
*/
void CCComm::setBattLowShutdownDetectEn(int endis)
{
	if(endis == DISABLE)	//Disable
	{
		//Update the array with the new value
		s_setBattLowShutdownDetectEn[5] = '0';	
	}
	else			//Enable
	{
		//Update the array with the new value
		s_setBattLowShutdownDetectEn[5] = '1';	
	}
		
	CRCcalc(s_setBattLowShutdownDetectEn, LEN6);

	//for(int i=0; i<LEN6; i++)
	//	printf("%c", s_setBattLowShutdownDetectEn[i]);
}

//set the battery equalization enable/disable
/*
	PBEQEn<cr>: Set battery equalization enable/disable.
	Computer:PBEQEn<CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	n: 0 means disable, 1 means enable
*/
void CCComm::setBattEqualizationEn(int endis)
{
	if(endis == DISABLE)	//Disable
	{
		//Update the array with the new value
		s_setBattEqualizationEn[5] = '0';	
	}
	else			//Enable
	{
		//Update the array with the new value
		s_setBattEqualizationEn[5] = '1';	
	}
		
	CRCcalc(s_setBattEqualizationEn, LEN6);

	//for(int i=0; i<LEN6; i++)
	//	printf("%c", s_setBattEqualizationEn[i]);
}	

//Set the battery equalized time			
/*
	PBEQT<nnn><cr>: Set battery equalized time.
	Computer:PBEQT<nnn><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	nnn:5~300, the unit is minute.
*/
void CCComm::setBattEqualizedTime(int eqTime)
{
	char const *eqTime_p;
	std::string etime;

	etime = std::to_string(eqTime);
	eqTime_p = etime.c_str();

	if(eqTime > 99)
	{
		//Update the array with the new current value
		s_setBattEqualizedTime[5] = eqTime_p[0];
		s_setBattEqualizedTime[6] = eqTime_p[1];
		s_setBattEqualizedTime[7] = eqTime_p[2];
	}
	else if(eqTime > 9)
	{
		//Update the array with the new current value
		s_setBattEqualizedTime[5] = '0';
		s_setBattEqualizedTime[6] = eqTime_p[0];
		s_setBattEqualizedTime[7] = eqTime_p[1];
	}
	else
	{
		//Update the array with the new current value
		s_setBattEqualizedTime[5] = '0';
		s_setBattEqualizedTime[6] = '0';
		s_setBattEqualizedTime[7] = eqTime_p[0];
	}
	
	CRCcalc(s_setBattEqualizedTime, LEN8);

	//for(int i=0; i<LEN8; i++)
	//	printf("%c", s_setBattEqualizedTime[i]);
}	

//Set the period of the battery equalization	
/*
	PBEQP<nnn><cr>: Set the period of battery equalization.
	Computer:PBEQP<nnn><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	nnn:0~060, the unit is days. 0 means battery equalization function only activate by key.
*/		
void CCComm::setPeriodBattEqualization(int period)
{
	char const *period_p;
	std::string period_t;

	period_t = std::to_string(period);
	period_p = period_t.c_str();

	if(period >= TRIPDIGIT)
	{
		//Update the array with the new current value
		s_setBattEqualizedTime[5] = period_p[0];
		s_setBattEqualizedTime[6] = period_p[1];
		s_setBattEqualizedTime[7] = period_p[2];
	}
	else if(period >= DOUBDIGIT)
	{
		//Update the array with the new current value
		s_setBattEqualizedTime[5] = '0';
		s_setBattEqualizedTime[6] = period_p[0];
		s_setBattEqualizedTime[7] = period_p[1];
	}
	else
	{
		//Update the array with the new current value
		s_setBattEqualizedTime[5] = '0';
		s_setBattEqualizedTime[6] = '0';
		s_setBattEqualizedTime[7] = period_p[0];
	}
	
	CRCcalc(s_setBattEqualizedTime, LEN8);

	//for(int i=0; i<LEN8; i++)
	//	printf("%c", s_setBattEqualizedTime[i]);
}		

//Set the max current of the battery equalization	
/*
	PBEQMC<nnn><cr>: Set the max current of battery equalization.
	Computer:PBEQMC<nnn><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	nnn:005~060, the unit is A. this value must less than max charged current.
*/
void CCComm::setMaxCurrentBatteryEqualization(int maxCurr)
{
	char const *maxCurr_p;
	std::string maxCurr_t;

	maxCurr_t = std::to_string(maxCurr);
	maxCurr_p = maxCurr_t.c_str();

	if(maxCurr >= TRIPDIGIT)
	{
		//Update the array with the new current value
		s_setMaxCurrentBatteryEqualization[6] = maxCurr_p[0];
		s_setMaxCurrentBatteryEqualization[7] = maxCurr_p[1];
		s_setMaxCurrentBatteryEqualization[8] = maxCurr_p[2];
	}
	else if(maxCurr >= DOUBDIGIT)
	{
		//Update the array with the new current value
		s_setMaxCurrentBatteryEqualization[6] = '0';
		s_setMaxCurrentBatteryEqualization[7] = maxCurr_p[0];
		s_setMaxCurrentBatteryEqualization[8] = maxCurr_p[1];
	}
	else
	{
		//Update the array with the new current value
		s_setMaxCurrentBatteryEqualization[6] = '0';
		s_setMaxCurrentBatteryEqualization[7] = '0';
		s_setMaxCurrentBatteryEqualization[8] = maxCurr_p[0];
	}
	
	CRCcalc(s_setMaxCurrentBatteryEqualization, LEN9);

	//for(int i=0; i<LEN9; i++)
	//	printf("%c", s_setMaxCurrentBatteryEqualization[i]);
}	

//set the battery equalized voltage
/*
	PBEQV<nn.nn><cr>: Set battery equalized voltage.
	Computer:PBEQV<nn.nn><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	nn.n:12.00~15.50, the unit is V. this value must more than bulk voltage.
*/
void CCComm::setBattEqualizedVoltage(float equalVolt)
{
	char equalVolt_p[5];

	//Convert the float to a character pointer
	sprintf(equalVolt_p, "%f", equalVolt);

	//Update the array with the new voltage value
	s_setBattEqualizedVoltage[5] = equalVolt_p[0];
	s_setBattEqualizedVoltage[6] = equalVolt_p[1];
	s_setBattEqualizedVoltage[7] = equalVolt_p[2];
	s_setBattEqualizedVoltage[8] = equalVolt_p[3];
	s_setBattEqualizedVoltage[9] = equalVolt_p[4];
	CRCcalc(s_setBattEqualizedVoltage, LEN10);

	//for(int i=0; i<LEN10; i++)
	//	printf("%c", s_setBattEqualizedVoltage[i]);
}			

//set the battery CV charge time
/*
	PBCVT<nnn><cr>: Set battery C.V. charge time.
	Computer:PBCVT<nnn><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	nnn:5~300, the unit is minute.
*/
void CCComm::setBattCVChargeTime(int cvtime)
{
	char const *cvtime_p;
	std::string cvtime_t;

	cvtime_t = std::to_string(cvtime);
	cvtime_p = cvtime_t.c_str();

	if(cvtime >= TRIPDIGIT)
	{
		//Update the array with the new current value
		s_setBattCVChargeTime[5] = cvtime_p[0];
		s_setBattCVChargeTime[6] = cvtime_p[1];
		s_setBattCVChargeTime[7] = cvtime_p[2];
	}
	else if(cvtime >= DOUBDIGIT)
	{
		//Update the array with the new current value
		s_setBattCVChargeTime[5] = '0';
		s_setBattCVChargeTime[6] = cvtime_p[0];
		s_setBattCVChargeTime[7] = cvtime_p[1];
	}
	else
	{
		//Update the array with the new current value
		s_setBattCVChargeTime[5] = '0';
		s_setBattCVChargeTime[6] = '0';
		s_setBattCVChargeTime[7] = cvtime_p[0];
	}
	
	CRCcalc(s_setBattCVChargeTime, LEN8);

	//for(int i=0; i<LEN8; i++)
	//	printf("%c", s_setBattCVChargeTime[i]);
}		

//set the time of the battery equalized timeout	
/*
	PBEQOT<nnn><cr>: Set the time of battery equalized timeout.
	Computer:PBEQOT<nnn><CRC><cr>
	Device: (ACK<CRC><cr> if device accepted, or respond (NAK<CRC><cr>
	nnn:5~360, the unit is minute.
*/	
void CCComm::setTimeBatteryEqualizedTimeout(int timeout)
{
	char const *timeout_p;
	std::string timeout_t;

	timeout_t = std::to_string(timeout);
	timeout_p = timeout_t.c_str();

	if(timeout >= TRIPDIGIT)
	{
		//Update the array with the new current value
		s_setTimeBatteryEqualizedTimeout[6] = timeout_p[0];
		s_setTimeBatteryEqualizedTimeout[7] = timeout_p[1];
		s_setTimeBatteryEqualizedTimeout[8] = timeout_p[2];
	}
	else if(timeout >= DOUBDIGIT)
	{
		//Update the array with the new current value
		s_setTimeBatteryEqualizedTimeout[6] = '0';
		s_setTimeBatteryEqualizedTimeout[7] = timeout_p[0];
		s_setTimeBatteryEqualizedTimeout[8] = timeout_p[1];
	}
	else
	{
		//Update the array with the new current value
		s_setTimeBatteryEqualizedTimeout[6] = '0';
		s_setTimeBatteryEqualizedTimeout[7] = '0';
		s_setTimeBatteryEqualizedTimeout[8] = timeout_p[0];
	}
	
	CRCcalc(s_setTimeBatteryEqualizedTimeout, LEN9);

	//for(int i=0; i<LEN9; i++)
	//	printf("%c", s_setTimeBatteryEqualizedTimeout[i]);
}	