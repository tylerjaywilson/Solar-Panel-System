#ifndef _PSOCCOMM_HPP
#define _PSOCCOMM_HPP

class PSOCComm
{	
	float batteryVoltage;
	float batteryCurrent;
	float loadVoltage;
	float loadCurrent;
	float externalVoltage;
	float externalCurrent;
	float extraVoltage;

	I2C i2c0;

public:
  	PSOCComm();   				//Default constructor
  	void writei2c(int);
  	void readi2c(int);

  	/* SET FUNCTIONS */
  	void setBatteryVoltage(float);
  	void setBatteryCurrent(float);
  	void setLoadVoltage(float);
  	void setLoadCurrent(float);
  	void setExternalVoltage(float);
  	void setExternalCurrent(float);
  	void setExtraVoltage(float);
  	
  	/* GET FUNCTIONS */
  	float getBatteryVoltage();
  	float getBatteryCurrent();
  	float getLoadVoltage();
  	float getLoadCurrent();
  	float getExternalVoltage();
  	float getExternalCurrent();
  	float getExtraVoltage();
};

#endif