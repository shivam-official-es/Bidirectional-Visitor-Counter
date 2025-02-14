#include<LPC21XX.H>
#define LCD_D 0X0F<<20
#define RS 1<<17
#define RW 1<<18
#define E 1<<19

void LCD_INIT(void);
void LCD_COMMAND(char );
void LCD_DATA(char );
void LCD_STR( char *);
void delay_millisec(int );
void LCD_INTEGER(int );
void LCD_2INT(int );

void LCD_INIT(void)
{
	IODIR1=LCD_D|RS|RW|E;
	LCD_COMMAND(0x01);
	LCD_COMMAND(0x02);
	LCD_COMMAND(0x0c);
	LCD_COMMAND(0x28);
	LCD_COMMAND(0x80);
}
void LCD_COMMAND(char cmd)
{
	IOCLR1=LCD_D;
	IOSET1=(cmd & 0xf0)<<16;
	IOCLR1=RS;
	IOSET1=E;
	delay_millisec(2);
	IOCLR1=E;
	IOCLR1=LCD_D;
	IOSET1=(cmd & 0x0f)<<20;
	IOCLR1=RS;
	IOSET1=E;
	delay_millisec(2);
	IOCLR1=E;
}
void LCD_DATA(char d)
{
	IOCLR1=LCD_D;
	IOSET1=(d & 0xf0)<<16;
	IOSET1=RS;
	IOSET1=E;
	delay_millisec(2);
	IOCLR1=E;
	IOCLR1=LCD_D;
	IOSET1=(d & 0x0f)<<20;
	IOSET1=RS;
	IOSET1=E;
	delay_millisec(2);
	IOCLR1=E;
}
void LCD_STR(char *s)
{
	while(*s)
	LCD_DATA(*s++);
}

void delay_millisec(int ms)
{
T0PR=15000-1;
T0TCR=0x01;
while(T0TC<ms);
T0TCR=0x03;
T0TCR=0x00;
}
void LCD_INTEGER(int n)
{
	unsigned char a[5];
	signed char i=0;
	if(n==0)
		LCD_DATA('0');
	else
		if(n<0)
		{
			LCD_DATA('-');
			n=-n;
		}
		while(n>0)
		{
			a[i++]=n%10;
			n=n/10;
		}
		for(--i;i>=0;i--)
		LCD_DATA(a[i]+48);
}
void LCD_2INT(int n)
{
	LCD_DATA(n/10+48);
	LCD_DATA(n%10+48);
}

