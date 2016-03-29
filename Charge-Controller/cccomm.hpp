#ifndef _CCCOMM_HPP
#define _CCCOMM_HPP

#define RX_LENGTH_MAX 256       //Max number of bytes in a receive array
#define NUM_BYTES_TX_MAX 20     //Max number of bytes in a request(transmit) array

class CCComm{

  /**********Variables for sending************/
  //Request command arrays
  char s_deviceSerialNum[NUM_BYTES_TX_MAX];     //QID request array
  char s_deviceRatedInfo[NUM_BYTES_TX_MAX];     //QPIRI request array
  char s_deviceGeneralStatusInfo[NUM_BYTES_TX_MAX]; //QPIGS request array
  char s_deviceWarningStatus[NUM_BYTES_TX_MAX];   //QPIWS request array
  char s_batteryEqualizedInfo[NUM_BYTES_TX_MAX];    //QBEQI request array

  //Setting parameters commands
  char s_setBattType[NUM_BYTES_TX_MAX];           //PBT setting array
  char s_setBattAbsorbtionChargingVoltage[NUM_BYTES_TX_MAX];  //PBAV setting array
  char s_setBattFloatingChargingVoltage[NUM_BYTES_TX_MAX];  //PBFV setting array
  char s_setRatedBattVoltage[NUM_BYTES_TX_MAX];       //PBRV setting array
  char s_setMaxChargingCurrent[NUM_BYTES_TX_MAX];       //MCHGC setting array
  char s_enRemoteBatteryVoltageDetect[NUM_BYTES_TX_MAX];    //PRBD setting array
  char s_setBattLowWarningVoltage[NUM_BYTES_TX_MAX];      //PBLV setting array
  char s_setBattLowShutdownDetectEn[NUM_BYTES_TX_MAX];    //PBLSE setting array
  char s_setBattEqualizationEn[NUM_BYTES_TX_MAX];       //PBEQE setting array
  char s_setBattEqualizedTime[NUM_BYTES_TX_MAX];        //PBEQT setting array
  char s_setPeriodBattEqualization[NUM_BYTES_TX_MAX];     //PBEQP setting array
  char s_setMaxCurrentBatteryEqualization[NUM_BYTES_TX_MAX];  //PBEQMC setting array
  char s_setBattEqualizedVoltage[NUM_BYTES_TX_MAX];     //PBEQV setting array
  char s_setBattCVChargeTime[NUM_BYTES_TX_MAX];       //PBCVT setting array
  char s_setTimeBatteryEqualizedTimeout[NUM_BYTES_TX_MAX];  //PBEQOT setting array
  char s_setControlParameterDefault[NUM_BYTES_TX_MAX];    //PF setting array

  /*********Variables and functions for receiving data***********/

  /* QID - Device serial number */
  std::string serialNum;

  /* QPIRI - Device rated information paramters */
  int   maxOutputPower;
  int   nominalBattVoltage;
  float nominalChargingCurrent;
  float absorptionVoltage;
  float floatVoltage;
  int   battType;
  int   remoteBattVoltageDetect;
  float battTempCompensation;
  int   remoteTempDetect;
  int   battRatedVoltageSet;
  int   battInSerial;
  float battLowWarningVoltage;
  int   battLowShutdownDetect;

  /* QPIGS - Device general status parameters */
  float pvInputVoltage;
  float battVoltage;
  float chargingCurrent;
  float chargingCurrent1;
  float chargingCurrent2;
  int   chargingPower;
  int   unitTemp;
  float remoteBattVoltage;
  int   remoteBattTemp;
  int   reserved;
  int   status;

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
  int   battEqualizedEn;
  int   battEqualizedTime;
  int   intervalTime;
  int   maxCurrent;
  int   remainingTime;
  float battEqualizedVoltage;
  int   battCVChargeTime;
  int   battEqualizedTimeout;

  /* Expected CRC value */
  uint16_t  expectedCRC;
  int       receivedCRC;

  /*Parsing and CRC functions*/
  void CRCcalc(char *, uint8_t);       //CRC calculation function
  void parseQID(unsigned char *rx_buffer_t);    //Parse the QID request for the device serial number
  void parseQPIRI(unsigned char *rx_buffer_t);  //Parse the QPIRI request for device rate information  
  void parseQPIGS(unsigned char *rx_buffer_t);  //Parse the QPIGS request for device general status parameters
  void parseQPIWS(unsigned char *rx_buffer_t);  //Parse the QDI request for device warning status
  void parseQBEQI(unsigned char *rx_buffer_t);  //Parse the QBEQI request for battery equalized information
  bool parseACKNAK(unsigned char *rx_buffer_t); //Parse the ACKNAK response to a setting command - Return true if ACK

  /*Timeout variable and function - Used for updating the charge controller parameters*/
  clock_t startTime;
  bool updateParameters();   //Return true if the data needs updating
  //Get function - Timeout - Start Time
  clock_t getstartTime();
  void setStartTime(clock_t);
  
  //UART object
  UART uart0;                           //Create an object of class UART - Used to establish a UART connection
  
  //Write and read functions for communicating to the Charge controller
  void uartWrite(int);
  void uartRead(int);

public:

  CCComm();   //Default constructor

  /* UART Input/Receive Buffer */
  unsigned char rx_buffer[RX_LENGTH_MAX];   

  /*******Set functions********/
  void setBattType(int);
  void setBattAbsorbtionChargingVoltage(float);
  void setBattFloatingChargingVoltage(float);
  void setRatedBattVoltage(int);
  void setMaxChargingCurrent(int);
  void setEnRemoteBatteryVoltageDetect(int);
  void setBattLowWarningVoltage(float);
  void setBattLowShutdownDetectEn(int);
  void setBattEqualizationEn(int);        
  void setBattEqualizedTime(int);       
  void setPeriodBattEqualization(int);      
  void setMaxCurrentBatteryEqualization(int); 
  void setBattEqualizedVoltage(float);      
  void setBattCVChargeTime(int);        
  void setTimeBatteryEqualizedTimeout(int);   
  
  /******Get functions*******/
  //QID
  std::string getSerialNum();

  //QPIRI
  int   getmaxOutputPower();
  int   getnominalBattVoltage();
  float getnominalChargingCurrent();
  float getabsorptionVoltage();
  float getfloatVoltage();
  int   getbattType();
  int   getremoteBattVoltageDetect();
  float getbattTempCompensation();
  int   getremoteTempDetect();
  int   getbattRatedVoltageSet();
  int   getbattInSerial();
  float getbattLowWarningVoltage();
  int   getbattLowShutdownDetect();

  //QPIGS
  float getpvInputVoltage();
  float getbattVoltage();
  float getchargingCurrent();
  float getchargingCurrent1();
  float getchargingCurrent2();
  int   getchargingPower();
  int   getunitTemp();
  float getremoteBattVoltage();
  int   getremoteBattTemp();
  int   getstatus();

  //QPIWS
  int   getoverChargeCurrent();
  int   getoverTemp();
  int   getbattVoltageUnder();
  int   getbattVoltageHigh();
  int   getpvHighLoss();
  int   getbattTempLow();
  int   getbattTempHigh();
  int   getpvLowLoss();
  int   getpvHighDerating();
  int   gettempHighDerating();
  int   getbattTempLowAlarm();
  int   getbattLowWarning();

  //QBEQI
  int   getbattEqualizedEn();
  int   getbattEqualizedTime();
  int   getintervalTime();
  int   getmaxCurrent();
  int   getremainingTime();
  float getbattEqualizedVoltage();
  int   getbattCVChargeTime();
  int   getbattEqualizedTimeout();    
};

#endif