
#define F_CPU 8000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "osa.h"
#include "OSAcfg.h"

ISR(TIMER0_OVF_vect)
{
	//here interrupt happens
	OS_Timer(); //add system ticks by one in OSA Kernal
}

void T0_Initialize()
{
	TCCR0 = 0x03; //using Normal mode, No pre-scalar
	TCNT0 = 0x00;
	
	TIMSK |= (1<<TOIE0); //T0 overflow interrupt enable
}

void Task1()
{
	DDRB |= (1<<0); //make Pin0 in PORTB is output
	
	while(1)
	{
		PORTB ^= (1<<0); //Toggle Pin0 in PORTB
		OS_Delay(500); //wait 500 ticks = 1s (1 system tick equals to 2ms by default) + [Task1 here is waiting]
	}
}

void Task2()
{
	DDRC |= (1<<0); //make Pin0 in PORTB is output
	
	while(1)
	{
		PORTC ^= (1<<0); //Toggle Pin0 in PORTB
		OS_Delay(1000); //wait 2s (1 system tick equals to 2ms by default) + [Task2 here is waiting]
	}
}

int main(void)
{
	OS_Init(); //must be called in the beginning
	
	T0_Initialize(); //Initialize T0 overflow interrupt
	sei(); //Enable global interrupts by setting global interrupt enable bit in SREG
 
	//here Task1 & Task2 are not created yet, so they are (not active)
	OS_Task_Create(0, Task1); //Task1 is ready, Task2 is not active
	OS_Task_Create(1, Task2); //Task1 is (ready or waiting or running), Task2 is ready
	//both tasks are running in non-sequential way
 
	OS_Run(); //must be called in the end, start scheduler to manage tasks
	return 0;
}

