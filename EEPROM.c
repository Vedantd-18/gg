#include "LPC214x.h" #include "lcd.h
#define PCLK 12000000
void DelayMs(unsigned int Ms); void ConfigI2c0(int BaudRate);
void WriteEeprom0(char SlaveAddress, unsigned char *Data,char len); void ReadEeprom0(char SlaveAddress, unsigned char *Data, char len); unsigned char addr= 0x00;
int main (void)
{
unsigned char EepromBufA[]={0x00,'E','C','E'}; unsigned char EepromBufB[2]={0x00,0}; unsigned int No;

edutechlcdinit(); ConfigI2c0(100);

WriteEeprom0(0xA0,EepromBufA,4); edutechlcdstring("Write Success",1,0);

for(No=0; No<4; No++)
{
ReadEeprom0(0xA0,EepromBufB,1); edutechlcdstring(&EepromBufB[0],2,No); DelayMs(100); // This delay is important addr++;
}
while(1);


return 0;
}


void DelayMs(unsigned int Ms)
{
Ms = Ms * 200; while (Ms)
--Ms;
}


void ConfigI2c0(int BaudRate)
{
PINSEL0 = 0x050;
I2C0CONCLR = 0x6c; // disable I2C I2C0CONSET = 0x40; // Enable I2C

I2C0SCLH = (PCLK/(2*(BaudRate*1000))); I2C0SCLL = (PCLK/(2*(BaudRate*1000)));
}


unsigned char I2C_WaitStatus0(unsigned char I2CStatus)
{
unsigned int Time,I2C_WAIT_TIME_OUT; I2C_WAIT_TIME_OUT = 200;
Time = 0;


while (Time++ < I2C_WAIT_TIME_OUT)
{
if (I2C0CONSET & 8) // poll SI bit for Comm complete
{


if (I2C0STAT == I2CStatus)// read I2C status value
{
Time = 0;
return 1;
}
}
}
return 0;
}


char WriteDataI2c0(char SlaveAddress, char *Data,unsigned char len)
{
I2C0CONCLR = 0x2c; // disable I2C I2C0CONSET = 0x40; // Enable I2C I2C0CONSET |= 0x20;

if (!I2C_WaitStatus0(0x08)) // 0x08: ready for device address return 0;

I2C0DAT = SlaveAddress; I2C0CONCLR = 0x2C;
if (!I2C_WaitStatus0(0x18)) // 0x08: ready for device address return 0;

I2C0DAT = *Data++; I2C0CONCLR = 0x2C;
if (!I2C_WaitStatus0(0x28)) // 0x08: ready for device address return 0;


while(len)
{ I2C0CONCLR=0x2C;

if (!I2C_WaitStatus0(0x28)) // 0x08: ready for device address return 0;

I2C0DAT = *Data++ ;


if(len>1)
I2C0CONSET = 0x04 | 0x40;
else
I2C0CONSET = (0x10 | 0x40);


len--;
}


I2C0CONSET = (0x10 | 0x40); // Send Stop bit. I2C0CONCLR=0x2C;

return(1);
}


char ReadDataI2c0(char SlaveAddress, char *Data, char len)
{


I2C0CONCLR = 0x2c; // disable I2C I2C0CONSET = 0x40; // Enable I2C I2C0CONSET |= 0x20;


if (!I2C_WaitStatus0(0x08)) // 0x08: ready for device address return 0;

I2C0DAT = SlaveAddress; I2C0CONCLR = 0x28;
if (!I2C_WaitStatus0(0x40)) // 0x08: ready for device address return 0;

while(len)
{ I2C0CONCLR=0x2C;

if (!I2C_WaitStatus0(0x58)) // 0x08: ready for device address return 0;

*Data++ = I2C0DAT;


if(len>1)
I2C0CONSET = 0x04 | 0x40;
else
I2C0CONSET = (0x10 | 0x40);
len--;
}


I2C0CONSET = (0x10 | 0x40); // Send Stop bit. I2C0CONCLR=0x2C;

return(1);
}
void WriteEeprom0(char SlaveAddress, unsigned char *Data,char len)


{
WriteDataI2c0(SlaveAddress,Data,len);
}


void ReadEeprom0(char SlaveAddress, unsigned char *Data, char len)
{
WriteDataI2c0(SlaveAddress,&addr,0); ReadDataI2c0((SlaveAddress + 0x01),Data,len);
}

