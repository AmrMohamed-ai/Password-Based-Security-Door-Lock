/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: Source file for the UART AVR driver
 *
 * Author: Amr Mohamed
 *
 *******************************************************************************/

#include "UART_Config.h"


/*******************************************************************************
 *                      Functions Definitions                                  *
 *******************************************************************************/
void UART_init(const Uart_ConfigType * Config_Ptr)
{
	uint16 BAUD_PRESCALE = (((F_CPU / (Config_Ptr->BaudRate * 8UL))) - 1);

	/* U2X = 1 for double transmission speed */
	if(Config_Ptr->Speed_of_operation==DoubleSpeed)
		UCSRA = (1<<U2X);
	else
		UCSRA=0;
	/************************** UCSRB Description **************************
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART Tx Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * TXEN  = 1 Transmitter Enable
	 ***********************************************************************/
	UCSRB = (1<<RXEN) | (1<<TXEN);

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 ***********************************************************************/
	SET_BIT(UCSRC,URSEL);
	UCSRC=(UCSRC&0xBF) | ((Config_Ptr->Mode_of_operation)<<6);

	UCSRC=(UCSRC&0xCF) | ((Config_Ptr->ParityType)<<4);

	UCSRC=(UCSRC&0xFC) | ((Config_Ptr->StopBitsNum)<<3);

	UCSRC=(UCSRC&0xFE) | (Config_Ptr->Clock_polarity);

	if (Config_Ptr->NumOfDataBits == 6)
	{
		SET_BIT(UCSRC,UCSZ0);
	}
	else if (Config_Ptr->NumOfDataBits == 7)
	{
		SET_BIT(UCSRC,UCSZ1);
	}
	else if (Config_Ptr->NumOfDataBits == 8)
	{
		SET_BIT(UCSRC,UCSZ0);
		SET_BIT(UCSRC,UCSZ1);
	}
	else if (Config_Ptr->NumOfDataBits == 9)
	{
		SET_BIT(UCSRB,UCSZ2);
		SET_BIT(UCSRC,UCSZ1);
		SET_BIT(UCSRC,UCSZ0);
	}


	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = BAUD_PRESCALE>>8;
	UBRRL = BAUD_PRESCALE;
}

void UART_sendByte(const uint16 data,const Uart_ConfigType * Config_Ptr)
{
	if ((Config_Ptr->NumOfDataBits) == 9)
	{
		/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
		 * transmitting a new byte so wait until this flag is set to one */
		while(BIT_IS_CLEAR(UCSRA,UDRE)){}
		/*writing the ninth bit in the transmitted data to TXB0 bit in Register UCSRB*/
		UCSRB=(UCSRB&0xFE) | ((data&0x0100)>>8) ;
		/*Writing the first eight bits in the transmitted data to UDR Register*/
		UDR = (data&0x00FF) ;

	}
	else
	{
		/* UDRE flag is set when the Tx buffer (UDR) is empty and ready for
		 * transmitting a new byte so wait until this flag is set to one */
		while(BIT_IS_CLEAR(UCSRA,UDRE)){}
		/* Put the required data in the UDR register and it also clear the UDRE flag as
		 * the UDR register is not empty now */
		UDR = data;
	}
	/************************* Another Method *************************
	UDR = data;
	while(BIT_IS_CLEAR(UCSRA,TXC)){} // Wait until the transimission is complete TXC = 1
	SET_BIT(UCSRA,TXC); // Clear the TXC flag
	 *******************************************************************/
}

uint16 UART_receiveByte(const Uart_ConfigType * Config_Ptr)
{
	uint16 RecievedData;
	if (Config_Ptr->NumOfDataBits == 9)
	{
		/* RXC flag is set when the UART receive data so wait until this
		 * flag is set to one */
		while(BIT_IS_CLEAR(UCSRA,RXC)){}

		RecievedData=((UCSRB&0x02)>>1)<<8;
		RecievedData=(RecievedData&0x0100) | (UDR&0x00FF) ;
		return RecievedData ;

	}
	else
	{
		/* RXC flag is set when the UART receive data so wait until this
		 * flag is set to one */
		while(BIT_IS_CLEAR(UCSRA,RXC)){}
		/* Read the received data from the Rx buffer (UDR) and the RXC flag
	   will be cleared after read this data */
		RecievedData=UDR;
		return RecievedData;
	}
}

void UART_sendString(const uint8 *Str , const Uart_ConfigType * Config_Ptr)
{
	uint8 i = 0;
	while(Str[i] != '\0')
	{
		UART_sendByte(Str[i] , Config_Ptr);
		i++;
	}
	/************************* Another Method *************************
	while(*Str != '\0')
	{
		UART_sendByte(*Str);
		Str++;
	}
	 *******************************************************************/
}

void UART_receiveString(uint8 *Str , const Uart_ConfigType * Config_Ptr)
{
	uint8 i = 0;
	Str[i] = UART_receiveByte(Config_Ptr);
	while(Str[i] != '#')
	{
		i++;
		Str[i] = UART_receiveByte(Config_Ptr);
	}
	Str[i] = '\0';
}
