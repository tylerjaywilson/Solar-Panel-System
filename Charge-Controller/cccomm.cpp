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
#include <ctime>
#include <unistd.h>			//Used for UART
#include <fcntl.h>			//Used for UART
#include <termios.h>		//Used for UART
#include "uart.hpp"
#include "i2c.hpp"
#include "cccomm.hpp"


#define RX_LENGTH_MAX 256	//The max receive buffer size is set to 256
#define IDLENGTH 14			//The device ID length is expected to be 14
#define INT_CONVERT 48		//Converts character integers to actual integer values

/* These #define's indicate the number of bytes within any given character array */
#define NUM_BYTES2 2
#define NUM_BYTES3 3
#define NUM_BYTES5 5
#define NUM_BYTES6 6
#define NUM_BYTES8 8
#define NUM_BYTES9 9
#define NUM_BYTES10 10
#define NUM_BYTES11 11
#define NUM_BYTES12 12
#define NUM_BYTES13 13

//Battery Types
#define AGM 0
#define FLOODED 1
#define CUSTOM 2

//Enable or disable different parameters
#define ENABLE 1
#define DISABLE 0

//Number of digits in an integer
#define TRIPDIGIT 100
#define DOUBDIGIT 10

//Used for converting characters to integers
#define INTEGER_BASE 10

//Used for timing - Updating the charge controller parameters
#define ONESECOND 1.0

//Lengths of received arrays after sending an inquiry
#define RX_BUFF_MAX 255
#define QID_LEN 18
#define QPIRI_LEN 56
#define QPIGS_LEN 68
#define QDI_LEN 40
#define QPIWS_LEN 34
#define QBEQI_LEN 35
#define ACKNACK_LEN 7

/* What kind of read or write is occuring */
#define SERIAL_NUM 1
#define RATED_INFO 2
#define GENERAL_STATUS 3
#define DEFAULT_SETTING 4
#define WARNING_STATUS 5
#define EQUALIZED_INFO 6
#define BATT_TYPE 7
#define ABSORB_CHARGE_VOLT 8
#define FLOAT_CHARGE_VOLT 9
#define RATED_BATT_VOLT 10
#define MAX_CHARGE_CURRENT 11
#define BATT_VOLT_DETECT 12
#define LOW_WARN_VOLT 13
#define LOW_SHUTDOWN_DETECT_EN 14
#define EQUALIZATAION_EN 15
#define EQUALIZED_TIME 16
#define PERIOD_EQUALIZATION 17
#define MAX_CURRENT_EQUALIZATION 18
#define EQUALIZED_VOLT 19
#define CV_CHARGE_TIME 20
#define EQUALIZED_TIMEOUT 21
#define DEFAULTS 22
#define ACKNACK 23

//Default Constructor
CCComm::CCComm()
{
	/********UART Inititialization***********/
	/* UART Stuff */
  	uart0.setFilestream(-1);       	//UART filestream for UART communication
  	uart0.setBaud(2400);       		//Default 2400 value;
	uart0.setFilestream(uart0.init(uart0.getFilestream(), uart0.getBaud()));   //Initialize the UART

	/*****************SEND VARIABLES*****************/

	/*******Inquiry parameters*****/
	//QID
	s_deviceSerialNum[0] = 'Q';
	s_deviceSerialNum[1] = 'I';
	s_deviceSerialNum[2] = 'D';
	CRCcalc(s_deviceSerialNum, NUM_BYTES3);			//Calculate the CRC and append the CRC and carriage return to the array
	
	//QPIRI
	s_deviceRatedInfo[0] = 'Q';
	s_deviceRatedInfo[1] = 'P';
	s_deviceRatedInfo[2] = 'I';
	s_deviceRatedInfo[3] = 'R';
	s_deviceRatedInfo[4] = 'I';
	CRCcalc(s_deviceRatedInfo, NUM_BYTES5);			//Calculate the CRC and append the CRC and carriage return to the array

	//QPIGS
	s_deviceGeneralStatusInfo[0] = 'Q';
	s_deviceGeneralStatusInfo[1] = 'P';
	s_deviceGeneralStatusInfo[2] = 'I';
	s_deviceGeneralStatusInfo[3] = 'G';
	s_deviceGeneralStatusInfo[4] = 'S';
	CRCcalc(s_deviceGeneralStatusInfo, NUM_BYTES5);	//Calculate the CRC and append the CRC and carriage return to the array
	
	//QDI
	s_defaultSettingValueInfo[0] = 'Q';
	s_defaultSettingValueInfo[1] = 'D';
	s_defaultSettingValueInfo[2] = 'I';
	CRCcalc(s_defaultSettingValueInfo, NUM_BYTES3);	//Calculate the CRC and append the CRC and carriage return to the array

	//QPIWS
	s_deviceWarningStatus[0] = 'Q';
	s_deviceWarningStatus[1] = 'P';
	s_deviceWarningStatus[2] = 'I';
	s_deviceWarningStatus[3] = 'W';
	s_deviceWarningStatus[4] = 'S';
	CRCcalc(s_deviceWarningStatus, NUM_BYTES5);		//Calculate the CRC and append the CRC and carriage return to the array

	//QBEQI
	s_batteryEqualizedInfo[0] = 'Q';
	s_batteryEqualizedInfo[1] = 'B';
	s_batteryEqualizedInfo[2] = 'E';
	s_batteryEqualizedInfo[3] = 'Q';
	s_batteryEqualizedInfo[4] = 'I';
	CRCcalc(s_batteryEqualizedInfo, NUM_BYTES5);	//Calculate the CRC and append the CRC and carriage return to the array
	/******END OF INQUIRY PARAMETERS************/

	/*******Setting Parameters********/
	//PBT
	s_setBattType[0] = 'P';		//PBT setting array
	s_setBattType[1] = 'B';
	s_setBattType[2] = 'T';
	s_setBattType[3] = '0';		//Default battery type - 00 - AGM
	s_setBattType[4] = '0';	
	CRCcalc(s_setBattType, NUM_BYTES5);				//Calculate the CRC and append the CRC and carriage return to the array

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
	CRCcalc(s_setBattAbsorbtionChargingVoltage, NUM_BYTES9);	//Calculate the CRC and append the CRC and carriage return to the array

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
	CRCcalc(s_setBattFloatingChargingVoltage, NUM_BYTES9);	//Calculate CRC and append with carriage return

	//PBRV
	s_setRatedBattVoltage[0] = 'P';		//PBRV setting array
	s_setRatedBattVoltage[1] = 'B';
	s_setRatedBattVoltage[2] = 'R';
	s_setRatedBattVoltage[3] = 'V';
	s_setRatedBattVoltage[4] = '0';		//Set the default to auto-sensing - 00
	s_setRatedBattVoltage[5] = '0';
	CRCcalc(s_setRatedBattVoltage, NUM_BYTES6);		//Append CRC

	//MCHGC
	s_setMaxChargingCurrent[0] = 'M';	//MCHGC setting array
	s_setMaxChargingCurrent[1] = 'C';
	s_setMaxChargingCurrent[2] = 'H';
	s_setMaxChargingCurrent[3] = 'G';
	s_setMaxChargingCurrent[4] = 'C';
	s_setMaxChargingCurrent[5] = '0';	//Set the default to 60 A
	s_setMaxChargingCurrent[6] = '6';
	s_setMaxChargingCurrent[7] = '0';
	CRCcalc(s_setMaxChargingCurrent, NUM_BYTES8);	//Append CRC

	//PRBD
	s_enRemoteBatteryVoltageDetect[0] = 'P';		//PRBD setting array
	s_enRemoteBatteryVoltageDetect[1] = 'R';
	s_enRemoteBatteryVoltageDetect[2] = 'B';
	s_enRemoteBatteryVoltageDetect[3] = 'D';
	s_enRemoteBatteryVoltageDetect[4] = '0';		//Set the default to disabled for the remote batt voltage detect
	s_enRemoteBatteryVoltageDetect[5] = '0';
	CRCcalc(s_enRemoteBatteryVoltageDetect, NUM_BYTES6);	//Append CRC

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
	CRCcalc(s_setBattLowWarningVoltage, NUM_BYTES9);		//Append CRC

	//PBLSE
	s_setBattLowShutdownDetectEn[0] = 'P';			//PBLSE setting array
	s_setBattLowShutdownDetectEn[1] = 'B';
	s_setBattLowShutdownDetectEn[2] = 'L';
	s_setBattLowShutdownDetectEn[3] = 'S';
	s_setBattLowShutdownDetectEn[4] = 'E';
	s_setBattLowShutdownDetectEn[5] = '1';			//Set the default to enabled for low battery shutdown detect
	CRCcalc(s_setBattLowShutdownDetectEn, NUM_BYTES6);	//Append CRC

	//PBEQE
	s_setBattEqualizationEn[0] = 'P';		//PBEQE setting array
	s_setBattEqualizationEn[1] = 'B';
	s_setBattEqualizationEn[2] = 'E';
	s_setBattEqualizationEn[3] = 'Q';
	s_setBattEqualizationEn[4] = 'E';
	s_setBattEqualizationEn[5] = '0';		//Set the default to disabled for the battery equalization
	CRCcalc(s_setBattEqualizationEn, NUM_BYTES6);		//Append CRC

	//PBEQT
	s_setBattEqualizedTime[0] = 'P';		//PBEQT setting array
	s_setBattEqualizedTime[1] = 'B';
	s_setBattEqualizedTime[2] = 'E';
	s_setBattEqualizedTime[3] = 'Q';
	s_setBattEqualizedTime[4] = 'T';
	s_setBattEqualizedTime[5] = '0';		//Set the default to 60 minutes for the battery equalized time
	s_setBattEqualizedTime[6] = '6';
	s_setBattEqualizedTime[7] = '0';
	CRCcalc(s_setBattEqualizedTime, NUM_BYTES8);	//Append CRC

	//PBEQP
	s_setPeriodBattEqualization[0] = 'P';			//PBEQP setting array
	s_setPeriodBattEqualization[1] = 'B';
	s_setPeriodBattEqualization[2] = 'E';
	s_setPeriodBattEqualization[3] = 'Q';
	s_setPeriodBattEqualization[4] = 'P';
	s_setPeriodBattEqualization[5] = '0';		//Default period of battery equalization is 30 days
	s_setPeriodBattEqualization[6] = '3';
	s_setPeriodBattEqualization[7] = '0';
	CRCcalc(s_setPeriodBattEqualization, NUM_BYTES8);	//Append CRC

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
	CRCcalc(s_setMaxCurrentBatteryEqualization, NUM_BYTES9);	//Append CRC

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
	CRCcalc(s_setBattEqualizedVoltage, NUM_BYTES10);	//Append CRC

	//PBCVT
	s_setBattCVChargeTime[0] = 'P';				//PBCVT setting array
	s_setBattCVChargeTime[1] = 'B';
	s_setBattCVChargeTime[2] = 'C';
	s_setBattCVChargeTime[3] = 'V';
	s_setBattCVChargeTime[4] = 'T';
	s_setBattCVChargeTime[5] = '1';				//Default charge time set to 150 minutes
	s_setBattCVChargeTime[6] = '5';
	s_setBattCVChargeTime[7] = '0';
	CRCcalc(s_setBattCVChargeTime, NUM_BYTES8);	//Append CRC

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
	CRCcalc(s_setTimeBatteryEqualizedTimeout, NUM_BYTES9);	//Apend CRC

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
	CRCcalc(s_setControlParameterDefault, NUM_BYTES2);	//Append CRC	
	

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

	/* QDI - Default Setting Value information */
  	// The 'd' is to indicate 'default'
	d_battRatedVoltageSet = -1;
   	d_maxChargingCurrent = -1.0;
    d_battType = -1;
   	d_absorbtionVoltage = -1.0;
   	d_floatingVoltage = -1.0;
    d_remoteBattVoltageDetect = -1;
   	d_tempCompensationRatio = -1.0;
    d_reserved = -1;

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

	//Timeout variable 
	startTime = clock();
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

/*
	Parse the default setting value information

	QDI<cr>: The default setting value information
	Computer: QDI<CRC><cr>
	Device: (BB CC.C DD EE.EE FF.FF GG ±HH.H IIII<CRC><cr>
*/
void CCComm::parseQDI(unsigned char *rx_buffer_t)
{
	char *rx_buff_p = (strtok((char *) (rx_buffer_t+1), " ")); //rx_buffer_t+1 is used to remove the beginning '(' of the received data
	d_battRatedVoltageSet = atoi(rx_buff_p);
	//printf("\nPV Voltage: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	d_maxChargingCurrent = atof(rx_buff_p);
	//printf("\nBatt Voltage: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	d_battType = atoi(rx_buff_p);
	//printf("\nCharging i: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	d_absorbtionVoltage = atof(rx_buff_p);
	//printf("\nCharging1 i: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	d_floatingVoltage = atof(rx_buff_p);
	//printf("\nCharging2 i: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	d_remoteBattVoltageDetect = atoi(rx_buff_p);
	//printf("\nCharging Power: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	d_tempCompensationRatio = atof(rx_buff_p);
	//printf("\nUnit Temp: %s\n", rx_buff_p);

	rx_buff_p = strtok(NULL, " ");
	d_reserved = atoi(rx_buff_p);
	//printf("\nRemote Batt V: %s\n", rx_buff_p);

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
bool CCComm::parseACKNACK(unsigned char *rx_buffer_t)
{
	//Check to see if an ACKNOWLEDGED was received
	if((rx_buffer[1] == 'A') && (rx_buffer_t[2] == 'C') && (rx_buffer_t[3] == 'K'))
		return true;
	else	//A NOT ACKNOWLEDGED was received
		return false;
}

// Determine if the data is older than one second and return true if an update is needed
bool CCComm::updateParameters()
{
	float elapsedTime = 0;
	float starttime = (float) getStartTime();
	float curtime = (float) clock();

	//printf("Start Time: %f\n", starttime);
	//printf("Current Time: %f\n", curtime);

	//Calculate the elapsed time
	elapsedTime = curtime-starttime;
	//printf("Elapsed Time: %f\n", (elapsedTime/CLOCKS_PER_SEC));

	//See if more than one second has elapsed
	if( (elapsedTime/CLOCKS_PER_SEC) > ONESECOND )
	{
		setStartTime(clock());	//Update the new start time
		return true;
	}
	else
		return false;
}

//UART Write function - Write data to the charge controller based on the write type (what kind of request)
//The function determines what kind of request or setting of parameters is occurring
void CCComm::uartWrite(int writeType)
{
	int tx_count = 0;	//Need to use this for error detection
	switch(writeType)
	{
		case SERIAL_NUM:
			tx_count = write(uart0.getFilestream(), &s_deviceSerialNum[0], NUM_BYTES6);	//write(filestream, starting address of inquiry, number of bytes to transmit)
			break;
		case RATED_INFO:
			tx_count = write(uart0.getFilestream(), &s_deviceRatedInfo[0], NUM_BYTES8);
			break;
		case GENERAL_STATUS:
			tx_count = write(uart0.getFilestream(), &s_deviceGeneralStatusInfo[0], NUM_BYTES8);
			break;
		case WARNING_STATUS:
			tx_count = write(uart0.getFilestream(), &s_deviceWarningStatus[0], NUM_BYTES8);
			break;
		case EQUALIZED_INFO:
			tx_count = write(uart0.getFilestream(), &s_batteryEqualizedInfo[0], NUM_BYTES8);
			break;
		case BATT_TYPE:
			tx_count = write(uart0.getFilestream(), &s_setBattType[0], NUM_BYTES8);
			break;
		case ABSORB_CHARGE_VOLT:
			tx_count = write(uart0.getFilestream(), &s_setBattAbsorbtionChargingVoltage[0], NUM_BYTES12);
			break;
		case FLOAT_CHARGE_VOLT:
			tx_count = write(uart0.getFilestream(), &s_setBattFloatingChargingVoltage[0], NUM_BYTES12);
			break;
		case RATED_BATT_VOLT:
			tx_count = write(uart0.getFilestream(), &s_setRatedBattVoltage[0], NUM_BYTES9);
			break;
		case MAX_CHARGE_CURRENT:
			tx_count = write(uart0.getFilestream(), &s_setMaxChargingCurrent[0], NUM_BYTES11);
			break;
		case BATT_VOLT_DETECT:
			tx_count = write(uart0.getFilestream(), &s_enRemoteBatteryVoltageDetect[0], NUM_BYTES9);
			break;
		case LOW_WARN_VOLT:
			tx_count = write(uart0.getFilestream(), &s_setBattLowWarningVoltage[0], NUM_BYTES12);
			break;
		case LOW_SHUTDOWN_DETECT_EN:
			tx_count = write(uart0.getFilestream(), &s_setBattLowShutdownDetectEn[0], NUM_BYTES9);
			break;
		case EQUALIZATAION_EN:
			tx_count = write(uart0.getFilestream(), &s_setBattEqualizationEn[0], NUM_BYTES9);
			break;
		case EQUALIZED_TIME:
			tx_count = write(uart0.getFilestream(), &s_setBattEqualizedTime[0], NUM_BYTES11);
			break;
		case PERIOD_EQUALIZATION:
			tx_count = write(uart0.getFilestream(), &s_setPeriodBattEqualization[0], NUM_BYTES11);
			break;
		case MAX_CURRENT_EQUALIZATION:
			tx_count = write(uart0.getFilestream(), &s_setMaxCurrentBatteryEqualization[0], NUM_BYTES12);
			break;
		case EQUALIZED_VOLT:
			tx_count = write(uart0.getFilestream(), &s_setBattEqualizedVoltage[0], NUM_BYTES13);
			break;
		case CV_CHARGE_TIME:
			tx_count = write(uart0.getFilestream(), &s_setBattCVChargeTime[0], NUM_BYTES11);
			break;
		case EQUALIZED_TIMEOUT:
			tx_count = write(uart0.getFilestream(), &s_setTimeBatteryEqualizedTimeout[0], NUM_BYTES12);
			break;
		case DEFAULTS:
			tx_count = write(uart0.getFilestream(), &s_setControlParameterDefault[0], NUM_BYTES5);
			break;
		default:
			printf("Error: Write Type!\n");
	}
}

//UART Charge Controller Read function
//Depending on which type of read is occuring, continue to read the available data until the 
//expected length of the response is met
void CCComm::uartRead(int readType)
{
	int rx_length, tot_length = 0;	//Need to use this for error detection
	bool finishedReading = false;

	//Continue reading incoming data until the expected limit is reached
	while(!finishedReading)
	{
		//Which type of read are we expecting to have
		switch(readType)
		{
			case SERIAL_NUM:				
				//Determine the rx_length of incoming data (total number of bytes)
				rx_length = read(uart0.getFilestream(), (void*)(rx_buffer+tot_length), RX_BUFF_MAX);	//read(filestream, storage buffer, number of bytes to read (max))
				
				//Add the rx_length to the total accumulated length.
				tot_length += rx_length;
				
				//If the tot_length == the expected length then parse the data
				if(tot_length == QID_LEN)			//The rx_length expectation (18) is hard-coded based on the expected length of the incoming data
				{
					finishedReading = true;
					//Parse the data
					parseQID(rx_buffer);
				}
				break;
			case RATED_INFO:
				//Determine the rx_length of incoming data (total number of bytes)
				rx_length = read(uart0.getFilestream(), (void*)(rx_buffer+tot_length), RX_BUFF_MAX);	//read(filestream, storage buffer, number of bytes to read (max))
				
				//Add the rx_length to the total accumulated length.
				tot_length += rx_length;
				
				//If the tot_length == the expected length then parse the data
				if(tot_length == QPIRI_LEN)			//The rx_length expectation (18) is hard-coded based on the expected length of the incoming data
				{
					finishedReading = true;
					//Parse the data
					parseQPIRI(rx_buffer);
				}
				break;
			case GENERAL_STATUS:
				//Determine the rx_length of incoming data (total number of bytes)
				rx_length = read(uart0.getFilestream(), (void*)(rx_buffer+tot_length), RX_BUFF_MAX);	//read(filestream, storage buffer, number of bytes to read (max))
				
				//Add the rx_length to the total accumulated length.
				tot_length += rx_length;
				
				//If the tot_length == the expected length then parse the data
				if(tot_length == QPIGS_LEN)			//The rx_length expectation (18) is hard-coded based on the expected length of the incoming data
				{
					finishedReading = true;
					//Parse the data
					parseQPIGS(rx_buffer);
				}
				break;
			case WARNING_STATUS:
				//Determine the rx_length of incoming data (total number of bytes)
				rx_length = read(uart0.getFilestream(), (void*)(rx_buffer+tot_length), RX_BUFF_MAX);	//read(filestream, storage buffer, number of bytes to read (max))
				
				//Add the rx_length to the total accumulated length.
				tot_length += rx_length;
				
				//If the tot_length == the expected length then parse the data
				if(tot_length == QPIWS_LEN)			//The rx_length expectation (18) is hard-coded based on the expected length of the incoming data
				{
					finishedReading = true;
					//Parse the data
					parseQPIWS(rx_buffer);
				}
				break;
			case EQUALIZED_INFO:
				//Determine the rx_length of incoming data (total number of bytes)
				rx_length = read(uart0.getFilestream(), (void*)(rx_buffer+tot_length), RX_BUFF_MAX);	//read(filestream, storage buffer, number of bytes to read (max))
				
				//Add the rx_length to the total accumulated length.
				tot_length += rx_length;
				
				//If the tot_length == the expected length then parse the data
				if(tot_length == QBEQI_LEN)			//The rx_length expectation (18) is hard-coded based on the expected length of the incoming data
				{
					finishedReading = true;
					//Parse the data
					parseQBEQI(rx_buffer);
				}
				break;
			case ACKNACK:
				//Determine the rx_length of incoming data (total number of bytes)
				rx_length = read(uart0.getFilestream(), (void*)(rx_buffer+tot_length), RX_BUFF_MAX);	//read(filestream, storage buffer, number of bytes to read (max))
				
				//Add the rx_length to the total accumulated length.
				tot_length += rx_length;
				
				//If the tot_length == the expected length then parse the data
				if(tot_length == ACKNACK_LEN)			//The rx_length expectation (18) is hard-coded based on the expected length of the incoming data
				{
					finishedReading = true;
					//Parse the data
					parseACKNACK(rx_buffer);
				}
				break;
			default:
				printf("Error: Read Type!\n");
		}
	}
}

/**************Get functions for charge controller parameters*************/

std::string CCComm::getSerialNum()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(SERIAL_NUM);
		uartRead(SERIAL_NUM);
	}	
	return serialNum;
}
int CCComm::getMaxOutputPower()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return maxOutputPower;
}
int CCComm::getNominalBattVoltage()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return nominalBattVoltage;
}
float CCComm::getNominalChargingCurrent()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return nominalChargingCurrent;
}
float CCComm::getAbsorptionVoltage()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return absorptionVoltage;
}
float CCComm::getFloatVoltage()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return floatVoltage;
}
int CCComm::getBattType()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return battType;
}
int CCComm::getRemoteBattVoltageDetect()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return remoteBattVoltageDetect;
}
float CCComm::getBattTempCompensation()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return battTempCompensation;
}
int CCComm::getRemoteTempDetect()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return remoteTempDetect;
}
int CCComm::getBattRatedVoltageSet()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return battRatedVoltageSet;
}
int CCComm::getBattInSerial()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return battInSerial;
}
float CCComm::getBattLowWarningVoltage()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return battLowWarningVoltage;
}
int CCComm::getBattLowShutdownDetect()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(RATED_INFO);
		uartRead(RATED_INFO);
	}	
	return battLowShutdownDetect;
}
float CCComm::getPVInputVoltage()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(GENERAL_STATUS);
		uartRead(GENERAL_STATUS);
	}	
	return pvInputVoltage;
}
float CCComm::getBattVoltage()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(GENERAL_STATUS);
		uartRead(GENERAL_STATUS);
	}	
	return battVoltage;
}
float CCComm::getChargingCurrent()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(GENERAL_STATUS);
		uartRead(GENERAL_STATUS);
	}	
	return chargingCurrent;
}
float CCComm::getChargingCurrent1()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(GENERAL_STATUS);
		uartRead(GENERAL_STATUS);
	}	
	return chargingCurrent1;
}
float CCComm::getChargingCurrent2()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(GENERAL_STATUS);
		uartRead(GENERAL_STATUS);
	}	
	return chargingCurrent2;
}
int CCComm::getChargingPower()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(GENERAL_STATUS);
		uartRead(GENERAL_STATUS);
	}	
	return chargingPower;
}
int CCComm::getUnitTemp()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(GENERAL_STATUS);
		uartRead(GENERAL_STATUS);
	}	
	return unitTemp;
}
float CCComm::getRemoteBattVoltage()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(GENERAL_STATUS);
		uartRead(GENERAL_STATUS);
	}	
	return remoteBattVoltage;
}
int CCComm::getRemoteBattTemp()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(GENERAL_STATUS);
		uartRead(GENERAL_STATUS);
	}	
	return remoteBattTemp;
}
int CCComm::getStatus()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(GENERAL_STATUS);
		uartRead(GENERAL_STATUS);
	}	
	return status;
}
int CCComm::getOverChargeCurrent()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return overChargeCurrent;
}
int CCComm::getOverTemp()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return overTemp;
}
int CCComm::getBattVoltageUnder()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return battVoltageUnder;
}
int CCComm::getBattVoltageHigh()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return battVoltageHigh;
}
int CCComm::getPVHighLoss()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return pvHighLoss;
}
int CCComm::getBattTempLow()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return battTempLow;
}
int CCComm::getBattTempHigh()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return battTempHigh;
}
int CCComm::getPVLowLoss()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return pvLowLoss;
}
int CCComm::getPVHighDerating()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return pvHighDerating;
}
int CCComm::getTempHighDerating()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return tempHighDerating;
}
int CCComm::getBattTempLowAlarm()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return battTempLowAlarm;
}
int CCComm::getBattLowWarning()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(WARNING_STATUS);
		uartRead(WARNING_STATUS);
	}	
	return battLowWarning;
}
int CCComm::getBattEqualizedEn()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(EQUALIZED_INFO);
		uartRead(EQUALIZED_INFO);
	}	
	return battEqualizedEn;
}
int CCComm::getBattEqualizedTime()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(EQUALIZED_INFO);
		uartRead(EQUALIZED_INFO);
	}	
	return battEqualizedTime;
}
int CCComm::getIntervalTime()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(EQUALIZED_INFO);
		uartRead(EQUALIZED_INFO);
	}	
	return intervalTime;
}
int CCComm::getMaxCurrent()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(EQUALIZED_INFO);
		uartRead(EQUALIZED_INFO);
	}	
	return maxCurrent;
}
int CCComm::getRemainingTime()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(EQUALIZED_INFO);
		uartRead(EQUALIZED_INFO);
	}	
	return remainingTime;
}
float CCComm::getBattEqualizedVoltage()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(EQUALIZED_INFO);
		uartRead(EQUALIZED_INFO);
	}	
	return battEqualizedVoltage;
}
int CCComm::getBattCVChargeTime()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(EQUALIZED_INFO);
		uartRead(EQUALIZED_INFO);
	}	
	return battCVChargeTime;
}
int CCComm::getBattEqualizedTimeout()
{
	bool update = updateParameters();
	
	if(update)	//If an update is needed then send the appropriate write command and wait for the receive
	{
		uartWrite(EQUALIZED_INFO);
		uartRead(EQUALIZED_INFO);
	}	
	return battEqualizedTimeout;
}
clock_t CCComm::getStartTime()
{
	return startTime;
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
		CRCcalc(s_setBattType, NUM_BYTES5);
	}
	else if(battType == FLOODED)	//Set to Flooded
	{
		s_setBattType[3] = '0';
		s_setBattType[4] = '1';
		CRCcalc(s_setBattType, NUM_BYTES5);
	}
	else					//Set to customized
	{
		s_setBattType[3] = '0';
		s_setBattType[4] = '2';
		CRCcalc(s_setBattType, NUM_BYTES5);
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
	
	CRCcalc(s_setBattAbsorbtionChargingVoltage, NUM_BYTES9);

	//for(int i=0; i<NUM_BYTES9; i++)
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
	
	CRCcalc(s_setBattFloatingChargingVoltage, NUM_BYTES9);

	//for(int i=0; i<NUM_BYTES9; i++)
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
	
	CRCcalc(s_setRatedBattVoltage, NUM_BYTES6);

	//for(int i=0; i<NUM_BYTES6; i++)
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
	
	CRCcalc(s_setMaxChargingCurrent, NUM_BYTES8);

	//for(int i=0; i<NUM_BYTES8; i++)
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
		
	CRCcalc(s_enRemoteBatteryVoltageDetect, NUM_BYTES6);

	//for(int i=0; i<NUM_BYTES6; i++)
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
	CRCcalc(s_setBattLowWarningVoltage, NUM_BYTES9);

	//for(int i=0; i<NUM_BYTES9; i++)
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
		
	CRCcalc(s_setBattLowShutdownDetectEn, NUM_BYTES6);

	//for(int i=0; i<NUM_BYTES6; i++)
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
		
	CRCcalc(s_setBattEqualizationEn, NUM_BYTES6);

	//for(int i=0; i<NUM_BYTES6; i++)
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
	
	CRCcalc(s_setBattEqualizedTime, NUM_BYTES8);

	//for(int i=0; i<NUM_BYTES8; i++)
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
	
	CRCcalc(s_setBattEqualizedTime, NUM_BYTES8);

	//for(int i=0; i<NUM_BYTES8; i++)
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
	
	CRCcalc(s_setMaxCurrentBatteryEqualization, NUM_BYTES9);

	//for(int i=0; i<NUM_BYTES9; i++)
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
	CRCcalc(s_setBattEqualizedVoltage, NUM_BYTES10);

	//for(int i=0; i<NUM_BYTES10; i++)
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
	
	CRCcalc(s_setBattCVChargeTime, NUM_BYTES8);

	//for(int i=0; i<NUM_BYTES8; i++)
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
	
	CRCcalc(s_setTimeBatteryEqualizedTimeout, NUM_BYTES9);

	//for(int i=0; i<NUM_BYTES9; i++)
	//	printf("%c", s_setTimeBatteryEqualizedTimeout[i]);
}	

//Set the start time back to the current time
void CCComm::setStartTime(clock_t startTime_t)
{
	startTime = startTime_t;
}