/*
 * Dc_Motor.c
 *
 *  Created on: Nov 6, 2020
 *      Author: Amr
 */

#include "DC_Motor.h"

void MOTOR_init( void )
{
	MOTOR_CTRL_DIR |= (1<<IN1) | (1<<IN2) |(1<<EN1);

}

void MOTOR_forward( void )
{
	MOTOR_PORT |= (1<<IN1)  |(1<<EN1);
	MOTOR_PORT &= ~(1<<IN2);

}

/* Toggle Motor Input pins to reverse its motion */
void MOTOR_reverse( void )
{
	MOTOR_PORT ^= (1<<IN1);
	MOTOR_PORT ^= (1<<IN2);
	MOTOR_PORT |= (1<<EN1);
}

void MOTOR_stop(void)
{
	MOTOR_PORT &= ~(1<<EN1);
}
