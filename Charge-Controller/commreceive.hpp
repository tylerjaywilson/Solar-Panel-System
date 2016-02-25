#ifndef _COMMRECEIVE_HPP
#define _COMMRECEIVE_HPP

class CommReceive{

  //Inquiry parameters
  uint8_t i_deviceProtocolID;
  uint8_t i_deviceSerialNum;
  uint8_t i_CPUFirmware;
  uint8_t i_deviceRatedInfo;
  uint8_t i_deviceGeneralStatusInfo;
  uint8_t i_defaultSettingInfo;
  uint8_t i_deviceWarningStatus;
  uint8_t i_batteryEqualizer;

  //Setting parameters
  uint8_t r_deviceSerialNum;
  uint8_t r_batteryType;
  uint8_t r_batteryAbsorptionChargeVoltage;
  uint8_t r_batteryFloatingChargeVoltage;
  uint8_t r_ratingBatteryVoltage;
  uint8_t r_maxChargeCurrent;
  uint8_t r_BTSTempCompRatio;
  uint8_t r_remoteBatteryVoltageDetect;
  uint8_t r_batteryLowWarningVoltage;
  uint8_t r_batteryLowShutdownDetect;
  uint8_t r_batteryEqualization;
  uint8_t r_batteryEqualizedTime;
  uint8_t r_periodBatteryEqualization;
  uint8_t r_maxCurrentBatteryEqualization;
  uint8_t r_batteryEqualizedVoltage;
  uint8_t r_batteryCVChargeTime;
  uint8_t r_timeBatteryEqualizedTimeout;
  uint8_t r_controlParameter;

public:
  CommReceive();   //Default constructor

  //Get and set functions
  
};

#endif