#include<lpc21xx.h>
#include <string.h>
//#include"i2c_config.c"
#include"eeprom.c"
#include"lcd4.h"
//#define I2C_EEPROM_SA 0x50 //7Bit Slave Addr
//u8 p[9] __attribute__((at(0x40000040)))="";
#define IN_IR 14
#define OUT_IR 15
#define LED 1<<17
void START(void);
int incount=0;
int outcount=0;

int main()
{
	u8 a=1,b=1;
	LCD_INIT();
	i2c_init();
	IODIR0=LED;
	START();
	incount=0;
	outcount=0;
	i2c_eeprom_write(0X50,0X00,a);
	i2c_eeprom_write(0X50,0X01,b);
	LCD_COMMAND(0X80);
	LCD_STR("IN");
	LCD_COMMAND(0X8A);
	LCD_STR("OUT");
	while(1)
	{
		incount=i2c_eeprom_read(0X50,0X00);
		outcount=i2c_eeprom_read(0X50,0X01);
		LCD_COMMAND(0XC0);
		LCD_2INT(incount);
		LCD_COMMAND(0XCA);
		LCD_2INT(outcount);
		if(((IOPIN0>>IN_IR)&1)==0)
		{
			incount++;
			//delay_millisec(100);
			LCD_COMMAND(0XC0);
			LCD_2INT(incount);
			i2c_eeprom_write(0X50,0X00,incount);
			delay_ms(500);
		}
		if(((IOPIN0>>OUT_IR)&1)==0)
		{
				if(incount!=outcount)
				{
					outcount++;
					//delay_millisec(100);
					LCD_COMMAND(0XCA);
					LCD_2INT(outcount);
					i2c_eeprom_write(0X50,0X01,outcount);
		  			delay_ms(500);	
				}
		}
		if((incount-outcount)!=0)
		IOCLR0=LED;
		else
		IOSET0=LED;	
	}
}

void START(void)
{
		char *s="BI-DIRECTIONAL VISITOR COUNTER";
		char i=0;
		while(i<31)
		{
			LCD_COMMAND(0XC0);
		        LCD_STR("V24CE2P2");
			LCD_COMMAND(0X80);
			LCD_STR(s+i);
			delay_millisec(200);
			LCD_COMMAND(0X01);
			i++;
		}
}
