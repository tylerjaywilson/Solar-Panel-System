#ifndef _CCCOMM_HPP
#define _CCCOMM_HPP

#define RX_LENGTH_MAX 256       //Max number of bytes in a receive array
#define NUM_BYTES_TX_MAX 20     //Max number of bytes in a request(transmit) array

class CCComm{

  /**********Variables for sending************/
  //Request command arrays
  char s_deviceSerialNum[NUM_BYTES_TX_MAX];         //QID request array
  char s_deviceRatedInfo[NUM_BYTES_TX_MAX];         //QPIRI request array
  char s_deviceGeneralStatusInfo[NUM_BYTES_TX_MAX]; //QPIGS request array
  char s_defaultSettingValueInfo[NUM_BYTES_TX_MAX]; //QDI request array
  char s_deviceWarningStatus[NUM_BYTES_TX_MAX];     //QPIWS request array
  char s_batteryEqualizedInfo[NUM_BYTES_TX_MAX];    //QBEQI request array

  //Setting parameters commands
  char s_setBattType[NUM_BYTES_TX_MAX];                       //PBT setting array
  char s_setBattAbsorbtionChargingVoltage[NUM_BYTES_TX_MAX];  //PBAV setting array
  char s_setBattFloatingChargingVoltage[NUM_BYTES_TX_MAX];    //PBFV setting array
  char s_setRatedBattVoltage[NUM_BYTES_TX_MAX];               //PBRV setting array
  char s_setMaxChargingCurrent[NUM_BYTES_TX_MAX];             //MCHGC setting array
  char s_enRemoteBatteryVoltageDetect[NUM_BYTES_TX_MAX];      //PRBD setting array
  char s_setBattLowWarningVoltage[NUM_BYTES_TX_MAX];          //PBLV setting array
  char s_setBattLowShutdownDetectEn[NUM_BYTES_TX_MAX];        //PBLSE setting array
  char s_setBattEqualizationEn[NUM_BYTES_TX_MAX];             //PBEQE setting array
  char s_setBattEqualizedTime[NUM_BYTES_TX_MAX];              //PBEQT setting array
  char s_setPeriodBattEqualization[NUM_BYTES_TX_MAX];         //PBEQP setting array
  char s_setMaxCurrentBatteryEqualization[NUM_BYTES_TX_MAX];  //PBEQMC setting array
  char s_setBattEqualizedVoltage[NUM_BYTES_TX_MAX];           //PBEQV setting array
  char s_setBattCVChargeTime[NUM_BYTES_TX_MAX];               //PBCVT setting array
  char s_setTimeBatteryEqualizedTimeout[NUM_BYTES_TX_MAX];    //PBEQOT setting array
  char s_setControlParameterDefault[NUM_BYTES_TX_MAX];        //PF setting array

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

  /* QDI - Default Setting Value information */
  // The 'd' is to indicate 'default'
  int   d_battRatedVoltageSet;
  float d_maxChargingCurrent;
  int   d_battType;
  float d_absorbtionVoltage;
  float d_floatingVoltage;
  int   d_remoteBattVoltageDetect;
  float d_tempCompensationRatio;
  int   d_reserved;

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
  void CRCcalc(char *, uint8_t);                  //CRC calculation function
  void parseQID(unsigned char *rx_buffer_t);      //Parse the QID request for the device serial number
  void parseQPIRI(unsigned char *rx_buffer_t);    //Parse the QPIRI request for device rate information  
  void parseQPIGS(unsigned char *rx_buffer_t);    //Parse the QPIGS request for device general status parameters
  void parseQPIWS(unsigned char *rx_buffer_t);    //Parse the QDI request for device warning status
  void parseQBEQI(unsigned char *rx_buffer_t);    //Parse the QBEQI request for battery equalized information
  void parseQDI(unsigned char *rx_buffer_t);      //Parse the QDI request for default setting value information
  bool parseACKNACK(unsigned char *rx_buffer_t);  //Parse the ACKNAK response to a setting command - Return true if ACK

  /*Timeout variable and function - Used for updating the charge controller parameters*/
  clock_t startTime;  

  //Get function - Timeout - Start Time
  clock_t getStartTime();
  void setStartTime(clock_t);
  
  //UART object
  UART uart0;   //Create an object of class UART - Used to establish a UART connection
  
  //Write and read functions for communicating to the Charge controller
  void uartWrite(int);
  void uartRead(int);

  /* UART Input/Receive Buffer */
  unsigned char rx_buffer[RX_LENGTH_MAX];   

public:
  /* I2C object and functions */
  I2C i2c0;     //Create an object of class I2C - Used for communication to the PSoC5

  CCComm();   //Default constructor
  bool updateParameters();   //Return true if the data needs updating
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
  int   getMaxOutputPower();
  int   getNominalBattVoltage();
  float getNominalChargingCurrent();
  float getAbsorptionVoltage();
  float getFloatVoltage();
  int   getBattType();
  int   getRemoteBattVoltageDetect();
  float getBattTempCompensation();
  int   getRemoteTempDetect();
  int   getBattRatedVoltageSet();
  int   getBattInSerial();
  float getBattLowWarningVoltage();
  int   getBattLowShutdownDetect();

  //QPIGS
  float getPVInputVoltage();
  float getBattVoltage();
  float getChargingCurrent();
  float getChargingCurrent1();
  float getChargingCurrent2();
  int   getChargingPower();
  int   getUnitTemp();
  float getRemoteBattVoltage();
  int   getRemoteBattTemp();
  int   getStatus();

  //QDI
  int   getDBattRatedVoltageSet();
  float getDMaxChargingCurrent();
  int   getDBattType();
  float getDAbsorptionVoltage();
  float getDFloatingVoltage();
  int   getDRemoteBattVoltageDetect();
  float getDTempCompensationRatio();
  int   getDReserved();

  //QPIWS
  int   getOverChargeCurrent();
  int   getOverTemp();
  int   getBattVoltageUnder();
  int   getBattVoltageHigh();
  int   getPVHighLoss();
  int   getBattTempLow();
  int   getBattTempHigh();
  int   getPVLowLoss();
  int   getPVHighDerating();
  int   getTempHighDerating();
  int   getBattTempLowAlarm();
  int   getBattLowWarning();

  //QBEQI
  int   getBattEqualizedEn();
  int   getBattEqualizedTime();
  int   getIntervalTime();
  int   getMaxCurrent();
  int   getRemainingTime();
  float getBattEqualizedVoltage();
  int   getBattCVChargeTime();
  int   getBattEqualizedTimeout();    
};

#endif