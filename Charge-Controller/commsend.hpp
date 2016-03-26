#ifndef _COMMSEND_HPP
#define _COMMSEND_HPP

#define NUM_BYTES_TX_MAX 20			//Max number of bytes in a request(transmit) array
class CommSend{

public:

	//Inquiry parameters
	char s_deviceSerialNum[NUM_BYTES_TX_MAX];			//QID request array
	char s_deviceRatedInfo[NUM_BYTES_TX_MAX];			//QPIRI request array
	char s_deviceGeneralStatusInfo[NUM_BYTES_TX_MAX];	//QPIGS request array
	char s_deviceWarningStatus[NUM_BYTES_TX_MAX];		//QPIWS request array
	char s_batteryEqualizedInfo[NUM_BYTES_TX_MAX];		//QBEQI request array

	//Setting parameters commands
	char s_setBattType[NUM_BYTES_TX_MAX];						//PBT setting array
	char s_setBattAbsorbtionChargingVoltage[NUM_BYTES_TX_MAX];	//PBAV setting array
	char s_setBattFloatingChargingVoltage[NUM_BYTES_TX_MAX];	//PBFV setting array
	char s_setRatedBattVoltage[NUM_BYTES_TX_MAX];				//PBRV setting array
	char s_setMaxChargingCurrent[NUM_BYTES_TX_MAX];				//MCHGC setting array
	char s_enRemoteBatteryVoltageDetect[NUM_BYTES_TX_MAX];		//PRBD setting array
	char s_setBattLowWarningVoltage[NUM_BYTES_TX_MAX];			//PBLV setting array
	char s_setBattLowShutdownDetectEn[NUM_BYTES_TX_MAX];		//PBLSE setting array
	char s_setBattEqualizationEn[NUM_BYTES_TX_MAX];				//PBEQE setting array
	char s_setBattEqualizedTime[NUM_BYTES_TX_MAX];				//PBEQT setting array
	char s_setPeriodBattEqualization[NUM_BYTES_TX_MAX];			//PBEQP setting array
	char s_setMaxCurrentBatteryEqualization[NUM_BYTES_TX_MAX];	//PBEQMC setting array
	char s_setBattEqualizedVoltage[NUM_BYTES_TX_MAX];			//PBEQV setting array
	char s_setBattCVChargeTime[NUM_BYTES_TX_MAX];				//PBCVT setting array
	char s_setTimeBatteryEqualizedTimeout[NUM_BYTES_TX_MAX];	//PBEQOT setting array
	char s_setControlParameterDefault[NUM_BYTES_TX_MAX];		//PF setting array

	void CRCcalc(char*, uint8_t);		//CRC calcuation function
	CommSend();   //Default constructor

	//Set functions
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
};

#endif