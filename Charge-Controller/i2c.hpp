#ifndef _I2C_HPP
#define _I2C_HPP

class I2C
{
	int slave_addr;
	int filestream;

public:
  	I2C();   				//Default constructor
  	void i2c_init();
};

#endif