/*****************************************************************************************************************
 *[FILE NAME]: used_functions.h
 *
 *[AUTHOR]: Amr Mohamed
 *
 *[DATE CREATED]: 12/12/2020
 *
 *[DESCRIPTION]: This file contains important functions description used in main file (CONTROL_ECU)
 *
 ************************************************************************************************************************/


#include "used_functions2.h"

void LOCK_receiveAndSaveNewPass(char * a_str_PTR)
{
	/********************************************************************************************************************************
	 * [Function Name] : LOCK_receiveAndSaveNewPass
	 * [Description] : This function is responsible for receiving the password entered by the user in HMI_ECU , then save it in EEPROM
	 *                 the function uses the UART MODULE during this operation to send and receive from HMI_ECU.
	 * [Args]        :
	 * [in]            a_pointerToCharacter a_str_PTR1
	 *                        The function takes pointer to character as a string is passed to it to receive the string in it using the function
	 *                        UART_receiveString(uint8 * str) .
	 * [Returns]     : The function returns void .
	 *
	 *********************************************************************************************************************************/
	uint8 i = 0 ; // counter
	while(UART_receiveByte(&Uart_Config) != FINISHED_NEW_PASS){}
	UART_sendByte(M_READY,&Uart_Config);

	UART_receiveString((uint8 *)a_str_PTR , &Uart_Config);


	for(i=0 ; i<5; i++)
	{
		EEPROM_writeByte(PASS_ADDRESS_EEPROM+i,(uint8)a_str_PTR[i]);
		_delay_ms(600);   // writing time

	}
}

void LOCK_sendRightPass(char * a_str_PTR1 )
{
	/********************************************************************************************************************************
	 * [Function Name] : LOCK_sendRightPass
	 * [Description] : This function is responsible for sending the right password saved at first time in EEPROM in CONTOL_ECU
	 *                 the function uses the UART MODULE during this operation to send and receive from HMI_ECU
	 * [Args]        :
	 * [in]            a_pointerToCharacter a_str_PTR1
	 *                        The function takes pointer to character as a string is passed to it to send the string in it using the function
	 *                        UART_sendString(uint8 * str) .
	 * [Returns]     : The function returns void .
	 *
	 *********************************************************************************************************************************/
	uint8 i ;
	char c ;
	for(i=0 ; i <5; i++)
	{
		EEPROM_readByte(PASS_ADDRESS_EEPROM+i,(uint8 *)&c);
		_delay_ms(100);
		a_str_PTR1[i]=c;
	}
	a_str_PTR1[i] = '#' ;
	a_str_PTR1[i+1] = '\0';
	UART_sendByte(FINISHED_PASS,&Uart_Config);
	while(UART_receiveByte(&Uart_Config)!=M_READY){}
	UART_sendString((uint8*)a_str_PTR1,&Uart_Config);

}
void LOCK_timerCount(uint8 time)
{
	/********************************************************************************************************************************
	 * [Function Name] : LOCK_timerCount
	 * [Description] : This function is responsible for counting seconds using the TIMER0 MODULE in AVR MCP
	 * [Args]        :
	 *
	 * [in]            a_time
	 *
	 *
	 * [Returns]     : The function returns void.
	 *
	 *
	 *********************************************************************************************************************************/
	Timer0_init (&Timer0_Config);
	while(g_timer != time){}
	Timer0_DeInit();
	g_sec = 0;
	g_timer = 0 ;
}
