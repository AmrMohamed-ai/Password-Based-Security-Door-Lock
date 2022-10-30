/*
 * timer0_config.c
 *
 *
 */


#include "timer0_config.h"
static volatile void (*g_callBackPtr_OVF)(void) = NULL_PTR;
static volatile void (*g_callBackPtr_CMP)(void) = NULL_PTR;



ISR(TIMER0_OVF_vect)
{
	if(g_callBackPtr_OVF != NULL_PTR)
	{
		(*g_callBackPtr_OVF)(); /* another method to call the function using pointer to function g_callBackPtr_OVF(); */
	}
}

ISR(TIMER0_COMP_vect)
{
	if(g_callBackPtr_CMP != NULL_PTR)
	{
		(*g_callBackPtr_CMP)(); /* another method to call the function using pointer to function g_callBackPtr_CMP(); */
	}
}



void Timer0_init (const Timer0_ConfigType * ConfigPtr)
{
	if (ConfigPtr->Mode == NORMAL_MODE)
	{
		TCNT0 = ConfigPtr->InitialValue;
		SET_BIT(TIMSK,TOIE0);                        // overflow INT Enable
		/* Configure the timer control register
			 * 1. Non PWM mode FOC0=1
			 * 2. Normal Mode WGM01=0 & WGM00=0
			 * 3. Normal Mode COM00=0 & COM01=0
			 */
		SET_BIT (TCCR0,FOC0);
		TCCR0 =(TCCR0&0xF8) | (ConfigPtr->Clock);  // clock
	}


	else if (ConfigPtr->Mode == CTC_MODE)
	{
		TCNT0 = ConfigPtr->InitialValue;
		OCR0 = ConfigPtr->CompareValue;
		SET_BIT (TIMSK,OCIE0);  // Enable Timer0 Compare Interrupt
		/* Configure timer0 control register
			 * 1. Non PWM mode FOC0=1
			 * 2. CTC Mode WGM01=1 & WGM00=0
			 * 3. No need for OC0 so COM00=0 & COM01=0 , "we'll do so using PWM mode"
			 */
		SET_BIT(TCCR0,FOC0);
		SET_BIT(TCCR0,WGM01);

		TCCR0 =(TCCR0&0xF8) | (ConfigPtr->Clock); // clock
	}


	else if (ConfigPtr->Mode == PWM_MODE)
	{
		TCNT0 = ConfigPtr->InitialValue;
		OCR0 = ConfigPtr->DutyCycle;
		DDRB  = DDRB | (1<<PB3); // Configure PB3/OC0 as output pin --> pin where the PWM signal is generated from MC

		/* Configure timer control register
		 * 1. Fast PWM mode FOC0=0
		 * 2. Fast PWM Mode WGM01=1 & WGM00=1
		 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
		 */
		SET_BIT (TCCR0,WGM00);
		SET_BIT (TCCR0,WGM01);
		SET_BIT (TCCR0,COM01);


		TCCR0 =(TCCR0&0xF8) | (ConfigPtr->Clock); // clock
	}


}



void Timer0_OVF_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_OVF = a_ptr;
}



void Timer0_CMP_setCallBack(void(*a_ptr)(void))
{
	/* Save the address of the Call back function in a global variable */
	g_callBackPtr_CMP = a_ptr;
}


void Timer0_DeInit(void)
{
	TCNT0=0;
	TCCR0=0;
	CLEAR_BIT(TIMSK,TOIE0);    // Disable Overflow INT
	CLEAR_BIT (TIMSK,OCIE0);   // Disable Timer0 Compare Interrupt
}
