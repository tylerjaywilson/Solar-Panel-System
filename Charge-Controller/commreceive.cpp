/*
* Authors: Tyler Wilson
* Date: 02/24/2016
* Notes: This class handles the reception commands from the MPPT-3000 RS232 Charge Controller
* All of the charge controller information can be accessed through the 'get' functions for each charge controller parameter
* 
*/
#include <string>
#include "commreceive.hpp"

#define RX_LENGTH_MAX 256

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
	nominalChargingCurrent = -1;
	absorptionVoltage = -1;
	floatVoltage = -1;
	battType = -1;
	remoteBattVoltageDetect = -1;
	battTempCompensation = "-1";
	remoteTempDetect = -1;
	battRatedVoltageSet = -1;
	battInSerial = -1;
	battLowWarningVoltage = -1;
	battLowShutdownDetect = -1;

	/* QPIGS - Device general status parameters */
	pvInputVoltage = "-1";
	battVoltage = "-1";
	chargingCurrent = "-1";
	chargingCurrent1 = "-1";
	chargingCurrent2 = "-1";
	chargingPower = "-1";
	unitTemp = "-1";
	remoteBattVoltage = "-1";
	remoteBattTemp = "-1";
	//reserved = "-1";
	status = "-1";

	/* QPIWS - Device warning status paramters */
	overChargeCurrent = "-1";
	overTemp = "-1";
	battVoltageUnder = "-1";
	battVoltageHigh = "-1";
	pvHighLoss = "-1";
	battTempLow = "-1";
	battTempHigh = "-1";
	pvLowLoss = "-1";
	pvHighDerating = "-1";
	tempHighDerating = "-1";
	battTempLowAlarm = "-1";
	battLowWarning = "-1";

	/* QBEQI - Battery equalized information */
	battEqualizedEn = "-1";
	battEqualizedTime = "-1";
	intervalTime = "-1";
	maxCurrent = "-1";
	remainingTime = "-1";
	battEqualizeddVoltage = "-1";
	battCVChargeTime = "-1";
	battEqualizedTimeout = "-1";
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
int CommReceive::getnominalChargingCurrent()
{
	return nominalChargingCurrent;
}
int CommReceive::getabsorptionVoltage()
{
	return absorptionVoltage;
}
int CommReceive::getfloatVoltage()
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
std::string CommReceive::getbattTempCompensation()
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
int CommReceive::getbattLowWarningVoltage()
{
	return battLowWarningVoltage;
}
int CommReceive::getbattLowShutdownDetect()
{
	return battLowShutdownDetect;
}
std::string CommReceive::getpvInputVoltage()
{
	return pvInputVoltage;
}
std::string CommReceive::getbattVoltage()
{
	return battVoltage;
}
std::string CommReceive::getchargingCurrent()
{
	return chargingCurrent;
}
std::string CommReceive::getchargingCurrent1()
{
	return chargingCurrent1;
}
std::string CommReceive::getchargingCurrent2()
{
	return chargingCurrent2;
}
std::string CommReceive::getchargingPower()
{
	return chargingPower;
}
std::string CommReceive::getunitTemp()
{
	return unitTemp;
}
std::string CommReceive::getremoteBattVoltage()
{
	return remoteBattVoltage;
}
std::string CommReceive::getremoteBattTemp()
{
	return remoteBattTemp;
}
std::string CommReceive::getstatus()
{
	return status;
}
std::string CommReceive::getoverChargeCurrent()
{
	return overChargeCurrent;
}
std::string CommReceive::getoverTemp()
{
	return overTemp;
}
std::string CommReceive::getbattVoltageUnder()
{
	return battVoltageUnder;
}
std::string CommReceive::getbattVoltageHigh()
{
	return battVoltageHigh;
}
std::string CommReceive::getpvHighLoss()
{
	return pvHighLoss;
}
std::string CommReceive::getbattTempLow()
{
	return battTempLow;
}
std::string CommReceive::getbattTempHigh()
{
	return battTempHigh;
}
std::string CommReceive::getpvLowLoss()
{
	return pvLowLoss;
}
std::string CommReceive::getpvHighDerating()
{
	return pvHighDerating;
}
std::string CommReceive::gettempHighDerating()
{
	return tempHighDerating;
}
std::string CommReceive::getbattTempLowAlarm()
{
	return battTempLowAlarm;
}
std::string CommReceive::getbattLowWarning()
{
	return battLowWarning;
}
std::string CommReceive::getbattEqualizedEn()
{
	return battEqualizedEn;
}
std::string CommReceive::getbattEqualizedTime()
{
	return battEqualizedTime;
}
std::string CommReceive::getintervalTime()
{
	return intervalTime;
}
std::string CommReceive::getmaxCurrent()
{
	return maxCurrent;
}
std::string CommReceive::getremainingTime()
{
	return remainingTime;
}
std::string CommReceive::getbattEqualizeddVoltage()
{
	return battEqualizeddVoltage;
}
std::string CommReceive::getbattCVChargeTime()
{
	return battCVChargeTime;
}
std::string CommReceive::getbattEqualizedTimeout()
{
	return battEqualizedTimeout;
}

//Parse the Device serial number
void CommReceive::parseQID(unsigned char rx_buffer_t[RX_LENGTH_MAX])
{
	char serialnumber[14];
	
	for (int i=0; i<14; i++)
	{
		serialnumber[i] = rx_buffer_t[i+1];
	}
	std::string snum(serialnumber, 14);
	serialNum = snum; //Set the new serial number.
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