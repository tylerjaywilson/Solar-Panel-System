INT16U cal_crc_half(INT8U far *pin, INT8U len)
{

	INT16U crc;

	INT8U da;
	INT8U far *ptr;
	INT8U bCRCHign;
    INT8U bCRCLow;

	INT16U crc_ta[16]=
	{ 
		0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,

		0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef
	};
	ptr=pin;
	crc=0;
	
	while(len--!=0) 
	{
		da=((INT8U)(crc>>8))>>4; 

		crc<<=4;

		crc^=crc_ta[da^(*ptr>>4)]; 

		da=((INT8U)(crc>>8))>>4; 

		crc<<=4;

		crc^=crc_ta[da^(*ptr&0x0f)]; 

		ptr++;
	}
	bCRCLow = crc;

    bCRCHign= (INT8U)(crc>>8);

	if(bCRCLow==0x28||bCRCLow==0x0d||bCRCLow==0x0a)

    {
    	bCRCLow++;
    }
    if(bCRCHign==0x28||bCRCHign==0x0d||bCRCHign==0x0a)

    {
		bCRCHign++;
    }
    crc = ((INT16U)bCRCHign)<<8;
    crc += bCRCLow;
	return(crc);
}
