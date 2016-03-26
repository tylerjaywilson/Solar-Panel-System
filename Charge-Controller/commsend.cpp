/*
* Authors: Tyler Wilson
* Date: 02/24/2016
* Notes: This class handles the transmission commands to the MPPT-3000 RS232 Charge Controller
* The necessary commands are hard-coded to allow for easy requesting of specific charge controller information
*/
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "commsend.hpp"

/* These #define's indicate the number of bytes within any given character array */
#define LEN2 2
#define LEN3 3
#define LEN5 5
#define LEN6 6
#define LEN8 8
#define LEN9 9
#define LEN10 10

#define INTEGER_BASE 10

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
	s_setControlParameterDefault[0] = 'P';			//PF setting array
	s_setControlParameterDefault[1] = 'F';			//Reset to all default parameters
	CRCcalc(s_setControlParameterDefault, LEN2);	//Append CRC	
}

/*******Set functions*******/
//Set the battery type
void CommSend::setBattType(int battType)
{
	if(battType == 0)		//Set to AGM
	{
		s_setBattType[3] = '0';
		s_setBattType[4] = '0';
		CRCcalc(s_setBattType, LEN5);
	}
	else if(battType == 1)	//Set to Flooded
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
void CommSend::setBattAbsorbtionChargingVoltage(float chargeVolt)
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
void CommSend::setBattFloatingChargingVoltage(float floatVolt)
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
void CommSend::setRatedBattVoltage(int battV)
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
void CommSend::setMaxChargingCurrent(int chargeCurr)
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
void CommSend::setEnRemoteBatteryVoltageDetect(int endis)
{
	if(endis == 0)	//Disable
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
void CommSend::setBattLowWarningVoltage(float lowVolt)
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
void CommSend::setBattLowShutdownDetectEn(int endis)
{
	if(endis == 0)	//Disable
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
void CommSend::setBattEqualizationEn(int endis)
{
	if(endis == 0)	//Disable
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
void CommSend::setBattEqualizedTime(int eqTime)
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
void CommSend::setPeriodBattEqualization(int period)
{
	char const *period_p;
	std::string period_t;

	period_t = std::to_string(period);
	period_p = period_t.c_str();

	if(period > 99)
	{
		//Update the array with the new current value
		s_setBattEqualizedTime[5] = period_p[0];
		s_setBattEqualizedTime[6] = period_p[1];
		s_setBattEqualizedTime[7] = period_p[2];
	}
	else if(period > 9)
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
void CommSend::setMaxCurrentBatteryEqualization(int maxCurr)
{
	char const *maxCurr_p;
	std::string maxCurr_t;

	maxCurr_t = std::to_string(maxCurr);
	maxCurr_p = maxCurr_t.c_str();

	if(maxCurr > 99)
	{
		//Update the array with the new current value
		s_setMaxCurrentBatteryEqualization[6] = maxCurr_p[0];
		s_setMaxCurrentBatteryEqualization[7] = maxCurr_p[1];
		s_setMaxCurrentBatteryEqualization[8] = maxCurr_p[2];
	}
	else if(maxCurr > 9)
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
void CommSend::setBattEqualizedVoltage(float equalVolt)
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
void CommSend::setBattCVChargeTime(int cvtime)
{
	char const *cvtime_p;
	std::string cvtime_t;

	cvtime_t = std::to_string(cvtime);
	cvtime_p = cvtime_t.c_str();

	if(cvtime > 99)
	{
		//Update the array with the new current value
		s_setBattCVChargeTime[5] = cvtime_p[0];
		s_setBattCVChargeTime[6] = cvtime_p[1];
		s_setBattCVChargeTime[7] = cvtime_p[2];
	}
	else if(cvtime > 9)
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
void CommSend::setTimeBatteryEqualizedTimeout(int timeout)
{
	char const *timeout_p;
	std::string timeout_t;

	timeout_t = std::to_string(timeout);
	timeout_p = timeout_t.c_str();

	if(timeout > 99)
	{
		//Update the array with the new current value
		s_setTimeBatteryEqualizedTimeout[6] = timeout_p[0];
		s_setTimeBatteryEqualizedTimeout[7] = timeout_p[1];
		s_setTimeBatteryEqualizedTimeout[8] = timeout_p[2];
	}
	else if(timeout > 9)
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