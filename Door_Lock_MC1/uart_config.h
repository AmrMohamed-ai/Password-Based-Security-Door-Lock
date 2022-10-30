 /******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.h
 *
 * Description: Header file for the UART AVR driver
 *
 * Author: Amr Mohamed
 *
 *******************************************************************************/

#ifndef UART_CONFIG_H_
#define UART_CONFIG_H_

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

/*******************************************************************************
 *                      Preprocessor Macros                                    *
 *******************************************************************************/


typedef enum
{
	Disabled,Reserved,Even,Odd
}Uart_ParityType;

typedef enum
{
	ASynchronous,synchronous
}Uart_Mode;

typedef enum
{
	NormalSpeed,DoubleSpeed
}Uart_Speed;
typedef enum
{
	TxRising_RxFalling,TxFalling_RxRising

}Uart_Clock_Polarity;
typedef enum
{
	ONE_STOP_BIT,TWO_STOP_BIT
}Uart_StopBitsNum;


typedef struct
{
	uint8 NumOfDataBits;
	uint16 BaudRate;
	Uart_ParityType ParityType;
	Uart_StopBitsNum StopBitsNum;
	Uart_Mode Mode_of_operation;
	Uart_Speed Speed_of_operation;
	Uart_Clock_Polarity Clock_polarity;
}Uart_ConfigType;

/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void UART_init(const Uart_ConfigType * Config_Ptr);

void UART_sendByte(const uint16 data,const Uart_ConfigType * Config_Ptr);

uint16 UART_receiveByte(const Uart_ConfigType * Config_Ptr);

void UART_sendString(const uint8 *Str , const Uart_ConfigType * Config_Ptr);

void UART_receiveString(uint8 *Str ,const Uart_ConfigType * Config_Ptr); // Receive until #

#endif /* UART_CONFIG_H_ */
