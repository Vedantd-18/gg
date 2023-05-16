#include "lpc214x.h" #define FOSC 12000000 void Transm(char); char Receivem();

int main()
{
//char data
PINSEL0 = 0X00000005; VPBDIV = 0x01; IO0DIR = 0x01;
U0FCR = 0x07; U0LCR = 0x87; U0DLL = 0x4E; U0DLM = 0x00; U0LCR = 0x07; U0TER = 0x80;
Transm('A'); while(1)


{
char k = Receiver(); Transm(k);
}
return 0;
}


void Transm(char a)
{
U0THR=a;
while (!(U0LSR & 0x20));
}


char Receivem()
{
while(!(U0LSR & 0x01)); char b = U0RBR;
return b;
}
