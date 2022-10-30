/*
 * Dc_Motor.h
 *
 *  Created on: Nov 6, 2020
 *      Author: Amr
 */

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#define MOTOR_CTRL_DIR DDRB        /* Connect motor to port b */
#define MOTOR_PORT PORTB
#define EN1 PB4                    /* Motor Enable */
#define IN1 PB2                   /* Motor control in1 */
#define IN2 PB3                    /* Motor control in2 */

void MOTOR_reverse( void );        /* Reverse Motor motion */
void MOTOR_forward( void );
void MOTOR_init( void );           /* Motor initialization */
void MOTOR_stop(void);

#endif /* DC_MOTOR_H_ */
