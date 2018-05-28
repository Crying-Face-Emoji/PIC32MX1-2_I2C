/* 
 * File:   I2C_pic32mx.cpp
 * Author: Rafa
 * 
 * Created on May 21, 2018, 8:13 PM
 */

#include <proc/p32mx150f128b.h>
#include "I2C_pic32mx.h"
//#include "UART_pic32mx.h"

#define F_SCL_NORMAL 100000UL
#define F_SCL_FAST 400000UL

/*
I2Cx_Init
I2Cx_Init_Advanced
I2Cx_Start
I2Cx_Restart
I2Cx_Is_Idle
I2Cx_Read
I2Cx_Write
I2Cx_Stop
 */

void I2C_pic32mx::Init(bool fastmode)
{
	ANSELBCLR = 0x00000300;
	LATBCLR = 0x00000300;
	TRISBCLR = 0x00000300;
//	CNPUBSET = 0x00000300;
	
//	I2C1BRG = (((1/(2*(fastmode ? F_SCL_NORMAL : F_SCL_FAST)))*GetPeripheralClock()) - 2);
	I2C1BRG = 8;
	
//	I2C1CONbits.DISSLW = !(fastmode);
	I2C1CONbits.DISSLW = 0;
	I2C1CONbits.ON = 1;
}

uint8_t I2C_pic32mx::Transmit(uint8_t address, uint8_t* src, uint16_t count, bool stop)
{
	static bool stoplast = 1;
	
//	IFS1bits.I2C1MIF = 0;
	if(stoplast)	/* if last state was a stop bit*/
	{
		I2C1ACONbits.SEN = 1;	/* else restart condition */
		while(I2C1ACONbits.SEN);
//		IFS1bits.I2C1MIF = 0;
	}
	else
	{
		I2C1ACONbits.RSEN = 1;	/* Start condition */
		while(I2C1ACONbits.RSEN);
//		IFS1bits.I2C1MIF = 0;
	}
	
//	UART1.Printf("transmit\n");
	I2C1TRN = address;
	while(I2C1STATbits.TRSTAT);	/* Master transmit is in progress (8 bits + ACK) */
//	IFS1bits.I2C1MIF = 0;
	if(I2C1STATbits.ACKSTAT)		/* 1 = Acknowledge was not received */
	{
		I2C1CONbits.PEN = 1;
		while(I2C1CONbits.PEN);
		return 0;
	}
	
	if(count)
	{
		do
		{
			if (!(address & 1)) // Write
			{
				I2C1TRN = *(src++);
				while(I2C1STATbits.TRSTAT);	/* Master transmit is in progress (8 bits + ACK) */
//				IFS1bits.I2C1MIF = 0;
				if(I2C1STATbits.ACKSTAT)		/* 1 = Acknowledge was not received */
				{
					I2C1CONbits.PEN = 1;
					while(I2C1CONbits.PEN);
//					IFS1bits.I2C1MIF = 0;
					return 0;
				}
			}
			else // Read
			{
				I2C1CONbits.ACKDT =  !(count > 1);
				
//				UART1.Printf("read\n");
				I2C1CONbits.RCEN = 1;
				while(I2C1CONbits.RCEN);
//				IFS1bits.I2C1MIF = 0;

				I2C1CONbits.ACKEN = 1;
				while(I2C1CONbits.ACKEN);
//				IFS1bits.I2C1MIF = 0;
				*(src++) = I2C1RCV;
			}
		} 
		while(--count);
	} 

	stoplast = stop;
	I2C1CONbits.PEN = stop;
	while(I2C1CONbits.PEN);
//	IFS1bits.I2C1MIF = 0;
	
	return 1;
}