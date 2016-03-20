#ifndef _COMMRECEIVE_HPP
#define _COMMRECEIVE_HPP

#define RX_LENGTH_MAX 256

class CommReceive{

  /* QID - Device serial number */
  std::string serialNum;

  /* QPIRI - Device rated information paramters */
  int maxOutputPower;
  int nominalBattVoltage;
  float nominalChargingCurrent;
  float absorptionVoltage;
  float floatVoltage;
  int battType;
  int remoteBattVoltageDetect;
  float battTempCompensation;
  int remoteTempDetect;
  int battRatedVoltageSet;
  int battInSerial;
  float battLowWarningVoltage;
  int battLowShutdownDetect;

  /* QPIGS - Device general status parameters */
  float pvInputVoltage;
  float battVoltage;
  float chargingCurrent;
  float chargingCurrent1;
  float chargingCurrent2;
  int chargingPower;
  int unitTemp;
  float remoteBattVoltage;
  int remoteBattTemp;
  int reserved;
  int status;

  /* QPIWS - Device warning status paramters */
  int overChargeCurrent;
  int overTemp;
  int battVoltageUnder;
  int battVoltageHigh;
  int pvHighLoss;
  int battTempLow;
  int battTempHigh;
  int pvLowLoss;
  int pvHighDerating;
  int tempHighDerating;
  int battTempLowAlarm;
  int battLowWarning;

  /* QBEQI - Battery equalized information */
  int battEqualizedEn;
  int battEqualizedTime;
  int intervalTime;
  int maxCurrent;
  int remainingTime;
  float battEqualizedVoltage;
  int battCVChargeTime;
  int battEqualizedTimeout;

  /* Expected CRC value */
  uint16_t expectedCRC;
  int receivedCRC;

public:
  /* UART Input/Receive Buffer */
  unsigned char rx_buffer[RX_LENGTH_MAX];   

  CommReceive();   //Default constructor
  void CRCcalc(unsigned char *, uint8_t);
  void parseQID(unsigned char *rx_buffer_t);
  void parseQPIRI(unsigned char *rx_buffer_t);
  void parseQPIGS(unsigned char *rx_buffer_t);
  void parseQPIWS(unsigned char *rx_buffer_t);
  void parseQBEQI(unsigned char *rx_buffer_t);

  //Get functions
  std::string getSerialNum();

  int getmaxOutputPower();
  int getnominalBattVoltage();
  float getnominalChargingCurrent();
  float getabsorptionVoltage();
  float getfloatVoltage();
  int getbattType();
  int getremoteBattVoltageDetect();
  float getbattTempCompensation();
  int getremoteTempDetect();
  int getbattRatedVoltageSet();
  int getbattInSerial();
  float getbattLowWarningVoltage();
  int getbattLowShutdownDetect();

  float getpvInputVoltage();
  float getbattVoltage();
  float getchargingCurrent();
  float getchargingCurrent1();
  float getchargingCurrent2();
  int getchargingPower();
  int getunitTemp();
  float getremoteBattVoltage();
  int getremoteBattTemp();
  int getstatus();

  int getoverChargeCurrent();
  int getoverTemp();
  int getbattVoltageUnder();
  int getbattVoltageHigh();
  int getpvHighLoss();
  int getbattTempLow();
  int getbattTempHigh();
  int getpvLowLoss();
  int getpvHighDerating();
  int gettempHighDerating();
  int getbattTempLowAlarm();
  int getbattLowWarning();

  int getbattEqualizedEn();
  int getbattEqualizedTime();
  int getintervalTime();
  int getmaxCurrent();
  int getremainingTime();
  float getbattEqualizedVoltage();
  int getbattCVChargeTime();
  int getbattEqualizedTimeout();  
};

#endif
