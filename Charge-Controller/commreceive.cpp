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

//Return the serial number
std::string CommReceive::getSerialNum()
{
	return serialNum;
}
std::string CommReceive::getmaxOutputPower()
{
	return maxOutputPower;
}
std::string CommReceive::getnominalBattVoltage()
{
	return nominalBattVoltage;
}
std::string CommReceive::getnominalChargingCurrent()
{
	return nominalChargingCurrent;
}
std::string CommReceive::getabsorptionVoltage()
{
	return absorptionVoltage;
}
std::string CommReceive::getfloatVoltage()
{
	return floatVoltage;
}
std::string CommReceive::getbattType()
{
	return battType;
}
std::string CommReceive::getremoteBattVoltageDetect()
{
	return remoteBattVoltageDetect;
}
std::string CommReceive::getbattTempCompensation()
{
	return battTempCompensation;
}
std::string CommReceive::getremoteTempDetect()
{
	return remoteTempDetect;
}
std::string CommReceive::getbattRatedVoltageSet()
{
	return battRatedVoltageSet;
}
std::string CommReceive::getbattInSerial()
{
	return battInSerial;
}
std::string CommReceive::getbattLowWarningVoltage()
{
	return battLowWarningVoltage;
}
std::string CommReceive::getbattLowShutdownDetect()
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
void CommReceive::parseQID(unsigned char rx_buffer_t[256])
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
void CommReceive::parseQPIRI(unsigned char rx_buffer_t[256])
{
	std::string data_i;
	char one[1];
	char two[2];
	char three[3];
	char four[4];
	char five[5];

	//Parse all the 1 character parameters
	one[0] = rx_buffer_t[46];
	battInSerial = data_i(one);
	one[0] = rx_buffer_t[54];
	battLowShutdownDetect = data_i(one);

	//Parse all the 2 char parameters	
	for(int i=0; i<2; i++)
	{
		two[i] = rx_buffer_t[i+7];
	}
	nominalBattVoltage = data_i(two,2);
	for(int i=0; i<2; i++)
	{
		two[i] = rx_buffer_t[i+27];
	}
	battType = data_i(two,2);
	for(int i=0; i<2; i++)
	{
		two[i] = rx_buffer_t[i+38];
	}
	remoteTempDetect = data_i(two,2);

	//Parse all the 3 char parameters
	//None to parse

	//Parse all the 4 char parameters	
	for(int i=0; i<4; i++)
	{
		four[i] = rx_buffer_t[i+1];
	}
	maxOutputPower = data_i(four,4);
	for(int i=0; i<4; i++)
	{
		four[i] = rx_buffer_t[i+9];
	}
	nominalChargingCurrent = data_i(four,4);
	for(int i=0; i<4; i++)
	{
		four[i] = rx_buffer_t[i+32];
	}
	battTempCompensation = data_i(four,4);

	//Parse all the 5 char parameters	
	for(int i=0; i<5; i++)
	{
		five[i] = rx_buffer_t[i+15];
	}
	absorptionVoltage = data_i(five,5);
	for(int i=0; i<5; i++)
	{
		five[i] = rx_buffer_t[i+21];
	}
	floatVoltage = data_i(five,5);
	for(int i=0; i<5; i++)
	{
		five[i] = rx_buffer_t[i+33];
	}
	battTempCompensation = data_i(five,5);
	for(int i=0; i<5; i++)
	{
		five[i] = rx_buffer_t[i+48];
	}
	battLowWarningVoltage = data_i(five,5);

}
//Parse the device general status information
void CommReceive::parseQPIGS(unsigned char rx_buffer_t[256])
{

}
//Parse the device warning status information
void CommReceive::parseQPIWS(unsigned char rx_buffer_t[256])
{

}
//Parse the battery equalized information
void CommReceive::parseQBEQI(unsigned char rx_buffer_t[256])
{

}