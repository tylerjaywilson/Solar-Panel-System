/* Author: Tyler Wilson
* Date: 03/24/2016
* Notes: This class handles the communication between the raspberry pi 2 and the psoc5
*
*/
#include <stdio.h>
#include <stdint.h>
#include "i2c.hpp"
#include "i2cdefines.hpp"
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

void PSOCComm::writei2c(int writeType)
{
	switch(writeType)
	{
		default:
			printf("Error with write request!\n");
	}
}

void PSOCComm::readi2c(int readType)
{	
	int retVal;

	switch(readType)
	{
		case I2C_BATT_VOLTAGE_LBYTE:
			retVal = i2c0.read16(I2C_BATT_VOLTAGE_LBYTE);
			setBatteryVoltage((float)retVal);
			break;
		case I2C_BATT_CURRENT_LBYTE:
			retVal = i2c0.read16(I2C_BATT_CURRENT_LBYTE);
			setBatteryCurrent((float)retVal);
			break;
		case I2C_LOAD_VOLTAGE_LBYTE:
			retVal = i2c0.read16(I2C_LOAD_VOLTAGE_LBYTE);
			setLoadVoltage((float)retVal);
			break;
		case I2C_LOAD_CURRENT_LBYTE:
			retVal = i2c0.read16(I2C_LOAD_CURRENT_LBYTE);
			setLoadCurrent((float)retVal);
			break;
		case I2C_EXT_VOLTAGE_LBYTE:
			retVal = i2c0.read16(I2C_EXT_VOLTAGE_LBYTE);
			setExternalVoltage((float)retVal);
			break;
		case I2C_EXT_CURRENT_LBYTE:
			retVal = i2c0.read16(I2C_EXT_CURRENT_LBYTE);
			setExternalCurrent((float)retVal);
			break;
		case I2C_EXTRA_VOLTAGE_LBYTE:
			retVal = i2c0.read16(I2C_EXTRA_VOLTAGE_LBYTE);
			setExtraVoltage((float)retVal);
			break;
		default:
			printf("Error with write request!\n");
	}
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
	readi2c(I2C_BATT_VOLTAGE_LBYTE);
}
float PSOCComm::getBatteryCurrent()
{
	readi2c(I2C_BATT_CURRENT_LBYTE);
}
float PSOCComm::getLoadVoltage()
{
	readi2c(I2C_LOAD_VOLTAGE_LBYTE);
}
float PSOCComm::getLoadCurrent()
{
	readi2c(I2C_LOAD_CURRENT_LBYTE);
}
float PSOCComm::getExternalVoltage()
{
	readi2c(I2C_EXT_VOLTAGE_LBYTE);
}
float PSOCComm::getExternalCurrent()
{
	readi2c(I2C_EXT_CURRENT_LBYTE);
}
float PSOCComm::getExtraVoltage()
{
	readi2c(I2C_EXTRA_VOLTAGE_LBYTE);
}
  	