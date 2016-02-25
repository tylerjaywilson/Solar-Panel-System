/*
* Authors: Tyler Wilson
* Date: 02/24/2016
* Notes: This class handles the transmission commands to the MPPT-3000 RS232 Charge Controller
* 
*/

#include "commsend.hpp"

//Default Constructor
CommSend::CommSend()
{
	//Inquiry parameters
	i_deviceProtocolID = '1';
	i_deviceSerialNum = '1';
	i_CPUFirmware = '1';
	i_deviceRatedInfo = '1';
	i_deviceGeneralStatusInfo = '1';
	i_defaultSettingInfo = '1';
	i_deviceWarningStatus = '1';
	i_batteryEqualizer = '1';

	//Setting parameters
	s_deviceSerialNum = '1';
	s_batteryType = '1';
	s_batteryAbsorptionChargeVoltage = '1';
	s_batteryFloatingChargeVoltage = '1';
	s_ratingBatteryVoltage = '1';
	s_maxChargeCurrent = '1';
	s_BTSTempCompRatio = '1';
	s_remoteBatteryVoltageDetect = '1';
	s_batteryLowWarningVoltage = '1';
	s_batteryLowShutdownDetect = '1';
	s_batteryEqualization = '1';
	s_batteryEqualizedTime = '1';
	s_periodBatteryEqualization = '1';
	s_maxCurrentBatteryEqualization = '1';
	s_batteryEqualizedVoltage = '1';
	s_batteryCVChargeTime = '1';
	s_timeBatteryEqualizedTimeout = '1';
	s_controlParameter = '1';
}

//Constructor
/*CommSend::CommSend(int curr_time_t, int task_id_t, int preempted_task_t, int deadline_missed_task_t)
{
  	curr_time = curr_time_t;
	task_id = task_id_t;
	preempted_task = preempted_task_t;
	deadline_missed_task = deadline_missed_task_t;
}*/ 