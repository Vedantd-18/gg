#include "includes.h"
#include "edutech.h"
#include "uart.h"
#include "lcd.h"

#define UART_DEBUG   0



/* Task1 Stack */
OS_STK Task1Stack[100];
void Task1(void *pdata);

/* Task2 Stack */
OS_STK Task2Stack[100];
void Task2(void *pdata);

/* Task3 Stack */
OS_STK Task3Stack[100];
void Task3(void *pdata);


/* Main Program */
int  main (void)
{
	timer_init();	// initialize OS Timer Tick

	OSInit();		// Initialize uC/OS-II



	OSTaskCreate(Task1, (void *)0, &Task1Stack[99], 1);		// Create task1
   	OSTaskCreate(Task2, (void *)0, &Task2Stack[99], 2);		// Create task2
	OSTaskCreate(Task3, (void *)0, &Task3Stack[99], 3);		// Create task3

	/* start the multitasking process which lets uC/OS-II manages the task that you have created */
	OSStart();
	return 0;
}


/* Task Definition */
/**
 * Task1 to Print A to Z on LCD line1
 */
void Task1(void *pdata)
{
	int i=0;
	Lcd_Init();               // Initialize LCD in 8bit mode
	Lcd_Cmd(0x01);            // LCD clear cmd
	Lcd_Cmd(0x80);  		  // LCD Line1 cmd
	Lcd_String("T1 ");

	while(1)
	{
		Lcd_Cmd(0x83);  		  // LCD Line1 cmd
		Lcd_Data(0x41 + i++);
		if(i==26) i=0;
	 	OSTimeDlyHMSM(0, 0, 0, 500);	//delay 500ms
	}
}


/**
 * Task2 to Print 0 to 9 on LCD line2
 */
void Task2(void *pdata)
{
	int i=0;
//	Lcd_Init();               // Initialize LCD in 8bit mode
	Lcd_Cmd(0xC0);  		  // LCD Line2 cmd
	Lcd_String("T2 ");

	while(1)
	{
		Lcd_Cmd(0xC3);  		  // LCD Line2 cmd
		Lcd_Data(0x30 + i++);
		if(i==10) i=0;
		OSTimeDlyHMSM(0, 0, 0, 500);	// Delay 1s
	}
}


/**
 * Task3 to Print 0 to 9999 on UART0
 */
void Task3(void *pdata)
{
	int i=0;
	Uart0_Init(9600);		// Initialize UART
 	while(1)
 	{
		uprintf("\x1b[1;1HTask3 %d04",i++);		// print on UART0
		if(i==9999) i=0;
	 	OSTimeDlyHMSM(0, 0, 0, 700);	// Delay 700ms
	}
}
