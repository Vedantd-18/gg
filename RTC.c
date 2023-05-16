#include"LPC214x.h"

#define LCD (0xff<<8)
#define RS (1<<5)
#define RW (1<<6)
#define EN (1<<7)

void delay_ms(unsigned int x,int y);
void LCD_data(unsigned int x);
void LCD_cmd (unsigned int x);
void LCD_ini();
void LCD_str(char x[]);
void RTC_init();
int hextodec(int x);

int main()
{
	PINSEL0=0X00000000;
	IODIR0=0XFFFFFFFF;

	int hour, min, sec, day, month, year;

	LCD_ini();
	RTC_init();
	LCD_cmd(0x80);
	LCD_str("Time:");
	LCD_cmd(0xC0);
	LCD_str("Date:");
while(1){
	hour= hextodec(HOUR);
	min= hextodec(MIN);
	sec= hextodec(SEC);
	day= hextodec(DOM);
	month= hextodec(MONTH);
	year= hextodec(YEAR);
delay_ms(100,10);
	LCD_cmd(0x85);
	LCD_data((hour/10)+'0');
	LCD_data((hour%10)+'0');
	LCD_cmd(0x87);
	LCD_data(':');
	LCD_cmd(0x88);
	LCD_data((min/10)+'0');
	LCD_data((min%10)+'0');
	LCD_cmd(0x8A);
	LCD_data(':');
	LCD_cmd(0x8B);
	LCD_data((sec/10)+'0');
	LCD_data((sec%10)+'0');

	LCD_cmd(0xC5);
	LCD_data((day/10)+'0');
	LCD_data((day%10)+'0');
	LCD_cmd(0xC7);
	LCD_data('/');
	LCD_cmd(0xC8);
	LCD_data((month/10)+'0');
	LCD_data((month%10)+'0');
	LCD_cmd(0xCA);
	LCD_data('/');
	LCD_cmd(0xCB);
	LCD_data((year/10)+'0');
	LCD_data((year%10)+'0');

}
}

void RTC_init()
{
	CCR=0x11;
	HOUR= 12;
	MIN= 8;
	SEC= 10;
	DOM= 23;
	MONTH= 03;
	YEAR= 23;
}

int hextodec (int value)
{
	int temp;
	temp=  value & 0x0f;
	value= (value & 0xf0)>>4;
	value= value*16;
	if (temp <= 9)
	{
		value = value + temp;
	}
	else if (temp > 9)
	{
		switch (temp)
		{
			case 0x0a:
				value= value + 10;
				break;

			case 0x0b:
				value= value + 11;
				break;

			case 0x0c:
				value= value + 12;
				break;

			case 0x0d:
				value= value + 13;
				break;

			case 0x0e:
				value= value + 14;
				break;

			case 0x0f:
				value= value + 15;
				break;
		}
	}
	return(value);
}

void LCD_data(unsigned int x)
{
	IOCLR0 = 0x0000FF00;
	x =(x<<8);
	IOSET0 = x;
	IOSET0 =RS;
	IOCLR0 =RW;
	IOSET0 =EN;
	delay_ms(100,10);
	IOCLR0 =EN;
}

void LCD_cmd (unsigned int x)
{
	IOCLR0 = 0x0000FF00;
	x =(x<<8);
	IOSET0 = x;
	IOCLR0 =RS;
	IOCLR0 =RW;
	IOSET0 =EN;
	delay_ms(100,10);
	IOCLR0 =EN;
}

void LCD_ini()
{
	LCD_cmd(0X38);
	LCD_cmd(0X0e);
	LCD_cmd(0X01);
	LCD_cmd(0X06);
}

void LCD_str(char x[])
{
	int i;
	for(i=0;x[i]!='\0';i++)
	{
		LCD_data(x[i]);
	}
}

void delay_ms(unsigned int x,int y)
{
	unsigned int i,j;
	for(i=0;i<x;i++)
	for(j=0;j<y;j++);
}
