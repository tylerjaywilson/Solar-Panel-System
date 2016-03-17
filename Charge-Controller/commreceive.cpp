/*
* Authors: Tyler Wilson
* Date: 02/24/2016
* Notes: This class handles the reception commands from the MPPT-3000 RS232 Charge Controller
* All of the charge controller information can be accessed through the 'get' functions for each charge controller parameter
* 
*/
#include <string>
#include <stdint.h>
#include "commreceive.hpp"

#define RX_LENGTH_MAX 256
#define IDLENGTH 14
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
	//reserved = -1.0;
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
	battEqualizeddVoltage = -1.0;
	battCVChargeTime = -1;
	battEqualizedTimeout = -1;
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
	return battEqualizeddVoltage;
}
int CommReceive::getbattCVChargeTime()
{
	return battCVChargeTime;
}
int CommReceive::getbattEqualizedTimeout()
{
	return battEqualizedTimeout;
}

//Parse the Device serial number
void CommReceive::parseQID(unsigned char rx_buffer_t[RX_LENGTH_MAX])
{
	std::string id(rx_buffer_t, rx_buffer_t + IDLENGTH);
	serialNum = id;
}
//Parse the device rating information
void CommReceive::parseQPIRI(unsigned char rx_buffer_t[RX_LENGTH_MAX])
{	

}
//Parse the device general status information
void CommReceive::parseQPIGS(unsigned char rx_buffer_t[RX_LENGTH_MAX])
{

}
//Parse the device warning status information
void CommReceive::parseQPIWS(unsigned char rx_buffer_t[RX_LENGTH_MAX])
{

}
//Parse the battery equalized information
void CommReceive::parseQBEQI(unsigned char rx_buffer_t[RX_LENGTH_MAX])
{

}