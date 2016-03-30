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
  	int read8(int);
  	int read16(int);
};

#endif