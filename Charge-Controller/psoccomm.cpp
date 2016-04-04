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
	bmsBattTimeEmergDelay 				=	0;
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

//Convert int16's to two unsigned characters in an array
void PSOCComm::int162Chars(int16_t val, unsigned char* i_value)
{
	//Split the float into two unsigned int8's
	uint8_t lowbyte = val & GET_LOW_BYTE;
	uint8_t highbyte = (val >> BITS8) & GET_LOW_BYTE;

	//Assign those uint8's as characters to the array that stores the floats;
	i_value[0] = (unsigned char) lowbyte;
	i_value[1] = (unsigned char) highbyte;
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

	//Convert to form for writing to PSOC memory
	batteryVoltageGain_t = batteryVoltageGain_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(batteryVoltageGain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_BATT_V_GAIN_LBYTE, tx_buff);	
}
void    PSOCComm::setBatteryVoltageOffset(float batteryVoltageOffset_t)
{
	batteryVoltageOffset = batteryVoltageOffset_t;

	//Convert to form for writing to PSOC memory
	batteryVoltageOffset_t = batteryVoltageOffset_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(batteryVoltageOffset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_BATT_V_OFFSET_LBYTE, tx_buff);	
}
void    PSOCComm::setBatteryCurrentGain(float batteryCurrentGain_t)
{
	batteryCurrentGain = batteryCurrentGain_t;

	//Convert to form for writing to PSOC memory
	batteryCurrentGain_t = batteryCurrentGain_t*M10A_2_A;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(batteryCurrentGain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_BATT_I_GAIN_LBYTE, tx_buff);	
}
void    PSOCComm::setBatteryCurrentOffset(float batteryCurrentOffset_t)
{
	batteryCurrentOffset = batteryCurrentOffset_t;

	//Convert to form for writing to PSOC memory
	batteryCurrentOffset_t = batteryCurrentOffset_t*M10A_2_A;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(batteryCurrentOffset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_BATT_I_OFFSET_LBYTE, tx_buff);
}

  //Load Variables
void    PSOCComm::setLoadVoltageGain(float loadVoltageGain_t)
{
	loadVoltageGain = loadVoltageGain_t;

	//Convert to form for writing to PSOC memory
	loadVoltageGain_t = loadVoltageGain_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(loadVoltageGain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_LOAD_V_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setLoadVoltageOffset(float loadVoltageOffset_t)
{
	loadVoltageOffset = loadVoltageOffset_t;

	//Convert to form for writing to PSOC memory
	loadVoltageOffset_t = loadVoltageOffset_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(loadVoltageOffset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_LOAD_V_OFFSET_LBYTE, tx_buff);
}
void    PSOCComm::setLoadCurrentGain(float loadCurrentGain_t)
{
	loadCurrentGain = loadCurrentGain_t;

	//Convert to form for writing to PSOC memory
	loadCurrentGain_t = loadCurrentGain_t*M10A_2_A;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(loadCurrentGain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_LOAD_I_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setLoadCurrentOffset(float loadCurrentOffset_t)
{
	loadCurrentOffset = loadCurrentOffset_t;

	//Convert to form for writing to PSOC memory
	loadCurrentOffset_t = loadCurrentOffset_t*M10A_2_A;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(loadCurrentOffset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_LOAD_I_OFFSET_LBYTE, tx_buff);
}

  //External Variables
void    PSOCComm::setExternalVoltageGain(float externalVoltageGain_t)
{
	externalVoltageGain = externalVoltageGain_t;

	//Convert to form for writing to PSOC memory
	externalVoltageGain_t = externalVoltageGain_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(externalVoltageGain, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_EXT_V_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setExternalVoltageOffset(float externalVoltageOffset_t)
{
	externalVoltageOffset = externalVoltageOffset_t;

	//Convert to form for writing to PSOC memory
	externalVoltageOffset_t = externalVoltageOffset_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(externalVoltageOffset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_EXT_V_OFFSET_LBYTE, tx_buff);	
}
void    PSOCComm::setExternalCurrentGain(float externalCurrentGain_t)
{
	externalCurrentGain = externalCurrentGain_t;

	//Convert to form for writing to PSOC memory
	externalCurrentGain_t = externalCurrentGain_t*M10A_2_A;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(externalCurrentGain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_EXT_I_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setExternalCurrentOffset(float externalCurrentOffset_t)
{
	externalCurrentOffset = externalCurrentOffset_t;

	//Convert to form for writing to PSOC memory
	externalCurrentOffset_t = externalCurrentOffset_t*M10A_2_A;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(externalCurrentOffset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_EXT_I_OFFSET_LBYTE, tx_buff);
}

  //Extra Variables
void    PSOCComm::setExtraVoltageGain(float extraVoltageGain_t)
{
	extraVoltageGain = extraVoltageGain_t;

	//Convert to form for writing to PSOC memory
	extraVoltageGain_t = extraVoltageGain_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(extraVoltageGain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_EXTRA_V_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setExtraVoltageOffset(float extraVoltageOffset_t)
{
	extraVoltageOffset = extraVoltageOffset_t;

	//Convert to form for writing to PSOC memory
	extraVoltageOffset_t = extraVoltageOffset_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(extraVoltageOffset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_EXTRA_V_OFFSET_LBYTE, tx_buff);
}

  //BMS Variables
void    PSOCComm::setBmsLoadVoltageReconnect(float bmsLoadVoltageReconnect_t)
{
	bmsLoadVoltageReconnect = bmsLoadVoltageReconnect_t;

	//Convert to form for writing to PSOC memory
	bmsLoadVoltageReconnect_t = bmsLoadVoltageReconnect_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(bmsLoadVoltageReconnect_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_BMS_I2C_LOAD_V_RECONNECT_LBYTE, tx_buff);	
}
void    PSOCComm::setBmsLoadTimeActionDelay(uint8_t bmsLoadTimeActionDelay_t)
{
	//Set the class private variable
	bmsLoadTimeActionDelay = bmsLoadTimeActionDelay_t;

	//Cast the variable to a character and prepare to send
	unsigned char tx_buff[TX_MAX_BUFF] = {0};
	tx_buff[0] = (unsigned char) bmsLoadTimeActionDelay_t;

	//Write the value to the PSoC
	i2c0.write8(I2C_BMS_LOAD_TIME_ACTIONDELAY_DISC_HBYTE, tx_buff);
}
void    PSOCComm::setBmsLoadVoltageDiscmin(float bmsLoadVoltageDiscmin_t)
{
	//Set the class private variable
	bmsLoadVoltageDiscmin = bmsLoadVoltageDiscmin_t;

	//Convert to form for writing to PSOC memory
	bmsLoadVoltageDiscmin_t = bmsLoadVoltageDiscmin_t*MV_2_V;
	
	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(bmsLoadVoltageDiscmin_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_BMS_I2C_LOAD_V_DISCMIN_LBYTE, tx_buff);	
}
void    PSOCComm::setBmsBattTimeEmergDelay(uint8_t bmsBattTimeEmergDelay_t)
{
	//Set the class private variable
	bmsBattTimeEmergDelay = bmsBattTimeEmergDelay_t;

	//Cast the variable to a character and prepare to send
	unsigned char tx_buff[TX_MAX_BUFF] = {0};
	tx_buff[0] = (unsigned char) bmsBattTimeEmergDelay_t;

	//Write the value to the PSoC
	i2c0.write8(I2C_BMS_BATT_TIME_EMERG_DELAY_DISC_HBYTE, tx_buff);
}
void    PSOCComm::setBmsBattVoltageEmergMinReconnect(float bmsBattVoltageEmergMinReconnect_t)
{
	//Set the class private variable
	bmsBattVoltageEmergMinReconnect = bmsBattVoltageEmergMinReconnect_t;

	//Convert to form for writing to PSOC memory
	bmsBattVoltageEmergMinReconnect_t = bmsBattVoltageEmergMinReconnect_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(bmsBattVoltageEmergMinReconnect_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_BMS_BATT_V_EMERGMIN_RECONNECT_LBYTE, tx_buff);	
}
void    PSOCComm::setBmsBattVoltageEmergMinDisconnect(float bmsBattVoltageEmergMinDisconnect_t)
{
	//Set the class private variable
	bmsBattVoltageEmergMinDisconnect = bmsBattVoltageEmergMinDisconnect_t;

	//Convert to form for writing to PSOC memory
	bmsBattVoltageEmergMinDisconnect_t = bmsBattVoltageEmergMinDisconnect_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(bmsBattVoltageEmergMinDisconnect_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_BMS_BATT_V_EMERGMIN_LBYTE, tx_buff);	
}
void    PSOCComm::setBmsBattVoltageCriticalWarning(float bmsBattVoltageCriticalWarning_t)
{
	//Set the class private variable
	bmsBattVoltageCriticalWarning = bmsBattVoltageCriticalWarning_t;

	//Convert to form for writing to PSOC memory
	bmsBattVoltageCriticalWarning_t = bmsBattVoltageCriticalWarning_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(bmsBattVoltageCriticalWarning_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_BMS_BATT_V_CRITICAL_WARNING_LBYTE, tx_buff);
}

  //Real Time Clock Variables
void    PSOCComm::setRtcYear(int16_t rtcYear_t)
{
	//Set the class private variable
	rtcYear = rtcYear_t;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store int16 into the tx_buff array as a high and low byte
	int162Chars(rtcYear_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_RTC_YEAR_LBYTE, tx_buff);
}
void    PSOCComm::setRtcMonth(uint8_t rtcMonth_t)
{
	//Set the class private variable
	rtcMonth = rtcMonth_t;

	//Cast the variable to a character and prepare to send
	unsigned char tx_buff[TX_MAX_BUFF] = {0};
	tx_buff[0] = (unsigned char) rtcMonth_t;

	//Write the value to the PSoC
	i2c0.write8(I2C_RTC_MONTH, tx_buff);
}
void    PSOCComm::setRtcDay(uint8_t rtcDay_t)
{
	//Set the class private variable
	rtcDay = rtcDay_t;

	//Cast the variable to a character and prepare to send
	unsigned char tx_buff[TX_MAX_BUFF] = {0};
	tx_buff[0] = (unsigned char) rtcDay_t;

	//Write the value to the PSoC
	i2c0.write8(I2C_RTC_DAYOFMONTH, tx_buff);
}
void    PSOCComm::setRtcHour(uint8_t rtcHour_t)
{
	//Set the class private variable
	rtcHour = rtcHour_t;

	//Cast the variable to a character and prepare to send
	unsigned char tx_buff[TX_MAX_BUFF] = {0};
	tx_buff[0] = (unsigned char) rtcHour_t;

	//Write the value to the PSoC
	i2c0.write8(I2C_RTC_HOUR, tx_buff);
}
void    PSOCComm::setRtcMinute(uint8_t rtcMinute_t)
{
	//Set the class private variable
	rtcMinute = rtcMinute_t;

	//Cast the variable to a character and prepare to send
	unsigned char tx_buff[TX_MAX_BUFF] = {0};
	tx_buff[0] = (unsigned char) rtcMinute_t;

	//Write the value to the PSoC
	i2c0.write8(I2C_RTC_MINUTE, tx_buff);
}
void    PSOCComm::setRtcSecond(uint8_t rtcSecond_t)
{
	//Set the class private variable
	rtcSecond = rtcSecond_t;

	//Cast the variable to a character and prepare to send
	unsigned char tx_buff[TX_MAX_BUFF] = {0};
	tx_buff[0] = (unsigned char) rtcSecond_t;

	//Write the value to the PSoC
	i2c0.write8(I2C_RTC_SECOND, tx_buff);
}
void    PSOCComm::setRtcChangesFlag(uint8_t rtcChangesFlag_t)
{
	//Set the class private variable
	rtcChangesFlag = rtcChangesFlag_t;

	//Cast the variable to a character and prepare to send
	unsigned char tx_buff[TX_MAX_BUFF] = {0};
	tx_buff[0] = (unsigned char) rtcChangesFlag_t;

	//Write the value to the PSoC
	i2c0.write8(I2C_RTC_CHANGES_FLAG, tx_buff);
}

  //Solar Panel Offset and Gain Variables
void    PSOCComm::setSolPanel1Offset(float solPanel1Offset_t)
{
	//Set the class private variable
	solPanel1Offset = solPanel1Offset_t;

	//Convert to form for writing to PSOC memory
	solPanel1Offset_t = solPanel1Offset_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel1Offset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_1_OFFSET_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel1Gain(float solPanel1Gain_t)
{
	//Set the class private variable
	solPanel1Gain = solPanel1Gain_t;

	//Convert to form for writing to PSOC memory
	solPanel1Gain_t = solPanel1Gain_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel1Gain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_1_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel2Offset(float solPanel2Offset_t)
{
	//Set the class private variable
	solPanel2Offset = solPanel2Offset_t;

	//Convert to form for writing to PSOC memory
	solPanel2Offset_t = solPanel2Offset_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel2Offset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_2_OFFSET_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel2Gain(float solPanel2Gain_t)
{
	//Set the class private variable
	solPanel2Gain = solPanel2Gain_t;

	//Convert to form for writing to PSOC memory
	solPanel2Gain_t = solPanel2Gain_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel2Gain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_2_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel3Offset(float solPanel3Offset_t)
{
	//Set the class private variable
	solPanel3Offset = solPanel3Offset_t;

	//Convert to form for writing to PSOC memory
	solPanel3Offset_t = solPanel3Offset_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel3Offset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_3_OFFSET_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel3Gain(float solPanel3Gain_t)
{
	//Set the class private variable
	solPanel3Gain = solPanel3Gain_t;

	//Convert to form for writing to PSOC memory
	solPanel3Gain_t = solPanel3Gain_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel3Gain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_3_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel4Offset(float solPanel4Offset_t)
{
	//Set the class private variable
	solPanel4Offset = solPanel4Offset_t;

	//Convert to form for writing to PSOC memory
	solPanel4Offset_t = solPanel4Offset_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel4Offset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_4_OFFSET_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel4Gain(float solPanel4Gain_t)
{
	//Set the class private variable
	solPanel4Gain = solPanel4Gain_t;

	//Convert to form for writing to PSOC memory
	solPanel4Gain_t = solPanel4Gain_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel4Gain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_4_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel5Offset(float solPanel5Offset_t)
{
	//Set the class private variable
	solPanel5Offset = solPanel5Offset_t;

	//Convert to form for writing to PSOC memory
	solPanel5Offset_t = solPanel5Offset_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel5Offset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_5_OFFSET_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel5Gain(float solPanel5Gain_t)
{
	//Set the class private variable
	solPanel5Gain = solPanel5Gain_t;

	//Convert to form for writing to PSOC memory
	solPanel5Gain_t = solPanel5Gain_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel5Gain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_5_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel6Offset(float solPanel6Offset_t)
{
	//Set the class private variable
	solPanel6Offset = solPanel6Offset_t;

	//Convert to form for writing to PSOC memory
	solPanel6Offset_t = solPanel6Offset_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel6Offset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_6_OFFSET_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel6Gain(float solPanel6Gain_t)
{
	//Set the class private variable
	solPanel6Gain = solPanel6Gain_t;

	//Convert to form for writing to PSOC memory
	solPanel6Gain_t = solPanel6Gain_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel6Gain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_6_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel7Offset(float solPanel7Offset_t)
{
	//Set the class private variable
	solPanel7Offset = solPanel7Offset_t;

	//Convert to form for writing to PSOC memory
	solPanel7Offset_t = solPanel7Offset_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel7Offset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_7_OFFSET_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel7Gain(float solPanel7Gain_t)
{
	//Set the class private variable
	solPanel7Gain = solPanel7Gain_t;

	//Convert to form for writing to PSOC memory
	solPanel7Gain_t = solPanel7Gain_t*MV_2_V;

	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel7Gain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_7_GAIN_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel8Offset(float solPanel8Offset_t)
{
	//Set the class private variable
	solPanel8Offset = solPanel8Offset_t;

	//Convert to form for writing to PSOC memory
	solPanel8Offset_t = solPanel8Offset_t*MV_2_V;
	
	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel8Offset_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_8_OFFSET_LBYTE, tx_buff);
}
void    PSOCComm::setSolPanel8Gain(float solPanel8Gain_t)
{
	//Set the class private variable
	solPanel8Gain = solPanel8Gain_t;

	//Convert to form for writing to PSOC memory
	solPanel8Gain_t = solPanel8Gain_t*MV_2_V;
	
	unsigned char tx_buff[TX_MAX_BUFF] = {0};

	//Store float into the tx_buff array as a high and low byte
	float2Chars(solPanel8Gain_t, tx_buff);

	//Write the value to the PSoC
	i2c0.write16(I2C_SOLPANEL_8_GAIN_LBYTE, tx_buff);
}

/*********GET FUNCTIONS**************/

bool    PSOCComm::getLoadRelay()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_LOAD_RELAY);

	//Assign to class variable
	loadRelay = retVal;
	return loadRelay;
}
bool    PSOCComm::getExternalRelay()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_EXT_RELAY);

	//Assign to class variable
	externalRelay = retVal;
	return externalRelay;
}
int8_t  PSOCComm::getTimeMinMaxAveMinutes()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_TIME_MINMAXAVE_MINUTES);

	//Assign to class variable
	timeMinMaxAveMinutes = retVal;
	return timeMinMaxAveMinutes;
}   
int8_t  PSOCComm::getTimeMinMaxAveSeconds()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_TIME_MINMAXAVE_SECONDS);

	//Assign to class variable
	timeMinMaxAveSeconds = retVal;
	return timeMinMaxAveSeconds;
}

//Battery Variables
float   PSOCComm::getBatteryVoltageGain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BATT_V_GAIN_LBYTE);

	//Convert the value to a float
	float batteryVoltageGain_t = (float) retVal;

	//Convert the float to the appropriate format
	batteryVoltageGain = batteryVoltageGain_t/MV_2_V;
	return batteryVoltageGain;
}
float   PSOCComm::getBatteryVoltageOffset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BATT_V_OFFSET_LBYTE);

	//Convert the value to a float
	float batteryVoltageOffset_t = (float) retVal;

	//Convert the float to the appropriate format
	batteryVoltageOffset = batteryVoltageOffset_t/MV_2_V;
	return batteryVoltageOffset;
}
float   PSOCComm::getBatteryCurrentGain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BATT_I_GAIN_LBYTE);

	//Convert the value to a float
	float batteryCurrentGain_t = (float) retVal;

	//Convert the float to the appropriate format
	batteryCurrentGain = batteryCurrentGain_t/M10A_2_A;
	return batteryCurrentGain;
}
float   PSOCComm::getBatteryCurrentOffset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BATT_I_OFFSET_LBYTE);

	//Convert the value to a float
	float batteryCurrentOffset_t = (float) retVal;

	//Convert the float to the appropriate format
	batteryCurrentOffset = batteryCurrentOffset_t/M10A_2_A;
	return batteryCurrentOffset;
}

//Load Variables
float   PSOCComm::getLoadVoltageGain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_LOAD_V_GAIN_LBYTE);

	//Convert the value to a float
	float loadVoltageGain_t = (float) retVal;

	//Convert the float to the appropriate format
	loadVoltageGain = loadVoltageGain_t/MV_2_V;
	return loadVoltageGain;
}
float   PSOCComm::getLoadVoltageOffset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_LOAD_V_OFFSET_LBYTE);

	//Convert the value to a float
	float loadVoltageOffset_t = (float) retVal;

	//Convert the float to the appropriate format
	loadVoltageOffset = loadVoltageOffset_t/MV_2_V;
	return loadVoltageOffset;
}
float   PSOCComm::getLoadCurrentGain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_LOAD_I_GAIN_LBYTE);

	//Convert the value to a float
	float loadCurrentGain_t = (float) retVal;

	//Convert the float to the appropriate format
	loadCurrentGain = loadCurrentGain_t/M10A_2_A;
	return loadCurrentGain;
}
float   PSOCComm::getLoadCurrentOffset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_LOAD_I_OFFSET_LBYTE);

	//Convert the value to a float
	float loadCurrentOffset_t = (float) retVal;

	//Convert the float to the appropriate format
	loadCurrentOffset = loadCurrentOffset_t/M10A_2_A;
	return loadCurrentOffset;
}

//External Variables
float   PSOCComm::getExternalVoltageGain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXT_V_GAIN_LBYTE);

	//Convert the value to a float
	float externalVoltageGain_t = (float) retVal;

	//Convert the float to the appropriate format
	externalVoltageGain = externalVoltageGain_t/MV_2_V;
	return externalVoltageGain;
}
float   PSOCComm::getExternalVoltageOffset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXT_V_OFFSET_LBYTE);

	//Convert the value to a float
	float externalVoltageOffset_t = (float) retVal;

	//Convert the float to the appropriate format
	externalVoltageOffset = externalVoltageOffset_t/MV_2_V;
	return externalVoltageOffset;
}
float   PSOCComm::getExternalCurrentGain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXT_I_GAIN_LBYTE);

	//Convert the value to a float
	float externalCurrentGain_t = (float) retVal;

	//Convert the float to the appropriate format
	externalCurrentGain = externalCurrentGain_t/M10A_2_A;
	return externalCurrentGain;
}
float   PSOCComm::getExternalCurrentOffset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXT_I_OFFSET_LBYTE);

	//Convert the value to a float
	float externalCurrentOffset_t = (float) retVal;

	//Convert the float to the appropriate format
	externalCurrentOffset = externalCurrentOffset_t/M10A_2_A;
	return externalCurrentOffset;
}

//Extra Variables
float   PSOCComm::getExtraVoltageGain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXTRA_V_GAIN_LBYTE);

	//Convert the value to a float
	float extraVoltageGain_t = (float) retVal;

	//Convert the float to the appropriate format
	extraVoltageGain = extraVoltageGain_t/MV_2_V;
	return extraVoltageGain;
}
float   PSOCComm::getExtraVoltageOffset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXTRA_V_OFFSET_LBYTE);

	//Convert the value to a float
	float extraVoltageOffset_t = (float) retVal;

	//Convert the float to the appropriate format
	extraVoltageOffset = extraVoltageOffset_t/MV_2_V;
	return extraVoltageOffset;
}

//BMS Variables
float   PSOCComm::getBmsLoadVoltageReconnect()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BMS_I2C_LOAD_V_RECONNECT_LBYTE);

	//Convert the value to a float
	float bmsLoadVoltageReconnect_t = (float) retVal;

	//Convert the float to the appropriate format
	bmsLoadVoltageReconnect = bmsLoadVoltageReconnect_t/MV_2_V;
	return bmsLoadVoltageReconnect;
}
uint8_t PSOCComm::getBmsLoadTimeActionDelay()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_BMS_LOAD_TIME_ACTIONDELAY_DISC_HBYTE);

	//Convert the value to a uint8
	uint8_t bmsLoadTimeActionDelay_t = (uint8_t) retVal;

	//Assign to class variable
	bmsLoadTimeActionDelay = bmsLoadTimeActionDelay_t;
	return bmsLoadTimeActionDelay;
}
float   PSOCComm::getBmsLoadVoltageDiscmin()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BMS_I2C_LOAD_V_DISCMIN_LBYTE);

	//Convert the value to a float
	float bmsLoadVoltageDiscmin_t = (float) retVal;

	//Convert the float to the appropriate format
	bmsLoadVoltageDiscmin = bmsLoadVoltageDiscmin_t/MV_2_V;
	return bmsLoadVoltageDiscmin;
}
uint8_t PSOCComm::getBmsBattTimeEmergDelay()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_BMS_BATT_TIME_EMERG_DELAY_DISC_HBYTE);

	//Convert the value to a uint8
	uint8_t bmsBattTimeEmergDelay_t = (uint8_t) retVal;

	//Assign to class variable
	bmsBattTimeEmergDelay = bmsBattTimeEmergDelay_t;
	return bmsBattTimeEmergDelay;
}
float   PSOCComm::getBmsBattVoltageEmergMinReconnect()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BMS_BATT_V_EMERGMIN_RECONNECT_LBYTE);

	//Convert the value to a float
	float bmsBattVoltageEmergMinReconnect_t = (float) retVal;

	//Convert the float to the appropriate format
	bmsBattVoltageEmergMinReconnect = bmsBattVoltageEmergMinReconnect_t/MV_2_V;
	return bmsBattVoltageEmergMinReconnect;
}
float   PSOCComm::getBmsBattVoltageEmergMinDisconnect()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BMS_BATT_V_EMERGMIN_LBYTE);

	//Convert the value to a float
	float bmsBattVoltageEmergMinDisconnect_t = (float) retVal;

	//Convert the float to the appropriate format
	bmsBattVoltageEmergMinDisconnect = bmsBattVoltageEmergMinDisconnect_t/MV_2_V;
	return bmsBattVoltageEmergMinDisconnect;
}
float   PSOCComm::getBmsBattVoltageCriticalWarning()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BMS_BATT_V_CRITICAL_WARNING_LBYTE);

	//Convert the value to a float
	float bmsBattVoltageCriticalWarning_t = (float) retVal;

	//Convert the float to the appropriate format
	bmsBattVoltageCriticalWarning = bmsBattVoltageCriticalWarning_t/MV_2_V;
	return bmsBattVoltageCriticalWarning;
}

//Real Time Clock Variables
int16_t PSOCComm::getRtcYear()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read8(I2C_RTC_MONTH);

	//Assign to class variable
	rtcYear = retVal;
	return rtcYear;
}
uint8_t PSOCComm::getRtcMonth()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_RTC_MONTH);

	//Convert the value to a uint8
	uint8_t rtcMonth_t = (uint8_t) retVal;

	//Assign to class variable
	rtcMonth = rtcMonth_t;
	return rtcMonth;
}
uint8_t PSOCComm::getRtcDay()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_RTC_DAYOFMONTH);

	//Convert the value to a uint8
	uint8_t rtcDay_t = (uint8_t) retVal;

	//Assign to class variable
	rtcDay = rtcDay_t;
	return rtcDay;
}
uint8_t PSOCComm::getRtcHour()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_RTC_HOUR);

	//Convert the value to a uint8
	uint8_t rtcHour_t = (uint8_t) retVal;

	//Assign to class variable
	rtcHour = rtcHour_t;
	return rtcHour;
}
uint8_t PSOCComm::getRtcMinute()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_RTC_MINUTE);

	//Convert the value to a uint8
	uint8_t rtcMinute_t = (uint8_t) retVal;

	//Assign to class variable
	rtcMinute = rtcMinute_t;
	return rtcMinute;
}
uint8_t PSOCComm::getRtcSecond()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_RTC_SECOND);

	//Convert the value to a uint8
	uint8_t rtcSecond_t = (uint8_t) retVal;

	//Assign to class variable
	rtcSecond = rtcSecond_t;
	return rtcSecond;
}
uint8_t PSOCComm::getRtcChangesFlag()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_RTC_CHANGES_FLAG);

	//Convert the value to a uint8
	uint8_t rtcChangesFlag_t = (uint8_t) retVal;

	//Assign to class variable
	rtcChangesFlag = rtcChangesFlag_t;
	return rtcChangesFlag;
}

//Solar Panel Offset and Gain Variables
float   PSOCComm::getSolPanel1Offset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_1_OFFSET_LBYTE);

	//Convert the value to a float
	float solPanel1Offset_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel1Offset = solPanel1Offset_t/MV_2_V;
	return solPanel1Offset;
}
float   PSOCComm::getSolPanel1Gain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_1_GAIN_LBYTE);

	//Convert the value to a float
	float solPanel1Gain_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel1Gain = solPanel1Gain_t/MV_2_V;
	return solPanel1Gain;
}
float   PSOCComm::getSolPanel2Offset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_2_OFFSET_LBYTE);

	//Convert the value to a float
	float solPanel2Offset_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel2Offset = solPanel2Offset_t/MV_2_V;
	return solPanel2Offset;
}
float   PSOCComm::getSolPanel2Gain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_2_GAIN_LBYTE);

	//Convert the value to a float
	float solPanel2Gain_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel2Gain = solPanel2Gain_t/MV_2_V;
	return solPanel2Gain;
}
float   PSOCComm::getSolPanel3Offset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_3_OFFSET_LBYTE);

	//Convert the value to a float
	float solPanel3Offset_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel3Offset = solPanel3Offset_t/MV_2_V;
	return solPanel3Offset;
}
float   PSOCComm::getSolPanel3Gain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_3_GAIN_LBYTE);

	//Convert the value to a float
	float solPanel3Gain_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel3Gain = solPanel3Gain_t/MV_2_V;
	return solPanel3Gain;
}
float   PSOCComm::getSolPanel4Offset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_4_OFFSET_LBYTE);

	//Convert the value to a float
	float solPanel4Offset_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel4Offset = solPanel4Offset_t/MV_2_V;
	return solPanel4Offset;
}
float   PSOCComm::getSolPanel4Gain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_4_GAIN_LBYTE);

	//Convert the value to a float
	float solPanel4Gain_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel4Gain = solPanel4Gain_t/MV_2_V;
	return solPanel4Gain;
}
float   PSOCComm::getSolPanel5Offset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_5_OFFSET_LBYTE);

	//Convert the value to a float
	float solPanel5Offset_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel5Offset = solPanel5Offset_t/MV_2_V;
	return solPanel5Offset;
}
float   PSOCComm::getSolPanel5Gain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_5_GAIN_LBYTE);

	//Convert the value to a float
	float solPanel5Gain_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel5Gain = solPanel5Gain_t/MV_2_V;
	return solPanel5Gain;
}
float   PSOCComm::getSolPanel6Offset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_6_OFFSET_LBYTE);

	//Convert the value to a float
	float solPanel6Offset_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel6Offset = solPanel6Offset_t/MV_2_V;
	return solPanel6Offset;
}
float   PSOCComm::getSolPanel6Gain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_6_GAIN_LBYTE);

	//Convert the value to a float
	float solPanel6Gain_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel6Gain = solPanel6Gain_t/MV_2_V;
	return solPanel6Gain;
}
float   PSOCComm::getSolPanel7Offset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_7_OFFSET_LBYTE);

	//Convert the value to a float
	float solPanel7Offset_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel7Offset = solPanel7Offset_t/MV_2_V;
	return solPanel7Offset;
}
float   PSOCComm::getSolPanel7Gain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_7_GAIN_LBYTE);

	//Convert the value to a float
	float solPanel7Gain_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel7Gain = solPanel7Gain_t/MV_2_V;
	return solPanel7Gain;
}
float   PSOCComm::getSolPanel8Offset()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_8_OFFSET_LBYTE);

	//Convert the value to a float
	float solPanel8Offset_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel8Offset = solPanel8Offset_t/MV_2_V;
	return solPanel8Offset;
}
float   PSOCComm::getSolPanel8Gain()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLPANEL_8_GAIN_LBYTE);

	//Convert the value to a float
	float solPanel8Gain_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel8Gain = solPanel8Gain_t/MV_2_V;
	return solPanel8Gain;
}

//Real time clock variable
uint8_t PSOCComm::getRtcSecondsLeftAverage()
{
	//Read the 8 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_RTC_SECONDSLEFT_AVERAGE);

	//Convert the value to a uint8
	uint8_t rtcSecondsLeftAverage_t = (uint8_t) retVal;

	//Assign to class variable
	rtcSecondsLeftAverage = rtcSecondsLeftAverage_t;
	return rtcSecondsLeftAverage;
}

//BMS state variable
uint8_t PSOCComm::getBmsState()
{
	//Read the 18 bit value in memory from the PSoC
	int8_t retVal = i2c0.read8(I2C_BMS_STATE);

	//Convert the value to a uint8
	uint8_t bmsState_t = (uint8_t) retVal;

	//Assign to class variable
	bmsState = bmsState_t;
	return bmsState;
}

//Solar Panel 8 Variables
float   PSOCComm::getSolPanel8Average()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_8_AVE_LBYTE);

	//Convert the value to a float
	float solPanel8Average_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel8Average = solPanel8Average_t/MV_2_V;
	return solPanel8Average;
}
float   PSOCComm::getSolPanel8Min()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_8_MIN_LBYTE);

	//Convert the value to a float
	float solPanel8Max_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel8Max = solPanel8Max_t/MV_2_V;
	return solPanel8Min;
}
float   PSOCComm::getSolPanel8Max()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_8_MAX_LBYTE);

	//Convert the value to a float
	float solPanel8Max_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel8Max = solPanel8Max_t/MV_2_V;
	return solPanel8Max;
}
float   PSOCComm::getSolPanel8()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_8_LBYTE);

	//Convert the value to a float
	float solPanel8_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel8 = solPanel8_t/MV_2_V;
	return solPanel8;
}

//Solar Panel 7 Variables
float   PSOCComm::getSolPanel7Average()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_7_AVE_LBYTE);

	//Convert the value to a float
	float solPanel7Average_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel7Average = solPanel7Average_t/MV_2_V;
	return solPanel7Average;
}
float   PSOCComm::getSolPanel7Min()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_7_MIN_LBYTE);

	//Convert the value to a float
	float solPanel7Min_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel7Min = solPanel7Min_t/MV_2_V;
	return solPanel7Min;
}
float   PSOCComm::getSolPanel7Max()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_7_MAX_LBYTE);

	//Convert the value to a float
	float solPanel7Max_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel7Max = solPanel7Max_t/MV_2_V;
	return solPanel7Max;
}
float   PSOCComm::getSolPanel7()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_7_LBYTE);

	//Convert the value to a float
	float solPanel7_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel7 = solPanel7_t/MV_2_V;
	return solPanel7;
}

//Solar Panel 6 Variables
float   PSOCComm::getSolPanel6Average()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_6_AVE_LBYTE);

	//Convert the value to a float
	float solPanel6Average_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel6Average = solPanel6Average_t/MV_2_V;
	return solPanel6Average;
}
float   PSOCComm::getSolPanel6Min()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_6_MIN_LBYTE);

	//Convert the value to a float
	float solPanel6Min_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel6Min = solPanel6Min_t/MV_2_V;
	return solPanel6Min;
}
float   PSOCComm::getSolPanel6Max()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_6_MAX_LBYTE);

	//Convert the value to a float
	float solPanel6Max_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel6Max = solPanel6Max_t/MV_2_V;
	return solPanel6Max;
}
float   PSOCComm::getSolPanel6()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_6_LBYTE);

	//Convert the value to a float
	float solPanel6_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel6 = solPanel6_t/MV_2_V;
	return solPanel6;
}

//Solar Panel 5 Variables
float   PSOCComm::getSolPanel5Average()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_5_AVE_LBYTE);

	//Convert the value to a float
	float solPanel5Average_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel5Average = solPanel5Average_t/MV_2_V;
	return solPanel5Average;
}
float   PSOCComm::getSolPanel5Min()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_5_MIN_LBYTE);

	//Convert the value to a float
	float solPanel5Min_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel5Min = solPanel5Min_t/MV_2_V;
	return solPanel5Min;
}
float   PSOCComm::getSolPanel5Max()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_5_MAX_LBYTE);

	//Convert the value to a float
	float solPanel5Max_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel5Max = solPanel5Max_t/MV_2_V;
	return solPanel5Max;
}
float   PSOCComm::getSolPanel5()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_5_LBYTE);

	//Convert the value to a float
	float solPanel5_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel5 = solPanel5_t/MV_2_V;
	return solPanel5;
}

//Solar Panel 4 Variables
float   PSOCComm::getSolPanel4Average()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_4_AVE_LBYTE);

	//Convert the value to a float
	float solPanel4Average_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel4Average = solPanel4Average_t/MV_2_V;
	return solPanel4Average;
}
float   PSOCComm::getSolPanel4Min()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_4_MIN_LBYTE);

	//Convert the value to a float
	float solPanel4Min_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel4Min = solPanel4Min_t/MV_2_V;
	return solPanel4Min;
}
float   PSOCComm::getSolPanel4Max()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_4_MAX_LBYTE);

	//Convert the value to a float
	float solPanel4Max_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel4Max = solPanel4Max_t/MV_2_V;
	return solPanel4Max;
}
float   PSOCComm::getSolPanel4()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_4_LBYTE);

	//Convert the value to a float
	float solPanel4_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel4 = solPanel4_t/MV_2_V;
	return solPanel4;
}

//Solar Panel 3 Variables
float   PSOCComm::getSolPanel3Average()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_3_AVE_LBYTE);

	//Convert the value to a float
	float solPanel3Average_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel3Average = solPanel3Average_t/MV_2_V;
	return solPanel3Average;
}
float   PSOCComm::getSolPanel3Min()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_3_MIN_LBYTE);

	//Convert the value to a float
	float solPanel3Min_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel3Min = solPanel3Min_t/MV_2_V;
	return solPanel3Min;
}
float   PSOCComm::getSolPanel3Max()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_3_MAX_LBYTE);

	//Convert the value to a float
	float solPanel3Max_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel3Max = solPanel3Max_t/MV_2_V;
	return solPanel3Max;
}
float   PSOCComm::getSolPanel3()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_3_LBYTE);

	//Convert the value to a float
	float solPanel3_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel3 = solPanel3_t/MV_2_V;
	return solPanel3;
}

//Solar Panel 2 Variables
float   PSOCComm::getSolPanel2Average()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_2_AVE_LBYTE);

	//Convert the value to a float
	float solPanel2Average_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel2Average = solPanel2Average_t/MV_2_V;
	return solPanel2Average;
}
float   PSOCComm::getSolPanel2Min()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_2_MIN_LBYTE);

	//Convert the value to a float
	float solPanel2Min_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel2Min = solPanel2Min_t/MV_2_V;
	return solPanel2Min;
}
float   PSOCComm::getSolPanel2Max()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_2_MAX_LBYTE);

	//Convert the value to a float
	float solPanel2Max_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel2Max = solPanel2Max_t/MV_2_V;
	return solPanel2Max;
}
float   PSOCComm::getSolPanel2()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_2_LBYTE);

	//Convert the value to a float
	float solPanel2_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel2 = solPanel2_t/MV_2_V;
	return solPanel2;
}

//Solar Panel 1 Variables
float   PSOCComm::getSolPanel1Average()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_1_AVE_LBYTE);

	//Convert the value to a float
	float solPanel1Average_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel1Average = solPanel1Average_t/MV_2_V;
	return solPanel1Average;
}
float   PSOCComm::getSolPanel1Min()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_1_MIN_LBYTE);

	//Convert the value to a float
	float solPanel1Min_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel1Min = solPanel1Min_t/MV_2_V;
	return solPanel1Min;
}
float   PSOCComm::getSolPanel1Max()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_1_MAX_LBYTE);

	//Convert the value to a float
	float solPanel1Max_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel1Max = solPanel1Max_t/MV_2_V;
	return solPanel1Max;
}
float   PSOCComm::getSolPanel1()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_SOLARPANEL_1_LBYTE);

	//Convert the value to a float
	float solPanel1_t = (float) retVal;

	//Convert the float to the appropriate format
	solPanel1 = solPanel1_t/MV_2_V;
	return solPanel1;
}

//Battery, Load, External, and Extra Variables
float   PSOCComm::getBatteryVoltageAverage()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BATT_V_AVE_LBYTE);

	//Convert the value to a float
	float batteryVoltageAverage_t = (float) retVal;

	//Convert the float to the appropriate format
	batteryVoltageAverage = batteryVoltageAverage_t/MV_2_V;
	return batteryVoltageAverage;
}
float   PSOCComm::getBatteryVoltageMin()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BATT_V_MIN_LBYTE);

	//Convert the value to a float
	float batteryVoltageMin_t = (float) retVal;

	//Convert the float to the appropriate format
	batteryVoltageMin = batteryVoltageMin_t/MV_2_V;
	return batteryVoltageMin;
}
float   PSOCComm::getBatteryVoltageMax()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BATT_V_MAX_LBYTE);

	//Convert the value to a float
	float batteryVoltageMax_t = (float) retVal;

	//Convert the float to the appropriate format
	batteryVoltageMax = batteryVoltageMax_t/MV_2_V;
	return batteryVoltageMax;
}

float   PSOCComm::getBatteryCurrentAverage()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BATT_I_AVE_LBYTE);

	//Convert the value to a float
	float batteryCurrentAverage_t = (float) retVal;

	//Convert the float to the appropriate format
	batteryCurrentAverage = batteryCurrentAverage_t/M10A_2_A;
	return batteryCurrentAverage;
}
float   PSOCComm::getBatteryCurrentMin()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BATT_I_MIN_LBYTE);

	//Convert the value to a float
	float batteryCurrentMin_t = (float) retVal;

	//Convert the float to the appropriate format
	batteryCurrentMin = batteryCurrentMin_t/M10A_2_A;
	return batteryCurrentMin;
}
float   PSOCComm::getBatteryCurrentMax()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_BATT_I_MAX_LBYTE);

	//Convert the value to a float
	float batteryCurrentMax_t = (float) retVal;

	//Convert the float to the appropriate format
	batteryCurrentMax = batteryCurrentMax_t/M10A_2_A;
	return batteryCurrentMax;
}

float   PSOCComm::getLoadVoltageAverage()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_LOAD_V_AVE_LBYTE);

	//Convert the value to a float
	float loadVoltageAverage_t = (float) retVal;

	//Convert the float to the appropriate format
	loadVoltageAverage = loadVoltageAverage_t/MV_2_V;
	return loadVoltageAverage;
}
float   PSOCComm::getLoadVoltageMin()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_LOAD_V_MIN_LBYTE);

	//Convert the value to a float
	float loadVoltageMin_t = (float) retVal;

	//Convert the float to the appropriate format
	loadVoltageMin = loadVoltageMin_t/MV_2_V;
	return loadVoltageMin;
}
float   PSOCComm::getLoadVoltageMax()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_LOAD_V_MAX_LBYTE);

	//Convert the value to a float
	float loadVoltageMax_t = (float) retVal;

	//Convert the float to the appropriate format
	loadVoltageMax = loadVoltageMax_t/MV_2_V;
	return loadVoltageMax;
}

float   PSOCComm::getLoadCurrentAverage()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_LOAD_I_AVE_LBYTE);

	//Convert the value to a float
	float loadCurrentAverage_t = (float) retVal;

	//Convert the float to the appropriate format
	loadCurrentAverage = loadCurrentAverage_t/M10A_2_A;
	return loadCurrentAverage;
}
float   PSOCComm::getLoadCurrentMin()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_LOAD_I_MIN_LBYTE);

	//Convert the value to a float
	float loadCurrentMin_t = (float) retVal;

	//Convert the float to the appropriate format
	loadCurrentMin = loadCurrentMin_t/M10A_2_A;
	return loadCurrentMin;
}
float   PSOCComm::getLoadCurrentMax()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_LOAD_I_MAX_LBYTE);

	//Convert the value to a float
	float loadCurrentMax_t = (float) retVal;

	//Convert the float to the appropriate format
	loadCurrentMax = loadCurrentMax_t/M10A_2_A;
	return loadCurrentMax;
}

float   PSOCComm::getExternalVoltageAverage()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXT_V_AVE_LBYTE);

	//Convert the value to a float
	float externalVoltageAverage_t = (float) retVal;

	//Convert the float to the appropriate format
	externalVoltageAverage = externalVoltageAverage_t/MV_2_V;
	return externalVoltageAverage;
}
float   PSOCComm::getExternalVoltageMin()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXT_V_MIN_LBYTE);

	//Convert the value to a float
	float externalVoltageMin_t = (float) retVal;

	//Convert the float to the appropriate format
	externalVoltageMin = externalVoltageMin_t/MV_2_V;
	return externalVoltageMin;
}
float   PSOCComm::getExternalVoltageMax()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXT_V_MAX_LBYTE);

	//Convert the value to a float
	float externalVoltageMax_t = (float) retVal;

	//Convert the float to the appropriate format
	externalVoltageMax = externalVoltageMax_t/MV_2_V;
	return externalVoltageMax;
}

float   PSOCComm::getExternalCurrentAverage()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXT_I_AVE_LBYTE);

	//Convert the value to a float
	float externalCurrentAverage_t = (float) retVal;

	//Convert the float to the appropriate format
	externalCurrentAverage = externalCurrentAverage_t/M10A_2_A;
	return externalCurrentAverage;
}
float   PSOCComm::getExternalCurrentMin()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXT_I_MIN_LBYTE);

	//Convert the value to a float
	float externalCurrentMin_t = (float) retVal;

	//Convert the float to the appropriate format
	externalCurrentMin = externalCurrentMin_t/M10A_2_A;
	return externalCurrentMin;
}
float   PSOCComm::getExternalCurrentMax()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXT_I_MAX_LBYTE);

	//Convert the value to a float
	float externalCurrentMax_t = (float) retVal;

	//Convert the float to the appropriate format
	externalCurrentMax = externalCurrentMax_t/M10A_2_A;
	return externalCurrentMax;
}

float   PSOCComm::getExtraVoltageAverage()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXTRA_V_AVE_LBYTE);

	//Convert the value to a float
	float extraVoltageAverage_t = (float) retVal;

	//Convert the float to the appropriate format
	extraVoltageAverage = extraVoltageAverage_t/MV_2_V;
	return extraVoltageAverage;
}
float   PSOCComm::getExtraVoltageMin()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXTRA_V_MIN_LBYTE);

	//Convert the value to a float
	float extraVoltageMin_t = (float) retVal;

	//Convert the float to the appropriate format
	extraVoltageMin = extraVoltageMin_t/MV_2_V;
	return extraVoltageMin;
}
float   PSOCComm::getExtraVoltageMax()
{
	//Read the 16 bit value in memory from the PSoC
	int16_t retVal = i2c0.read16(I2C_EXTRA_V_MAX_LBYTE);

	//Convert the value to a float
	float extraVoltageMax_t = (float) retVal;

	//Convert the float to the appropriate format
	extraVoltageMax = extraVoltageMax_t/MV_2_V;
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
