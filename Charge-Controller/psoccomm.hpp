#ifndef _PSOCCOMM_HPP
#define _PSOCCOMM_HPP

class PSOCComm
{	
  /****Variables that can be read or write****/
  bool    loadRelay;
  bool    externalRelay;
  int8_t  timeMinMaxAveMinutes;   
  int8_t  timeMinMaxAveSeconds;

  //Battery Variables
  float   batteryVoltageGain;
  float   batteryVoltageOffset;
  float   batteryCurrentGain;
  float   batteryCurrentOffset;

  //Load Variables
  float   loadVoltageGain;
  float   loadVoltageOffset;
  float   loadCurrentGain;
  float   loadCurrentOffset;

  //External Variables
  float   externalVoltageGain;
  float   externalVoltageOffset;
  float   externalCurrentGain;
  float   externalCurrentOffset;

  //Extra Variables
  float   extraVoltageGain;
  float   extraVoltageOffset;

  //BMS Variables
  float   bmsLoadVoltageReconnect;
  uint8_t bmsLoadTimeActionDelay;
  float   bmsLoadVoltageDiscmin;
  uint8_t bmsBattTimeActionDelay;
  float   bmsBattVoltageEmergMinReconnect;
  float   bmsBattVoltageEmergMinDisconnect;
  float   bmsBattVoltageCriticalWarning;

  //Real Time Clock Variables
  int16_t rtcYear;
  uint8_t rtcMonth;
  uint8_t rtcDay;
  uint8_t rtcHour;
  uint8_t rtcMinute;
  uint8_t rtcSecond;
  uint8_t rtcChangesFlag;

  //Solar Panel Offset and Gain Variables
  float   solPanel1Offset;
  float   solPanel1Gain;
  float   solPanel2Offset;
  float   solPanel2Gain;
  float   solPanel3Offset;
  float   solPanel3Gain;
  float   solPanel4Offset;
  float   solPanel4Gain;
  float   solPanel5Offset;
  float   solPanel5Gain;
  float   solPanel6Offset;
  float   solPanel6Gain;
  float   solPanel7Offset;
  float   solPanel7Gain;
  float   solPanel8Offset;
  float   solPanel8Gain;

  /**** Variables that are read only ****/

  //Real time clock variable
  uint8_t rtcSecondsLeftAverage;

  //BMS state variable
  uint8_t bmsState;

  //Solar Panel 8 Variables
  float   solPanel8Average;
  float   solPanel8Min;
  float   solPanel8Max;
  float   solPanel8;

  //Solar Panel 7 Variables
  float   solPanel7Average;
  float   solPanel7Min;
  float   solPanel7Max;
  float   solPanel7;

  //Solar Panel 6 Variables
  float   solPanel6Average;
  float   solPanel6Min;
  float   solPanel6Max;
  float   solPanel6;

  //Solar Panel 5 Variables
  float   solPanel5Average;
  float   solPanel5Min;
  float   solPanel5Max;
  float   solPanel5;

  //Solar Panel 4 Variables
  float   solPanel4Average;
  float   solPanel4Min;
  float   solPanel4Max;
  float   solPanel4;

  //Solar Panel 3 Variables
  float   solPanel3Average;
  float   solPanel3Min;
  float   solPanel3Max;
  float   solPanel3;

  //Solar Panel 2 Variables
  float   solPanel2Average;
  float   solPanel2Min;
  float   solPanel2Max;
  float   solPanel2;

  //Solar Panel 1 Variables
  float   solPanel1Average;
  float   solPanel1Min;
  float   solPanel1Max;
  float   solPanel1;

  //Battery, Load, External, and Extra Variables
  float   batteryVoltageAverage;
  float   batteryVoltageMin;
  float   batteryVoltageMax;
  float   batteryVoltage;

  float   batteryCurrentAverage;
  float   batteryCurrentMin;
  float   batteryCurrentMax;
  float   batteryCurrent;

  float   loadVoltageAverage;
  float   loadVoltageMin;
  float   loadVoltageMax;
  float   loadVoltage;

  float   loadCurrentAverage;
  float   loadCurrentMin;
  float   loadCurrentMax;
  float   loadCurrent;

  float   externalVoltageAverage;
  float   externalVoltageMin;
  float   externalVoltageMax;
  float   externalVoltage; 

  float   externalCurrentAverage;
  float   externalCurrentMin;
  float   externalCurrentMax;
  float   externalCurrent; 

  float   extraVoltageAverage;
  float   extraVoltageMin;
  float   extraVoltageMax;
  float   extraVoltage; 
  /******END OF READ ONLY VARIABLES**********/

  //Create an object of the I2C class
	I2C i2c0;

  //Convert float to two unsigned characters within an array
  void float2Chars(float, unsigned char*);

public:
	PSOCComm();   				//Default constructor

  /* SET FUNCTIONS */
	void    setLoadRelay(bool);
  void    setExternalRelay(bool);
  void    setTimeMinMaxAveMinutes(int8_t);   
  void    setTimeMinMaxAveSeconds(int8_t);

  //Battery Variables
  void    setBatteryVoltageGain(float);
  void    setBatteryVoltageOffset(float);
  void    setBatteryCurrentGain(float);
  void    setBatteryCurrentOffset(float);

  //Load Variables
  void    setLoadVoltageGain(float);
  void    setLoadVoltageOffset(float);
  void    setLoadCurrentGain(float);
  void    setLoadCurrentOffset(float);

  //External Variables
  void    setExternalVoltageGain(float);
  void    setExternalVoltageOffset(float);
  void    setExternalCurrentGain(float);
  void    setExternalCurrentOffset(float);

  //Extra Variables
  void    setExtraVoltageGain(float);
  void    setExtraVoltageOffset(float);

  //BMS Variables
  void    setBmsLoadVoltageReconnect(float);
  void    setBmsLoadTimeActionDelay(uint8_t);
  void    setBmsLoadVoltageDiscmin(float);
  void    setBmsBattTimeActionDelay(uint8_t);
  void    setBmsBattVoltageEmergMinReconnect(float);
  void    setBmsBattVoltageEmergMinDisconnect(float);
  void    setBmsBattVoltageCriticalWarning(float);

  //Real Time Clock Variables
  void    setRtcYear(int16_t);
  void    setRtcMonth(uint8_t);
  void    setRtcDay(uint8_t);
  void    setRtcHour(uint8_t);
  void    setRtcMinute(uint8_t);
  void    setRtcSecond(uint8_t);
  void    setRtcChangesFlag(uint8_t);

  //Solar Panel Offset and Gain Variables
  void    setSolPanel1Offset(float);
  void    setSolPanel1Gain(float);
  void    setSolPanel2Offset(float);
  void    setSolPanel2Gain(float);
  void    setSolPanel3Offset(float);
  void    setSolPanel3Gain(float);
  void    setSolPanel4Offset(float);
  void    setSolPanel4Gain(float);
  void    setSolPanel5Offset(float);
  void    setSolPanel5Gain(float);
  void    setSolPanel6Offset(float);
  void    setSolPanel6Gain(float);
  void    setSolPanel7Offset(float);
  void    setSolPanel7Gain(float);
  void    setSolPanel8Offset(float);
  void    setSolPanel8Gain(float);

	/* GET FUNCTIONS */
	bool    getLoadRelay();
  bool    getExternalRelay();
  int8_t  getTimeMinMaxAveMinutes();   
  int8_t  getTimeMinMaxAveSeconds();

  //Battery Variables
  float   getBatteryVoltageGain();
  float   getBatteryVoltageOffset();
  float   getBatteryCurrentGain();
  float   getBatteryCurrentOffset();

  //Load Variables
  float   getLoadVoltageGain();
  float   getLoadVoltageOffset();
  float   getLoadCurrentGain();
  float   getLoadCurrentOffset();

  //External Variables
  float   getExternalVoltageGain();
  float   getExternalVoltageOffset();
  float   getExternalCurrentGain();
  float   getExternalCurrentOffset();

  //Extra Variables
  float   getExtraVoltageGain();
  float   getExtraVoltageOffset();

  //BMS Variables
  float   getBmsLoadVoltageReconnect();
  uint8_t getBmsLoadTimeActionDelay();
  float   getBmsLoadVoltageDiscmin();
  uint8_t getBmsBattTimeActionDelay();
  float   getBmsBattVoltageEmergMinReconnect();
  float   getBmsBattVoltageEmergMinDisconnect();
  float   getBmsBattVoltageCriticalWarning();

  //Real Time Clock Variables
  int16_t getRtcYear();
  uint8_t getRtcMonth();
  uint8_t getRtcDay();
  uint8_t getRtcHour();
  uint8_t getRtcMinute();
  uint8_t getRtcSecond();
  uint8_t getRtcChangesFlag();

  //Solar Panel Offset and Gain Variables
  float   getSolPanel1Offset();
  float   getSolPanel1Gain();
  float   getSolPanel2Offset();
  float   getSolPanel2Gain();
  float   getSolPanel3Offset();
  float   getSolPanel3Gain();
  float   getSolPanel4Offset();
  float   getSolPanel4Gain();
  float   getSolPanel5Offset();
  float   getSolPanel5Gain();
  float   getSolPanel6Offset();
  float   getSolPanel6Gain();
  float   getSolPanel7Offset();
  float   getSolPanel7Gain();
  float   getSolPanel8Offset();
  float   getSolPanel8Gain();

    //Real time clock variable
  uint8_t getRtcSecondsLeftAverage();

  //BMS state variable
  uint8_t getBmsState();

  //Solar Panel 8 Variables
  float   getSolPanel8Average();
  float   getSolPanel8Min();
  float   getSolPanel8Max();
  float   getSolPanel8();

  //Solar Panel 7 Variables
  float   getSolPanel7Average();
  float   getSolPanel7Min();
  float   getSolPanel7Max();
  float   getSolPanel7();

  //Solar Panel 6 Variables
  float   getSolPanel6Average();
  float   getSolPanel6Min();
  float   getSolPanel6Max();
  float   getSolPanel6();

  //Solar Panel 5 Variables
  float   getSolPanel5Average();
  float   getSolPanel5Min();
  float   getSolPanel5Max();
  float   getSolPanel5();

  //Solar Panel 4 Variables
  float   getSolPanel4Average();
  float   getSolPanel4Min();
  float   getSolPanel4Max();
  float   getSolPanel4();

  //Solar Panel 3 Variables
  float   getSolPanel3Average();
  float   getSolPanel3Min();
  float   getSolPanel3Max();
  float   getSolPanel3();

  //Solar Panel 2 Variables
  float   getSolPanel2Average();
  float   getSolPanel2Min();
  float   getSolPanel2Max();
  float   getSolPanel2();

  //Solar Panel 1 Variables
  float   getSolPanel1Average();
  float   getSolPanel1Min();
  float   getSolPanel1Max();
  float   getSolPanel1();

  //Battery, Load, External, and Extra Variables
  float   getBatteryVoltageAverage();
  float   getBatteryVoltageMin();
  float   getBatteryVoltageMax();
  float   getBatteryVoltage();

  float   getBatteryCurrentAverage();
  float   getBatteryCurrentMin();
  float   getBatteryCurrentMax();
  float   getBatteryCurrent();

  float   getLoadVoltageAverage();
  float   getLoadVoltageMin();
  float   getLoadVoltageMax();
  float   getLoadVoltage();

  float   getLoadCurrentAverage();
  float   getLoadCurrentMin();
  float   getLoadCurrentMax();
  float   getLoadCurrent();

  float   getExternalVoltageAverage();
  float   getExternalVoltageMin();
  float   getExternalVoltageMax();
  float   getExternalVoltage(); 

  float   getExternalCurrentAverage();
  float   getExternalCurrentMin();
  float   getExternalCurrentMax();
  float   getExternalCurrent(); 

  float   getExtraVoltageAverage();
  float   getExtraVoltageMin();
  float   getExtraVoltageMax();
  float   getExtraVoltage(); 
};  
  
#endif