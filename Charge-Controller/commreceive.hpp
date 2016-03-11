#ifndef _COMMRECEIVE_HPP
#define _COMMRECEIVE_HPP

class CommReceive{

  /* UART Input/Receive Buffer */
  unsigned char rx_buffer[256];   

  /* QID - Device serial number */
  std::string serialNum;

  /* QPIRI - Device rated information paramters */
  std::string maxOutputPower;
  std::string nominalBattVoltage;
  std::string nominalChargingCurrent;
  std::string absorptionVoltage;
  std::string floatVoltage;
  std::string battType;
  std::string remoteBattVoltageDetect;
  std::string battTempCompensation;
  std::string remoteTempDetect;
  std::string battRatedVoltageSet;
  std::string battInSerial;
  std::string battLowWarningVoltage;
  std::string battLowShutdownDetect;

  /* QPIGS - Device general status parameters */
  std::string pvInputVoltage;
  std::string battVoltage;
  std::string chargingCurrent;
  std::string chargingCurrent1;
  std::string chargingCurrent2;
  std::string chargingPower;
  std::string unitTemp;
  std::string remoteBattVoltage;
  std::string remoteBattTemp;
  //std::string reserved;
  std::string status;

  /* QPIWS - Device warning status paramters */
  std::string overChargeCurrent;
  std::string overTemp;
  std::string battVoltageUnder;
  std::string battVoltageHigh;
  std::string pvHighLoss;
  std::string battTempLow;
  std::string battTempHigh;
  std::string pvLowLoss;
  std::string pvHighDerating;
  std::string tempHighDerating;
  std::string battTempLowAlarm;
  std::string battLowWarning;

  /* QBEQI - Battery equalized information */
  std::string battEqualizedEn;
  std::string battEqualizedTime;
  std::string intervalTime;
  std::string maxCurrent;
  std::string remainingTime;
  std::string battEqualizeddVoltage;
  std::string battCVChargeTime;
  std::string battEqualizedTimeout;


public:
  CommReceive();   //Default constructor

  //Get functions
  std::string getSerialNum();
};

#endif