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
  void CRCcalc(unsigned char *, uint8_t);       //CRC calculation function
  void parseQID(unsigned char *rx_buffer_t);    //Parse the QID request for the device serial number
  void parseQPIRI(unsigned char *rx_buffer_t);  //Parse the QPIRI request for device rate information  
  void parseQPIGS(unsigned char *rx_buffer_t);  //Parse the QPIGS request for device general status parameters
  void parseQPIWS(unsigned char *rx_buffer_t);  //Parse the QDI request for device warning status
  void parseQBEQI(unsigned char *rx_buffer_t);  //Parse the QBEQI request for battery equalized information

  /******Get functions*******/
  //QID
  std::string getSerialNum();

  //QPIRI
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

  //QPIGS
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

  //QPIWS
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

  //QBEQI
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
