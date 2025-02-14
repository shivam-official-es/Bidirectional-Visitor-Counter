#include<lpc21xx.h>
#include<string.h>
#include"vc_i2c_config.c"
#include"vc_eeprom.c"
//#include"delay.h"
#define LCD_D 0X0f<<20 
#define RS 1<<17
#define RW 1<<18
#define E 1<<19
#define ir1 14
#define ir2 15
#define light 17
unsigned int incount=0,outcount=0,total=0,current=0;
typedef unsigned char u8;
void lcd_init(void);
void lcd_command(unsigned char );
void lcd_data(unsigned char );
void lcd_project(u8* );
void lcd_str(u8 *,u8 );
void lcd_int(int );
void visitor_counter(void);
int main()
{	    
		u8 *a="Bidirectional Visitor Counter                  ";
		u8 *id="V24CE2V2";
	
		lcd_init();
		lcd_str("BIDIRECTIONAL",0X81);
		lcd_str("VISITOR COUNTER",0XC0);
		delay_ms(2000);
		lcd_command(0X01);
		lcd_str(id,0XC4);
		lcd_project(a);
		delay_ms(200);
		 i2c_init();
		visitor_counter();
		 }
void visitor_counter()
{	   	
		
		u8 *v="VISITOR";
		u8* in="I:";
		u8*out="O:";
		u8 *c="c:";
		u8 i=0;
		 IODIR0|=1<<light;
		 IOSET0=1<<light;
		//incount=0;
		//i2c_eeprom_write(0X50,0X00,incount);
		//i=i2c_eeprom_read(0X50,0X00);
		//lcd_command(0X01);
		//lcd_command(0X80);
		//lcd_int(i);
		//delay_ms(4000);
		//lcd_command(0X01);
		
		  while(1){
					  if(incount==0){
			    i=i2c_eeprom_read(0X50,0X00);
				}
		 		if(((IOPIN0>>ir1)&1)==0){
		 			delay_ms(350);
						   //lcd_command(0X01);
					lcd_str("People entering",0X80);
				//	delay_ms(2000);
					incount++;	 		
		 		}
		 		if(((IOPIN0>>ir2)&1)==0){
					 if(outcount<incount){
					    delay_ms(200);
						//lcd_command(0X01);
						lcd_str("People leaving ",0X80);
				//		delay_ms(2000);
		 		   		outcount++;
		 				}
		 			}
		  total=incount-outcount;
		  if(total==0){
				  IOSET0=1<<light;
				  lcd_command(0X01);
				  lcd_str("TOTAL=0",0X84);
		   }
		   else { 
			    IOCLR0=1<<light;
			    //lcd_command(0X01); 
			    //lcd_str(v,0X84);
			    lcd_str(in,0Xc0);
				lcd_command(0Xc2);
			    lcd_int(i+incount);	
			 	lcd_str(out,0Xc6);
				lcd_command(0Xc8);
				lcd_int(outcount);
				lcd_str(c,0Xca);
				lcd_command(0Xcc);
				lcd_int(incount);
			  	i2c_eeprom_write(0X50,0X00,i+incount);
			    delay_ms(500);
			   }
			   
		 }
}
void lcd_project(u8* a)
{
int i=0,n=strlen(a),j,k=0;
while(k<30)
{
for(j=0;j<16;j++)
{
	lcd_command(0X80+j);
	lcd_data(a[(i+j)%n]);
}
i=(i+1)%n;
delay_ms(250); 

k++;
}
}

void lcd_str(u8 *id,u8 a)
{ 
   lcd_command(a);
   while(*id)
  { lcd_data(*id);
   id++;}
}
 
 void lcd_init()
{
IODIR1=LCD_D|RS|RW|E;
lcd_command(0X01);
lcd_command(0X02);
lcd_command(0X0C);
lcd_command(0X28);
}
void lcd_command(unsigned char cmd)
{
		IOCLR1=LCD_D;
		IOSET1=(cmd&0xf0)<<16;
		IOCLR1=RS;
		IOCLR1=RW;
		IOSET1=E;
		delay_ms(2);
		IOCLR1=E;

		IOCLR1=LCD_D;
		IOSET1=(cmd&0x0f)<<20;
		IOCLR1=RS;
		IOCLR1=RW;
		IOSET1=E;
		delay_ms(2);
		IOCLR1=E;
}
void lcd_data(unsigned char d )
{
			IOCLR1=LCD_D;
			IOSET1=(d&0xf0)<<16;
			IOSET1=RS;
			IOCLR1=RW;
			IOSET1=E;
			delay_ms(2);
			IOCLR1=E;
			
			IOCLR1=LCD_D;
			IOSET1=(d&0x0f)<<20;
			IOSET1=RS;
			IOCLR1=RW;
			IOSET1=E;
		    delay_ms(2);
		  	IOCLR1=E;
}

void lcd_int(int a)
{
u8 b[10];
int i=0;
	if(a==0){
		lcd_data('0');
		return;
		}
		if(a<0){
		a=-a;
		}
while(a!=0){
b[i++]=a%10;
a=a/10;
}
for(--i;i>=0;i--){
lcd_data(b[i]+48);
}
}
