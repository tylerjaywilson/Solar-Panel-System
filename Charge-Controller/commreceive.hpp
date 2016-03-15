#ifndef _COMMRECEIVE_HPP
#define _COMMRECEIVE_HPP

#define RX_LENGTH_MAX 256

class CommReceive{

  /* QID - Device serial number */
  std::string serialNum;

  /* QPIRI - Device rated information paramters */
  int maxOutputPower;
  int nominalBattVoltage;
  int nominalChargingCurrent;
  int absorptionVoltage;
  int floatVoltage;
  int battType;
  int remoteBattVoltageDetect;
  std::string battTempCompensation;
  int remoteTempDetect;
  int battRatedVoltageSet;
  int battInSerial;
  int battLowWarningVoltage;
  int battLowShutdownDetect;

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
  unsigned char rx_buffer[RX_LENGTH_MAX];   

  CommReceive();   //Default constructor
  void parseQID(unsigned char rx_buffer_t[RX_LENGTH_MAX]);
  void parseQPIRI(unsigned char rx_buffer_t[RX_LENGTH_MAX]);
  void parseQPIGS(unsigned char rx_buffer_t[RX_LENGTH_MAX]);
  void parseQPIWS(unsigned char rx_buffer_t[RX_LENGTH_MAX]);
  void parseQBEQI(unsigned char rx_buffer_t[RX_LENGTH_MAX]);

  //Get functions
  std::string getSerialNum();
  int getmaxOutputPower();
  int getnominalBattVoltage();
  int getnominalChargingCurrent();
  int getabsorptionVoltage();
  int getfloatVoltage();
  int getbattType();
  int getremoteBattVoltageDetect();
  std::string getbattTempCompensation();
  int getremoteTempDetect();
  int getbattRatedVoltageSet();
  int getbattInSerial();
  int getbattLowWarningVoltage();
  int getbattLowShutdownDetect();

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
};

#endif