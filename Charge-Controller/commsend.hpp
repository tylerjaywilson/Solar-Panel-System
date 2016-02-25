#ifndef _COMMSEND_HPP
#define _COMMSEND_HPP

class CommSend{

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
  uint8_t s_deviceSerialNum;
  uint8_t s_batteryType;
  uint8_t s_batteryAbsorptionChargeVoltage;
  uint8_t s_batteryFloatingChargeVoltage;
  uint8_t s_ratingBatteryVoltage;
  uint8_t s_maxChargeCurrent;
  uint8_t s_BTSTempCompRatio;
  uint8_t s_remoteBatteryVoltageDetect;
  uint8_t s_batteryLowWarningVoltage;
  uint8_t s_batteryLowShutdownDetect;
  uint8_t s_batteryEqualization;
  uint8_t s_batteryEqualizedTime;
  uint8_t s_periodBatteryEqualization;
  uint8_t s_maxCurrentBatteryEqualization;
  uint8_t s_batteryEqualizedVoltage;
  uint8_t s_batteryCVChargeTime;
  uint8_t s_timeBatteryEqualizedTimeout;
  uint8_t s_controlParameter;

public:
  CommSend();   //Default constructor
  //CommSend(); //Constructor
  //Get and set functions
  
};

#endif