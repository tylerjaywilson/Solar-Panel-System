/* Author: Tyler Wilson
* Date: 03/24/2016
* Notes: This class handles the communication between the raspberry pi 2 and the psoc5
*
*/
#include <stdio.h>
#include <stdint.h>
#include "i2cdefines.hpp"
#include "i2c.hpp"
#include "psoccomm.hpp"

#define			MV_2_V				1000
#define			M10A_2_A			100
#define			TX_MAX_BUFF			5
#define			BITS8				8
#define			GET_LOW_BYTE		0x0F
//Default Constructor
PSOCComm::PSOCComm()
{
	i2c0.init();	//Connect the PSoC to the RaPi2 

	/****Variables that can be read or write****/
    loadRelay 				=	false;
    externalRelay 			=	false;	
    timeMinMaxAveMinutes 	=	0;   	
    timeMinMaxAveSeconds 	= 	0;

  //Battery Variables
	batteryVoltageGain 		=	0.0;
	batteryVoltageOffset 	=	0.0;
	batteryCurrentGain 		=	0.0;
	batteryCurrentOffset 	= 	0.0;

  //Load Variables
	loadVoltageGain 		=	0.0;
	loadVoltageOffset 		=	0.0;
	loadCurrentGain 		=	0.0;
	loadCurrentOffset 		=	0.0;

  //External Variables
	externalVoltageGain 	=	0.0;
	externalVoltageOffset 	=	0.0;
	externalCurrentGain 	=	0.0;
	externalCurrentOffset 	=	0.0;

  //Extra Variables
	extraVoltageGain 		=	0.0;
	extraVoltageOffset 		=	0.0;

  //BMS Variables
	bmsLoadVoltageReconnect 			=	0.0;	
	bmsLoadTimeActionDelay 				=	0;
	bmsLoadVoltageDiscmin 				=	0.0;	
	bmsBattTimeActionDelay 				=	0;
	bmsBattVoltageEmergMinReconnect 	=	0.0;
	bmsBattVoltageEmergMinDisconnect 	=	0.0;
	bmsBattVoltageCriticalWarning 		=	0.0;

  //Real Time Clock Variables
	rtcYear 		=	0;
	rtcMonth 		=	0;	
	rtcDay 			=	0;
	rtcHour 			=	0;	
	rtcMinute 		=	0;
	rtcSecond 		=	0;	
	rtcChangesFlag 	=	0;

  //Solar Panel Offset and Gain Variables
	solPanel1Offset 		=	0.0;
	solPanel1Gain 			=	0.0;
	solPanel2Offset 		=	0.0;
	solPanel2Gain 			=	0.0;
	solPanel3Offset 		=	0.0;
	solPanel3Gain 			=	0.0;
	solPanel4Offset 		=	0.0;
	solPanel4Gain 			=	0.0;
	solPanel5Offset 		=	0.0;
	solPanel5Gain 			=	0.0;
	solPanel6Offset 		=	0.0;
	solPanel6Gain 			=	0.0;
	solPanel7Offset 		=	0.0;
	solPanel7Gain 			=	0.0;
	solPanel8Offset 		=	0.0;
	solPanel8Gain 			=	0.0;

  /**** Variables that are read only ****/

  //Real time clock variable
	rtcSecondsLeftAverage 	=	0;
  
  //BMS state variable
	bmsState 				=	0;

  //Solar Panel 8 Variables
	solPanel8Average 		=	0.0;
	solPanel8Min 			=	0.0;
	solPanel8Max 			=	0.0;
	solPanel8 				=	0.0;

  //Solar Panel 7 Variables
	solPanel7Average 		=	0.0;
	solPanel7Min 			=	0.0;
	solPanel7Max 			=	0.0;
	solPanel7 				=	0.0;

  //Solar Panel 6 Variables
	solPanel6Average 		=	0.0;
	solPanel6Min 			=	0.0;
	solPanel6Max 			=	0.0;
	solPanel6 				=	0.0;

  //Solar Panel 5 Variables
	solPanel5Average 		=	0.0;
	solPanel5Min 			=	0.0;
	solPanel5Max 			=	0.0;
	solPanel5 				=	0.0;

  //Solar Panel 4 Variables
	solPanel4Average 		=	0.0;
	solPanel4Min 			=	0.0;
	solPanel4Max 			=	0.0;
	solPanel4 				=	0.0;

  //Solar Panel 3 Variables
	solPanel3Average 		=	0.0;
	solPanel3Min 			=	0.0;
	solPanel3Max 			=	0.0;
	solPanel3 				=	0.0;

  //Solar Panel 2 Variables
	solPanel2Average 		=	0.0;
	solPanel2Min 			=	0.0;
	solPanel2Max 			=	0.0;
	solPanel2 				=	0.0;

  //Solar Panel 1 Variables
	solPanel1Average 		=	0.0;
	solPanel1Min 			=	0.0;
	solPanel1Max 			=	0.0;
	solPanel1 				=	0.0;

  //Battery, Load, External, and Extra Variables
	batteryVoltageAverage 		=	0.0;
	batteryVoltageMin 			=	0.0;
	batteryVoltageMax 			=	0.0;
	batteryVoltage 				=	0.0;

	batteryCurrentAverage 		=	0.0;
	batteryCurrentMin 			=	0.0;
	batteryCurrentMax 			=	0.0;
	batteryCurrent 				=	0.0;

	loadVoltageAverage 			=	0.0;
	loadVoltageMin 				=	0.0;
	loadVoltageMax 				=	0.0;
	loadVoltage 				=	0.0;

	loadCurrentAverage 			=	0.0;
	loadCurrentMin 				=	0.0;
	loadCurrentMax 				=	0.0;
	loadCurrent 				=	0.0;

	externalVoltageAverage 		=	0.0;
	externalVoltageMin 			=	0.0;
	externalVoltageMax 			=	0.0;
	externalVoltage 			=	0.0;

	externalCurrentAverage 		=	0.0;
	externalCurrentMin 			=	0.0;
	externalCurrentMax 			=	0.0;
	externalCurrent 			=	0.0;

	extraVoltageAverage 		=	0.0;
	extraVoltageMin 			=	0.0;
	extraVoltageMax 			=	0.0;
	extraVoltage 				=	0.0;
}

//Convert float to two unsigned characters in an array
void PSOCComm::float2Chars(float value, unsigned char *f_value)
{
	//Split the float into two unsigned int8's
	int16_t val16 = (int16_t) value;
	uint8_t lowbyte = val16 & GET_LOW_BYTE;
	uint8_t highbyte = (val16 >> BITS8) & GET_LOW_BYTE;

	//Assign those uint8's as characters to the array that stores the floats;
	f_value[0] = (unsigned char) lowbyte;
	f_value[1] = (unsigned char) highbyte;
}

/**********SET FUNCTIONS*************/
void    PSOCComm::setLoadRelay(bool loadRelay_t)
{
	loadRelay = loadRelay_t;
	int onOff = 0;
	unsigned char tx_buff[TX_MAX_BUFF];

	//Determine if the load relay is on or off
	if(loadRelay_t)
		onOff = 1;

	//Cast the int to a character
	tx_buff[0] = (char) onOff;
	//Write the value to the PSOC memory address
	i2c0.write8(I2C_LOAD_RELAY, tx_buff);
}
void    PSOCComm::setExternalRelay(bool externalRelay_t)
{
	externalRelay = externalRelay_t;
	int onOff = 0;
	unsigned char tx_buff[TX_MAX_BUFF];

	//Determine if the load relay is on or off
	if(externalRelay_t)
		onOff = 1;

	//Cast the int to a character
	tx_buff[0] = (char) onOff;
	//Write the value to the PSOC memory address
	i2c0.write8(I2C_EXT_RELAY, tx_buff);	
}
void    PSOCComm::setTimeMinMaxAveMinutes(int8_t timeMinMaxAveMinutes_t)
{
	timeMinMaxAveMinutes = timeMinMaxAveMinutes_t;

	unsigned char tx_buff[TX_MAX_BUFF];
	//Convert the value to a character and prepare for transmission
	tx_buff[0] = (unsigned char) timeMinMaxAveMinutes_t;
	i2c0.write8(I2C_TIME_MINMAXAVE_MINUTES, tx_buff);
}   
void    PSOCComm::setTimeMinMaxAveSeconds(int8_t timeMinMaxAveSeconds_t)
{
	timeMinMaxAveSeconds = timeMinMaxAveSeconds_t;

	unsigned char tx_buff[TX_MAX_BUFF];
	//Convert the value to a character and prepare for transmission
	tx_buff[0] = (unsigned char) timeMinMaxAveSeconds_t;
	i2c0.write8(I2C_TIME_MINMAXAVE_SECONDS, tx_buff);
}

  //Battery Variables
void    PSOCComm::setBatteryVoltageGain(float batteryVoltageGain_t)
{
	batteryVoltageGain = batteryVoltageGain_t;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(batteryVoltageGain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_BATT_V_GAIN_LBYTE, tx_buff);	
}
void    PSOCComm::setBatteryVoltageOffset(float)
{
	
}
void    PSOCComm::setBatteryCurrentGain(float)
{
	
}
void    PSOCComm::setBatteryCurrentOffset(float)
{
	
}

  //Load Variables
void    PSOCComm::setLoadVoltageGain(float)
{
	
}
void    PSOCComm::setLoadVoltageOffset(float)
{
	
}
void    PSOCComm::setLoadCurrentGain(float)
{
	
}
void    PSOCComm::setLoadCurrentOffset(float)
{
	
}

  //External Variables
void    PSOCComm::setExternalVoltageGain(float)
{
	
}
void    PSOCComm::setExternalVoltageOffset(float)
{
	
}
void    PSOCComm::setExternalCurrentGain(float)
{
	
}
void    PSOCComm::setExternalCurrentOffset(float)
{
	
}

  //Extra Variables
void    PSOCComm::setExtraVoltageGain(float)
{
	
}
void    PSOCComm::setExtraVoltageOffset(float)
{
	
}

  //BMS Variables
void    PSOCComm::setBmsLoadVoltageReconnect(float)
{
	
}
void    PSOCComm::setBmsLoadTimeActionDelay(uint8_t)
{
	
}
void    PSOCComm::setBmsLoadVoltageDiscmin(float)
{
	
}
void    PSOCComm::setBmsBattTimeActionDelay(uint8_t)
{
	
}
void    PSOCComm::setBmsBattVoltageEmergMinReconnect(float)
{
	
}
void    PSOCComm::setBmsBattVoltageEmergMinDisconnect(float)
{
	
}
void    PSOCComm::setBmsBattVoltageCriticalWarning(float)
{
	
}

  //Real Time Clock Variables
void    PSOCComm::setRtcYear(int16_t)
{
	
}
void    PSOCComm::setRtcMonth(uint8_t)
{
	
}
void    PSOCComm::setRtcDay(uint8_t)
{
	
}
void    PSOCComm::setRtcHour(uint8_t)
{
	
}
void    PSOCComm::setRtcMinute(uint8_t)
{
	
}
void    PSOCComm::setRtcSecond(uint8_t)
{
	
}
void    PSOCComm::setRtcChangesFlag(uint8_t)
{
	
}

  //Solar Panel Offset and Gain Variables
void    PSOCComm::setSolPanel1Offset(float)
{
	
}
void    PSOCComm::setSolPanel1Gain(float)
{
	
}
void    PSOCComm::setSolPanel2Offset(float)
{
	
}
void    PSOCComm::setSolPanel2Gain(float)
{
	
}
void    PSOCComm::setSolPanel3Offset(float)
{
	
}
void    PSOCComm::setSolPanel3Gain(float)
{
	
}
void    PSOCComm::setSolPanel4Offset(float)
{
	
}
void    PSOCComm::setSolPanel4Gain(float)
{
	
}
void    PSOCComm::setSolPanel5Offset(float)
{
	
}
void    PSOCComm::setSolPanel5Gain(float)
{
	
}
void    PSOCComm::setSolPanel6Offset(float)
{
	
}
void    PSOCComm::setSolPanel6Gain(float)
{
	
}
void    PSOCComm::setSolPanel7Offset(float)
{
	
}
void    PSOCComm::setSolPanel7Gain(float)
{
	
}
void    PSOCComm::setSolPanel8Offset(float)
{
	
}
void    PSOCComm::setSolPanel8Gain(float)
{
	
}

/*********GET FUNCTIONS**************/

bool    PSOCComm::getLoadRelay()
{
	return loadRelay;
}
bool    PSOCComm::getExternalRelay()
{
	return externalRelay;
}
int8_t  PSOCComm::getTimeMinMaxAveMinutes()
{
	return timeMinMaxAveMinutes;
}   
int8_t  PSOCComm::getTimeMinMaxAveSeconds()
{
	return timeMinMaxAveSeconds;
}

//Battery Variables
float   PSOCComm::getBatteryVoltageGain()
{
	return batteryVoltageGain;
}
float   PSOCComm::getBatteryVoltageOffset()
{
	return batteryVoltageOffset;
}
float   PSOCComm::getBatteryCurrentGain()
{
	return batteryCurrentGain;
}
float   PSOCComm::getBatteryCurrentOffset()
{
	return batteryCurrentOffset;
}

//Load Variables
float   PSOCComm::getLoadVoltageGain()
{
	return loadVoltageGain;
}
float   PSOCComm::getLoadVoltageOffset()
{
	return loadVoltageOffset;
}
float   PSOCComm::getLoadCurrentGain()
{
	return loadCurrentGain;
}
float   PSOCComm::getLoadCurrentOffset()
{
	return loadCurrentOffset;
}

//External Variables
float   PSOCComm::getExternalVoltageGain()
{
	return externalVoltageGain;
}
float   PSOCComm::getExternalVoltageOffset()
{
	return externalVoltageOffset;
}
float   PSOCComm::getExternalCurrentGain()
{
	return externalCurrentGain;
}
float   PSOCComm::getExternalCurrentOffset()
{
	return externalCurrentOffset;
}

//Extra Variables
float   PSOCComm::getExtraVoltageGain()
{
	return extraVoltageGain;
}
float   PSOCComm::getExtraVoltageOffset()
{
	return extraVoltageOffset;
}

//BMS Variables
float   PSOCComm::getBmsLoadVoltageReconnect()
{
	return bmsLoadVoltageReconnect;
}
uint8_t PSOCComm::getBmsLoadTimeActionDelay()
{
	return bmsLoadTimeActionDelay;
}
float   PSOCComm::getBmsLoadVoltageDiscmin()
{
	return bmsLoadVoltageDiscmin;
}
uint8_t PSOCComm::getBmsBattTimeActionDelay()
{
	return bmsBattTimeActionDelay;
}
float   PSOCComm::getBmsBattVoltageEmergMinReconnect()
{
	return bmsBattVoltageEmergMinReconnect;
}
float   PSOCComm::getBmsBattVoltageEmergMinDisconnect()
{
	return bmsBattVoltageEmergMinDisconnect;
}
float   PSOCComm::getBmsBattVoltageCriticalWarning()
{
	return bmsBattVoltageCriticalWarning;
}

//Real Time Clock Variables
int16_t PSOCComm::getRtcYear()
{
	return rtcYear;
}
uint8_t PSOCComm::getRtcMonth()
{
	return rtcMonth;
}
uint8_t PSOCComm::getRtcDay()
{
	return rtcDay;
}
uint8_t PSOCComm::getRtcHour()
{
	return rtcHour;
}
uint8_t PSOCComm::getRtcMinute()
{
	return rtcMinute;
}
uint8_t PSOCComm::getRtcSecond()
{
	return rtcSecond;
}
uint8_t PSOCComm::getRtcChangesFlag()
{
	return rtcChangesFlag;
}

//Solar Panel Offset and Gain Variables
float   PSOCComm::getSolPanel1Offset()
{
	return solPanel1Offset;
}
float   PSOCComm::getSolPanel1Gain()
{
	return solPanel1Gain;
}
float   PSOCComm::getSolPanel2Offset()
{
	return solPanel2Offset;
}
float   PSOCComm::getSolPanel2Gain()
{
	return solPanel2Gain;
}
float   PSOCComm::getSolPanel3Offset()
{
	return solPanel3Offset;
}
float   PSOCComm::getSolPanel3Gain()
{
	return solPanel3Gain;
}
float   PSOCComm::getSolPanel4Offset()
{
	return solPanel4Offset;
}
float   PSOCComm::getSolPanel4Gain()
{
	return solPanel4Gain;
}
float   PSOCComm::getSolPanel5Offset()
{
	return solPanel5Offset;
}
float   PSOCComm::getSolPanel5Gain()
{
	return solPanel5Gain;
}
float   PSOCComm::getSolPanel6Offset()
{
	return solPanel6Offset;
}
float   PSOCComm::getSolPanel6Gain()
{
	return solPanel6Gain;
}
float   PSOCComm::getSolPanel7Offset()
{
	return solPanel7Offset;
}
float   PSOCComm::getSolPanel7Gain()
{
	return solPanel7Gain;
}
float   PSOCComm::getSolPanel8Offset()
{
	return solPanel8Offset;
}
float   PSOCComm::getSolPanel8Gain()
{
	return solPanel8Gain;
}

//Real time clock variable
uint8_t PSOCComm::getRtcSecondsLeftAverage()
{
	return rtcSecondsLeftAverage;
}

//BMS state variable
uint8_t PSOCComm::getBmsState()
{
	return bmsState;
}

//Solar Panel 8 Variables
float   PSOCComm::getSolPanel8Average()
{
	return solPanel8Offset;
}
float   PSOCComm::getSolPanel8Min()
{
	return solPanel8Min;
}
float   PSOCComm::getSolPanel8Max()
{
	return solPanel8Max;
}
float   PSOCComm::getSolPanel8()
{
	return solPanel8;
}

//Solar Panel 7 Variables
float   PSOCComm::getSolPanel7Average()
{
	return solPanel7Average;
}
float   PSOCComm::getSolPanel7Min()
{
	return solPanel7Min;
}
float   PSOCComm::getSolPanel7Max()
{
	return solPanel7Max;
}
float   PSOCComm::getSolPanel7()
{
	return solPanel7;
}

//Solar Panel 6 Variables
float   PSOCComm::getSolPanel6Average()
{
	return solPanel6Average;
}
float   PSOCComm::getSolPanel6Min()
{
	return solPanel6Min;
}
float   PSOCComm::getSolPanel6Max()
{
	return solPanel6Max;
}
float   PSOCComm::getSolPanel6()
{
	return solPanel6;
}

//Solar Panel 5 Variables
float   PSOCComm::getSolPanel5Average()
{
	return solPanel5Average;
}
float   PSOCComm::getSolPanel5Min()
{
	return solPanel5Min;
}
float   PSOCComm::getSolPanel5Max()
{
	return solPanel5Max;
}
float   PSOCComm::getSolPanel5()
{
	return solPanel5;
}

//Solar Panel 4 Variables
float   PSOCComm::getSolPanel4Average()
{
	return solPanel4Average;
}
float   PSOCComm::getSolPanel4Min()
{
	return solPanel4Min;
}
float   PSOCComm::getSolPanel4Max()
{
	return solPanel4Max;
}
float   PSOCComm::getSolPanel4()
{
	return solPanel4;
}

//Solar Panel 3 Variables
float   PSOCComm::getSolPanel3Average()
{
	return solPanel3Average;
}
float   PSOCComm::getSolPanel3Min()
{
	return solPanel3Min;
}
float   PSOCComm::getSolPanel3Max()
{
	return solPanel3Max;
}
float   PSOCComm::getSolPanel3()
{
	return solPanel3;
}

//Solar Panel 2 Variables
float   PSOCComm::getSolPanel2Average()
{
	return solPanel2Average;
}
float   PSOCComm::getSolPanel2Min()
{
	return solPanel2Min;
}
float   PSOCComm::getSolPanel2Max()
{
	return solPanel2Max;
}
float   PSOCComm::getSolPanel2()
{
	return solPanel2;
}

//Solar Panel 1 Variables
float   PSOCComm::getSolPanel1Average()
{
	return solPanel1Average;
}
float   PSOCComm::getSolPanel1Min()
{
	return solPanel1Min;
}
float   PSOCComm::getSolPanel1Max()
{
	return solPanel1Max;
}
float   PSOCComm::getSolPanel1()
{
	return solPanel1;
}

//Battery, Load, External, and Extra Variables
float   PSOCComm::getBatteryVoltageAverage()
{
	return batteryVoltageAverage;
}
float   PSOCComm::getBatteryVoltageMin()
{
	return batteryVoltageMin;
}
float   PSOCComm::getBatteryVoltageMax()
{
	return batteryVoltageMax;
}

float   PSOCComm::getBatteryCurrentAverage()
{
	return batteryCurrentAverage;
}
float   PSOCComm::getBatteryCurrentMin()
{
	return batteryCurrentMin;
}
float   PSOCComm::getBatteryCurrentMax()
{
	return batteryCurrentMax;
}

float   PSOCComm::getLoadVoltageAverage()
{
	return loadVoltageAverage;
}
float   PSOCComm::getLoadVoltageMin()
{
	return loadVoltageMin;
}
float   PSOCComm::getLoadVoltageMax()
{
	return loadVoltageMax;
}

float   PSOCComm::getLoadCurrentAverage()
{
	return loadCurrentAverage;
}
float   PSOCComm::getLoadCurrentMin()
{
	return loadCurrentMin;
}
float   PSOCComm::getLoadCurrentMax()
{
	return loadCurrentMax;
}

float   PSOCComm::getExternalVoltageAverage()
{
	return externalVoltageAverage;
}
float   PSOCComm::getExternalVoltageMin()
{
	return externalVoltageMin;
}
float   PSOCComm::getExternalVoltageMax()
{
	return externalVoltageMax;
}

float   PSOCComm::getExternalCurrentAverage()
{
	return externalCurrentAverage;
}
float   PSOCComm::getExternalCurrentMin()
{
	return externalCurrentMin;
}
float   PSOCComm::getExternalCurrentMax()
{
	return externalCurrentMax;
}

float   PSOCComm::getExtraVoltageAverage()
{
	return extraVoltageAverage;
}
float   PSOCComm::getExtraVoltageMin()
{
	return extraVoltageMin;
}
float   PSOCComm::getExtraVoltageMax()
{
	return extraVoltageMax;
}

float PSOCComm::getBatteryVoltage()
{
	//Read the battery voltage level, convert it to a float, and do the appropriate arithmetic to convert to Volt form (instead of mV)
	int16_t retVal = i2c0.read16(I2C_BATT_VOLTAGE_LBYTE);
	float temp_batteryVoltage = (float) retVal;
	batteryVoltage = temp_batteryVoltage/MV_2_V;
	return batteryVoltage;
}
float PSOCComm::getBatteryCurrent()
{
	int16_t retVal = i2c0.read16(I2C_BATT_CURRENT_LBYTE);
	float temp_batteryCurrent = (float) retVal;
	batteryCurrent = temp_batteryCurrent/M10A_2_A;
	return batteryCurrent;
}
float PSOCComm::getLoadVoltage()
{
	int16_t retVal = i2c0.read16(I2C_LOAD_VOLTAGE_LBYTE);
	float temp_loadVoltage = (float) retVal;
	loadVoltage = temp_loadVoltage/MV_2_V;
	return loadVoltage;
}
float PSOCComm::getLoadCurrent()
{
	int16_t retVal = i2c0.read16(I2C_LOAD_CURRENT_LBYTE);
	float temp_loadCurrent = (float) retVal;
	loadCurrent = temp_loadCurrent/M10A_2_A;
	return loadCurrent;
}
float PSOCComm::getExternalVoltage()
{
	int16_t retVal = i2c0.read16(I2C_EXT_VOLTAGE_LBYTE);
	float temp_externalVoltage = (float) retVal;
	externalVoltage = temp_externalVoltage/MV_2_V;
	return externalVoltage;
}
float PSOCComm::getExternalCurrent()
{
	int16_t retVal = i2c0.read16(I2C_EXT_CURRENT_LBYTE);
	float temp_externalCurrent = (float) retVal;
	externalCurrent = temp_externalCurrent/M10A_2_A;
	return externalCurrent;
}
float PSOCComm::getExtraVoltage()
{
	int16_t retVal = i2c0.read16(I2C_EXTRA_VOLTAGE_LBYTE);
	float temp_extraVoltage = (float) retVal;
	extraVoltage = temp_extraVoltage/MV_2_V;
	return extraVoltage;
}  	
