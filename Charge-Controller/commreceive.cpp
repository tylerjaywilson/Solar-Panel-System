/*
* Authors: Tyler Wilson
* Date: 02/24/2016
* Notes: This class handles the reception commands from the MPPT-3000 RS232 Charge Controller
* 
*/
#include <stdint.h>
#include "commreceive.hpp"

//Default Constructor
CommReceive::CommReceive()
{
	//Inquiry parameters
	i_deviceProtocolID = 1;
	i_deviceSerialNum = 1;
	i_CPUFirmware = 1;
	i_deviceRatedInfo = 1;
	i_deviceGeneralStatusInfo = 1;
	i_defaultSettingInfo = 1;
	i_deviceWarningStatus = 1;
	i_batteryEqualizer = 1;

	//Setting parameters
	r_deviceSerialNum = 1;
	r_batteryType = 1;
	r_batteryFloatingChargeVoltage = 1;
	r_batteryAbsorptionChargeVoltage = 1;
	r_ratingBatteryVoltage = 1;
	r_maxChargeCurrent = 1;
	r_BTSTempCompRatio = 1;
	r_remoteBatteryVoltageDetect = 1;
	r_batteryLowWarningVoltage = 1;
	r_batteryLowShutdownDetect = 1;
	r_batteryEqualization = 1;
	r_batteryEqualizedTime = 1;
	r_periodBatteryEqualization = 1;
	r_maxCurrentBatteryEqualization = 1;
	r_batteryEqualizedVoltage = 1;
	r_batteryCVChargeTime = 1;
	r_timeBatteryEqualizedTimeout = 1;
	r_controlParameter = 1;
}