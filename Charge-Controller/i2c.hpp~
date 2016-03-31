#ifndef _I2C_HPP
#define _I2C_HPP

class I2C
{
	int psoc_addr;
	int filestream;

public:
  	I2C();   				//Default constructor
  	void init();
  	void write8(unsigned char *);
  	void write16(unsigned char *);
  	uint8_t read8(int);
  	uint16_t read16(int);
};

#endif