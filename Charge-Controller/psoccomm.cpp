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

//Default Constructor
PSOCComm::PSOCComm()
{
	i2c0.init();	//Connect the PSoC to the RaPi2 
	batteryVoltage = 0.0;
	batteryCurrent = 0.0;
	loadVoltage = 0.0;
	loadCurrent = 0.0;
	externalVoltage = 0.0;
	externalCurrent = 0.0;
	extraVoltage = 0.0;
}

/**********SET FUNCTIONS*************/

void PSOCComm::setBatteryVoltage(float battVolt)
{
	batteryVoltage = battVolt;
}
void PSOCComm::setBatteryCurrent(float battCurr)
{
	batteryCurrent = battCurr;	
}
void PSOCComm::setLoadVoltage(float loadVolt)
{
	loadVoltage = loadVolt;
}
void PSOCComm::setLoadCurrent(float loadCurr)
{
	loadCurrent = loadCurr;
}
void PSOCComm::setExternalVoltage(float extVolt)
{
	externalVoltage = extVolt;
}
void PSOCComm::setExternalCurrent(float extCurr)
{
	externalCurrent = extCurr;
}
void PSOCComm::setExtraVoltage(float extraVolt)
{
	extraVoltage = extraVolt;
}

/*********GET FUNCTIONS**************/

float PSOCComm::getBatteryVoltage()
{
	uint16_t retVal = i2c0.read16(I2C_BATT_VOLTAGE_LBYTE);
	batteryVoltage = (float) retVal;
	return batteryVoltage;
}
float PSOCComm::getBatteryCurrent()
{
	uint16_t retVal = i2c0.read16(I2C_BATT_CURRENT_LBYTE);
	batteryCurrent = (float) retVal;
	return batteryCurrent;
}
float PSOCComm::getLoadVoltage()
{
	uint16_t retVal = i2c0.read16(I2C_LOAD_VOLTAGE_LBYTE);
	loadVoltage = (float) retVal;
	return loadVoltage;
}
float PSOCComm::getLoadCurrent()
{
	uint16_t retVal = i2c0.read16(I2C_LOAD_CURRENT_LBYTE);
	loadCurrent = (float) retVal;
	return loadCurrent;
}
float PSOCComm::getExternalVoltage()
{
	uint16_t retVal = i2c0.read16(I2C_LOAD_CURRENT_LBYTE);
	externalVoltage = (float) retVal;
	return externalVoltage;
}
float PSOCComm::getExternalCurrent()
{
	uint16_t retVal = i2c0.read16(I2C_EXT_CURRENT_LBYTE);
	externalCurrent = (float) retVal;
	return externalCurrent;
}
float PSOCComm::getExtraVoltage()
{
	uint16_t retVal = i2c0.read16(I2C_EXTRA_VOLTAGE_LBYTE);
	extraVoltage = (float) retVal;
	return extraVoltage;
}
  	