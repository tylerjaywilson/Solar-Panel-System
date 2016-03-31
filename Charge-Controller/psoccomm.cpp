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

//Default Constructor
PSOCComm::PSOCComm()
{
	i2c0.init();	//Connect the PSoC to the RaPi2 
	batteryVoltage 	= 0.0;
	batteryCurrent = 0.0;
	loadVoltage = 0.0;
	loadCurrent = 0.0;
	externalVoltage = 0.0;
	externalCurrent = 0.0;
	extraVoltage = 0.0;
}

/**********SET FUNCTIONS*************/
void    PSOCComm::setLoadRelay(bool)
{
	
}
void    PSOCComm::setExternalRelay(bool)
{
	
}
void    PSOCComm::setTimeMinMaxAveMinutes(int8_t)
{
	
}   
void    PSOCComm::setTimeMinMaxAveSeconds(int8_t)
{
	
}

  //Battery Variables
void    PSOCComm::setBatteryVoltageGain(float)
{
	
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

float PSOCComm::getBatteryVoltage()
{
	//Read the battery voltage level, convert it to a float, and do the appropriate arithmetic to convert to Volt form (instead of mV)
	int16_t retVal = i2c0.read16(I2C_BATT_VOLTAGE_LBYTE);
	batteryVoltage = (float) retVal;
	return (batteryVoltage/MV_2_V);
}
float PSOCComm::getBatteryCurrent()
{
	int16_t retVal = i2c0.read16(I2C_BATT_CURRENT_LBYTE);
	batteryCurrent = (float) retVal;
	return (batteryCurrent/M10A_2_A);
}
float PSOCComm::getLoadVoltage()
{
	int16_t retVal = i2c0.read16(I2C_LOAD_VOLTAGE_LBYTE);
	loadVoltage = (float) retVal;
	return (loadVoltage/MV_2_V);
}
float PSOCComm::getLoadCurrent()
{
	int16_t retVal = i2c0.read16(I2C_LOAD_CURRENT_LBYTE);
	loadCurrent = (float) retVal;
	return (loadCurrent/M10A_2_A);
}
float PSOCComm::getExternalVoltage()
{
	int16_t retVal = i2c0.read16(I2C_EXT_VOLTAGE_LBYTE);
	externalVoltage = (float) retVal;
	return (externalVoltage/MV_2_V);
}
float PSOCComm::getExternalCurrent()
{
	int16_t retVal = i2c0.read16(I2C_EXT_CURRENT_LBYTE);
	externalCurrent = (float) retVal;
	return (externalCurrent/M10A_2_A);
}
float PSOCComm::getExtraVoltage()
{
	int16_t retVal = i2c0.read16(I2C_EXTRA_VOLTAGE_LBYTE);
	extraVoltage = (float) retVal;
	return (extraVoltage/MV_2_V);
}  	
