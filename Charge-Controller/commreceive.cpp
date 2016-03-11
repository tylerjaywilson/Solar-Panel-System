/*
* Authors: Tyler Wilson
* Date: 02/24/2016
* Notes: This class handles the reception commands from the MPPT-3000 RS232 Charge Controller
* All of the charge controller information can be accessed through the 'get' functions for each charge controller parameter
* 
*/
#include <string>
#include "commreceive.hpp"


//Default Constructor
CommReceive::CommReceive()
{

	rx_buffer[0] = {0};
	
	/* Serial number */
	serialNum = "-1";

  	/* QPIRI - Device rated information paramters */
	maxOutputPower = "-1";
	nominalBattVoltage = "-1";
	nominalChargingCurrent = "-1";
	absorptionVoltage = "-1";
	floatVoltage = "-1";
	battType = "-1";
	remoteBattVoltageDetect = "-1";
	battTempCompensation = "-1";
	remoteTempDetect = "-1";
	battRatedVoltageSet = "-1";
	battInSerial = "-1";
	battLowWarningVoltage = "-1";
	battLowShutdownDetect = "-1";

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

//Parse the Device serial number
void CommReceive::parseQID(char rx_buffer_t[256])
{

}
//Parse the device rating information
void CommReceive::parseQPIRI(char rx_buffer_t[256])
{

}
//Parse the device general status information
void CommReceive::parseQPIGS(char rx_buffer_t[256])
{

}
//Parse the device warning status information
void CommReceive::parseQPIWS(char rx_buffer_t[256])
{

}
//Parse the battery equalized information
void CommReceive::parseQBEQI(char rx_buffer_t[256])
{

}

//Return the serial number
std::string CommReceive::getSerialNum()
{
	return serialNum;
}
