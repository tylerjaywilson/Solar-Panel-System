#ifndef _COMMRECEIVE_HPP
#define _COMMRECEIVE_HPP

class CommReceive{

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
  char r_deviceSerialNum;
  char r_batteryType;
  char r_batteryAbsorptionChargeVoltage;
  char r_batteryFloatingChargeVoltage;
  char r_ratingBatteryVoltage;
  char r_maxChargeCurrent;
  char r_BTSTempCompRatio;
  char r_remoteBatteryVoltageDetect;
  char r_batteryLowWarningVoltage;
  char r_batteryLowShutdownDetect;
  char r_batteryEqualization;
  char r_batteryEqualizedTime;
  char r_periodBatteryEqualization;
  char r_maxCurrentBatteryEqualization;
  char r_batteryEqualizedVoltage;
  char r_batteryCVChargeTime;
  char r_timeBatteryEqualizedTimeout;
  char r_controlParameter;

public:
  CommReceive();   //Default constructor

  //Get and set functions
  
};

#endif