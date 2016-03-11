#ifndef _COMMRECEIVE_HPP
#define _COMMRECEIVE_HPP

class CommReceive{

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
  /* UART Input/Receive Buffer */
  unsigned char rx_buffer[256];   

  CommReceive();   //Default constructor
  void parseQID(char rx_buffer_t[256]);
  void parseQPIRI(char rx_buffer_t[256]);
  void parseQPIGS(char rx_buffer_t[256]);
  void parseQPIWS(char rx_buffer_t[256]);
  void parseQBEQI(char rx_buffer_t[256]);

  //Get functions
  std::string getSerialNum();
  std::string getmaxOutputPower();
  std::string getnominalBattVoltage();
  std::string getnominalChargingCurrent();
  std::string getabsorptionVoltage();
  std::string getfloatVoltage();
  std::string getbattType();
  std::string getremoteBattVoltageDetect();
  std::string getbattTempCompensation();
  std::string getremoteTempDetect();
  std::string getbattRatedVoltageSet();
  std::string getbattInSerial();
  std::string getbattLowWarningVoltage();
  std::string getbattLowShutdownDetect();

  std::string getpvInputVoltage();
  std::string getbattVoltage();
  std::string getchargingCurrent();
  std::string getchargingCurrent1();
  std::string getchargingCurrent2();
  std::string getchargingPower();
  std::string getunitTemp();
  std::string getremoteBattVoltage();
  std::string getremoteBattTemp();
  std::string getstatus();

  std::string getoverChargeCurrent();
  std::string getoverTemp();
  std::string getbattVoltageUnder();
  std::string getbattVoltageHigh();
  std::string getpvHighLoss();
  std::string getbattTempLow();
  std::string getbattTempHigh();
  std::string getpvLowLoss();
  std::string getpvHighDerating();
  std::string gettempHighDerating();
  std::string getbattTempLowAlarm();
  std::string getbattLowWarning();

  std::string getbattEqualizedEn();
  std::string getbattEqualizedTime();
  std::string getintervalTime();
  std::string getmaxCurrent();
  std::string getremainingTime();
  std::string getbattEqualizeddVoltage();
  std::string getbattCVChargeTime();
  std::string getbattEqualizedTimeout();

  /* Set functions */
  void setSerialNum();
  void setmaxOutputPower();
  void setnominalBattVoltage();
  void setnominalChargingCurrent();
  void setabsorptionVoltage();
  void setfloatVoltage();
  void setbattType();
  void setremoteBattVoltageDetect();
  void setbattTempCompensation();
  void setremoteTempDetect();
  void setbattRatedVoltageSet();
  void setbattInSerial();
  void setbattLowWarningVoltage();
  void setbattLowShutdownDetect();

  void setpvInputVoltage();
  void setbattVoltage();
  void setchargingCurrent();
  void setchargingCurrent1();
  void setchargingCurrent2();
  void setchargingPower();
  void setunitTemp();
  void setremoteBattVoltage();
  void setremoteBattTemp();
  void setstatus();

  void setoverChargeCurrent();
  void setoverTemp();
  void setbattVoltageUnder();
  void setbattVoltageHigh();
  void setpvHighLoss();
  void setbattTempLow();
  void setbattTempHigh();
  void setpvLowLoss();
  void setpvHighDerating();
  void settempHighDerating();
  void setbattTempLowAlarm();
  void setbattLowWarning();

  void setbattEqualizedEn();
  void setbattEqualizedTime();
  void setintervalTime();
  void setmaxCurrent();
  void setremainingTime();
  void setbattEqualizeddVoltage();
  void setbattCVChargeTime();
  void setbattEqualizedTimeout();
};

#endif