A: ADC with interrupt
#include "LPC214x.h"
#include "VIClowlevel.h"

#define RS (1<<5)
#define RW (1<<6)
#define EN (1<<7)
void delay_fv(unsigned int x,int y);
void lcd_display(unsigned int x);
void cmd (unsigned int x);
void lcd_ini(void);
void lcd_str(unsigned char x[]);
//void lcd_str(unsigned char *x);
void DisplayHexLcd(int LcdData);
unsigned int AdcData =0;


void __attribute__((interrupt("IRQ"))) ADC0_routine(void)
{
	ISR_ENTRY();
	AdcData = (AD0GDR & 0x0000FFC0)>>6;
	DisplayHexLcd(AdcData);
	AD0INTEN = 0x00;
	VICVectAddr = 0x00;
	ISR_EXIT();
}

int main()
{
	int Result;

	PINSEL0 = 0x00000000;
//	PINSEL1 = 0x01000000;
	VPBDIV = 0x01;

	IODIR0 = 0XFFE0;
//	AD0CR = 0X01200402;
//	AD0GDR = 0X01000000;
	lcd_ini();
	cmd(0x80);
	lcd_str ("ADC o/p:");
	cmd (0x8B);
	lcd_display ('H');

//	AD0CR = 0x01200402;
//	AD0CR = AD0CR|(1<<24);
	while(1)
	{
		VICIntEnable |= (0x01 << 18);
		VICVectAddr0 = (unsigned) ADC0_routine;
		VICVectCntl0 = (0x32);
		enableIRQ();
		PINSEL1 = 0x1000000;
		VPBDIV = 0x01;
     	AD0CR = 0X01200402;
	    ADGSR = 0X01000000;
		AD0INTEN = 0x02;

	}
	Result = ((AD0GDR & 0x0000FFC0)>>6);
	DisplayHexLcd(Result);
	while(1);

}
void lcd_display(unsigned int x)
{
	IOCLR0 = 0x0000FF00;
	x =(x<<8);
	IOSET0 = x;
	IOSET0 =RS;
	IOCLR0 =RW;
	IOSET0 =EN;
	delay_fv(100,10);
	IOCLR0 =EN;
}
void delay_fv(unsigned int x,int y)
{
	unsigned int i,j;
	for(i=0;i<x;i++)
	for(j=0;j<y;j++);
}
void cmd (unsigned int x)
{
	IOCLR0 = 0x0000FF00;
	x =(x<<8);
	IOSET0 = x;
	IOCLR0 =RS;
	IOCLR0 =RW;
	IOSET0 =EN;
	delay_fv(100,10);
	IOCLR0 =EN;
}

void lcd_ini()
{
	cmd(0X38);
	cmd(0X0e);
	cmd(0X01);
	cmd(0X06);
}

void lcd_str(unsigned char x[])
{
	int i;
	for(i=0;x[i]!='\0';i++)
	{
		lcd_display(x[i]);
	}
}

void DisplayHexLcd(int LcdData)
{
    unsigned char Character[17]={"0123456789ABCDEF"};
    unsigned int DivValue=0x100,BaseValue=0x10;
    char j = 0;
    while(DivValue)
    {
		cmd(0x88+j++);
    	lcd_display(Character[LcdData/DivValue]);
        LcdData%=DivValue;
        DivValue/=BaseValue;

     }
}
