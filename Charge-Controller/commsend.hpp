#ifndef _COMMSEND_HPP
#define _COMMSEND_HPP

class CommSend{

  //Inquiry parameters
  char i_deviceProtocolID;
  char i_deviceSerialNum;
  char i_CPUFirmware;
  char i_deviceRatedInfo;
  char i_deviceGeneralStatusInfo;
  char i_defaultSettingInfo;
  char i_deviceWarningStatus;
  char i_batteryEqualizer;

  //Setting parameters
  char s_deviceSerialNum;
  char s_batteryType;
  char s_batteryAbsorptionChargeVoltage;
  char s_batteryFloatingChargeVoltage;
  char s_ratingBatteryVoltage;
  char s_maxChargeCurrent;
  char s_BTSTempCompRatio;
  char s_remoteBatteryVoltageDetect;
  char s_batteryLowWarningVoltage;
  char s_batteryLowShutdownDetect;
  char s_batteryEqualization;
  char s_batteryEqualizedTime;
  char s_periodBatteryEqualization;
  char s_maxCurrentBatteryEqualization;
  char s_batteryEqualizedVoltage;
  char s_batteryCVChargeTime;
  char s_timeBatteryEqualizedTimeout;
  char s_controlParameter;

public:
  CommSend();   //Default constructor
  //CommSend(); //Constructor
  //Get and set functions
  
};

#endif