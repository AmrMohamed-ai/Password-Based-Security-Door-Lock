/*
 * timer0_config.h
 *
 *
 *
 */

#ifndef TIMER0_CONFIG_H_
#define TIMER0_CONFIG_H_

#include "std_types.h"
#include "common_macros.h"
#include "micro_config.h"
#define NULL_PTR    ((void*)0)

typedef enum
{
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}Timer0_Clock;


typedef enum
{
	NORMAL_MODE,CTC_MODE,PWM_MODE
}Timer0_Mode;



typedef struct
{
	 Timer0_Mode Mode;
	 uint8 InitialValue;
	 Timer0_Clock Clock;
	 uint8 CompareValue;
	 uint8 DutyCycle;
}Timer0_ConfigType;




void Timer0_init(const Timer0_ConfigType * Config_Ptr);

void Timer0_OVF_setCallBack(void(*a_ptr)(void));

void Timer0_CMP_setCallBack(void(*a_ptr)(void));

void Timer0_DeInit(void);




#endif /* TIMER0_CONFIG_H_ */
