/*****************************************************************************************************************
 *[FILE NAME]: used_functions.h
 *
 *[AUTHOR]: Amr Mohamed
 *
 *[DATE CREATED]: 12/12/2020
 *
 *[DESCRIPTION]: This file contains important functions description used in main file (HMI_ECU)
 *
 ************************************************************************************************************************/


#include "used_functions.h"


void LOCK_receiveRightPass(char *a_str_PTR)
{
	/********************************************************************************************************************************
	 * [Function Name] : LOCK_receiveRightPass
	 * [Description] : This function is responsible for receiving the right password saved at first time in EEPROM in CONTOL_ECU
	 *                 the function uses the UART MODULE during this operation to send and receive from CONTROL_ECU
	 * [Args]        :
	 * [in]            a_pointerToCharacter a_str_PTR1
	 *                        The function takes pointer to character as a string is passed to it to receive the string in it using the function
	 *                        UART_receiveString(uint8 * str) .
	 * [Returns]     : The function returns void .
	 *
	 *********************************************************************************************************************************/

	while(UART_receiveByte(&Uart_Config) != FINISHED_PASS){}

	UART_sendByte(M_READY,&Uart_Config);

	UART_receiveString((uint8 *)a_str_PTR,&Uart_Config);
	LCD_clearScreen();
}

uint8 LOCK_enterPass(char *a_str_PTR1 , char*a_str_PTR2 )
{
	/********************************************************************************************************************************
	 * [Function Name] : LOCK_enterPass
	 * [Description] : This function is responsible for getting the password from the user and compare it with the recieved password from
	 *                 CONTROL_ECU , if the password match the function returns 1 , otherwise it diplayes "WRONG PASS!" then "you have i more
	 *                 attempts " , Where i is a counter starts with 2 and decrements to zero as the user enter a wrong password ,
	 *                 if i = 0 , the function returns 0 .
	 * [Args]        :
	 *
	 * [in]             a_pointerToCharacter a_str_PTR1
	 *                     This Arg shall indicate a pointer to character it points to the entered string from the user .
	 * [in]             a_pointerToCharacter a_str_PTR2
	 *                     This Arg shall indicate a pointer to character it points to the received string from EEPROM in CONTROL_ECU
	 * [Returns]     : The return shall indicate whether the password entered is right , or the user entered wrong pass for 3 times in a row
	 *
	 *
	 *********************************************************************************************************************************/
	uint8 i = 2 ;
	LOCK_pass(a_str_PTR1 , "Enter Your Pass");
	a_str_PTR1[5] = '\0';
	while(strcmp(a_str_PTR1,a_str_PTR2)!=0)
	{
		if(i==0)
		{
			return 0 ;
		}
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"WRONG PASS!");
		_delay_ms(4800);
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"you have ");
		LCD_goToRowColumn(0,10);
		LCD_intgerToString(i);
		LCD_displayStringRowColumn(1,0,"more attempts");
		_delay_ms(4800);

		LOCK_pass(a_str_PTR1, "Enter Your pass");
		a_str_PTR1[5] = '\0';
		i--;
	}

	return 1 ;

}

void LOCK_sendNewPass(char *a_str_PTR1 , char *a_str_PTR2)
{
	/********************************************************************************************************************************
	 * [Function Name] : LOCK_sendNewPass
	 * [Description] : This function is responsible for getting the new password from the user then send it to the CONTOL_ECU to save it in the EEPROM, First
	 *                 the LCD display "EnterNewPassword" and then asks the user to reenter and displays "Re_enterPassword" , the using the function
	 *                 strcmp it compares the two passwords enterd by the user ,if matched it will display "saving.." during the send time to CONTROL_ECU
	 *                 ,If not matched it continues to ask the user to enter a new password then reenter until matching
	 * [Args]        :
	 *
	 * [in]            a_pointerToCharacter a_str_PTR1
	 *                     This Arg shall indicate a pointer to character it points to the entered string from the user .
	 * [in]            a_pointerToCharacter a_str_PTR2
	 *                     This Arg shall indicate a pointer to character it points to the reentered string from the user.
	 * [Returns]     : The function returns void.
	 *
	 *
	 *********************************************************************************************************************************/

	LOCK_pass(a_str_PTR1,"EnterNewPassword");
	LOCK_pass(a_str_PTR2,"Re_enterPassword");


	while(strcmp(a_str_PTR1,a_str_PTR2) != 0)
	{

		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"Unmatched");
		_delay_ms(3000);
		LCD_clearScreen();
		LCD_displayStringRowColumn(0,0,"please re_enter");
		_delay_ms(3000);
		LOCK_pass(a_str_PTR1,"EnterNewPassword");
		LOCK_pass(a_str_PTR2,"Re_enterPassword");

	}
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,"saving...");
	UART_sendByte(FINISHED_NEW_PASS,&Uart_Config);
	while(UART_receiveByte(&Uart_Config) != M_READY ){}


	UART_sendString((uint8*)a_str_PTR1,&Uart_Config);


}

void LOCK_pass(char *a_str_PTR , char *a_lcdDisplayStr_PTR)
{
	/********************************************************************************************************************************
	 * [Function Name] : LOCK_sendNewPass
	 * [Description] : This function is responsible for getting the new password from the user then send it to the CONTOL_ECU to save it in the EEPROM, First
	 *                 the LCD display "EnterNewPassword" and then asks the user to reenter and displays "Re_enterPassword" , the using the function
	 *                 strcmp it compares the two passwords enterd by the user ,if matched it will display "saving.." during the send time to CONTROL_ECU
	 *                 ,If not matched it continues to ask the user to enter a new password then reenter until matching
	 * [Args]        :
	 *
	 * [in]            a_pointerToCharacter a_str_PTR1
	 *                     This Arg shall indicate a pointer to character it points to the entered string from the user .
	 * [in]            a_pointerToCharacter a_str_PTR2
	 *                     This Arg shall indicate a pointer to character it points to the reentered string from the user.
	 * [Returns]     : The function returns void.
	 *
	 *
	 *********************************************************************************************************************************/
	uint8 i = 0 ;   // counter
	LCD_clearScreen();
	LCD_displayStringRowColumn(0,0,a_lcdDisplayStr_PTR);

	while(i<5)
	{
		a_str_PTR[i] = (char) KeyPad_getPressedKey();         // store value from keypad
		LCD_goToRowColumn(1,i);
		LCD_displayCharacter('*');
		i++;
		_delay_ms(2500);
	}
	a_str_PTR[i]='#';
	a_str_PTR[i+1]='\0';
	while(KeyPad_getPressedKey() != 13){}           //wait until user press enter
	_delay_ms(2500);
}
void LOCK_timerCount(uint8 a_time)
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
	while(g_timer != a_time){}
	Timer0_DeInit();
	g_sec = 0;
	g_timer = 0 ;
}
