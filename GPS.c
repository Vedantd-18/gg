//#include "gps.h"
//#include"delay.h" #include "lpc214x.h"
//#include "uart.h"
void delay_fv(unsigned int x) // delay inducing loop
{
int i,j; for(i=0;i<=x;i++) for(j=0;j<=100;j++);

}
int main()
{
int pclk=12000000, value; PINSEL0|=0x05; IO0DIR|=0x01; VPBDIV|=0x01;
U0FCR=0x07; //FIFo control register - controls the operation of the uart rx and tx fifo's
U0LCR=0x83; // line control register - determines the data format of the data character that is to be transmitted and received
U0FDR=0xC1; //fractional divider - controls the clk prescaler for thr baud rate genratio value=pclk/(16*115200);
U0DLL=value & 0x0FF; //divisor latch lsb - holds the value used to divide the clk which came from fractional prescaler
U0DLM=(value & 0xFF00)>>8; // division latch msb - '' [ lsb hold the lower 8 bits and msb reg holds the upper/higher 8 bits which in total hold 16 bit divisor]
U0TER=0x80;// transmit enable register
U0LCR=0x03;// line control register - must be the one to acess the uart divisor latches

//ConfigUart(Uart1,9600); PINSEL0|=0x05<<16; IO0DIR|=0x01<<8;
// VPBDIV|=0x01; U1FCR=0x07; U1LCR=0x83;
value=pclk/(16*9600); U1DLL=value & 0x0FF; U1DLM=(value & 0xFF00)>>8;
U1TER=0x80; U1LCR=0x03;
//ConfigUart(Uart1,9600)
//ConfigUart(Uart0,115200);
delay_fv(500);
while(1)
{
while(!(U1LSR & 0x01)); char data1= U1RBR; U0THR=data1; while(!(U0LSR & 0x20));

}

}
