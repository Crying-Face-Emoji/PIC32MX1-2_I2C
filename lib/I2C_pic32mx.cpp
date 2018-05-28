/* 
 * File:   I2C_pic32mx.cpp
 * Author: Rafa
 * 
 * Created on May 21, 2018, 8:13 PM
 */

#include <proc/p32mx150f128b.h>
#include "I2C_pic32mx.h"
#include "UART_pic32mx.h"

void I2C_pic32mx::Init(uint32_t speed)
{
	ANSELBCLR = 0x00000300;
	LATBCLR = 0x00000300;
	TRISBCLR = 0x00000300;
	
	I2C1BRG = (((1.f/(2.f*(float)speed)) - 0.000000104f ) * (float)GetPeripheralClock()) - 2.f;
	
	I2C1CONbits.DISSLW = !(speed == F_SCL_FAST);
	I2C1CONbits.ON = 1;
}

uint8_t I2C_pic32mx::Transmit(uint8_t address, uint8_t* src, uint16_t count, bool stop)
{
	
	if(I2C1STATbits.P)	/* if last state was a stop bit*/
	{
		I2C1ACONbits.SEN = 1;	/* Start condition */
		while(I2C1ACONbits.SEN);
	}
	else
	{
		I2C1ACONbits.RSEN = 1;	/* else restart condition */
		while(I2C1ACONbits.RSEN);
	}
	if(I2C1STATbits.BCL == 1)
		return 0;
	
	I2C1TRN = address;
	while(I2C1STATbits.TRSTAT);	/* Master transmit is in progress (8 bits + ACK) */
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
				if(I2C1STATbits.ACKSTAT)		/* 1 = Acknowledge was not received */
				{
					I2C1CONbits.PEN = 1;
					while(I2C1CONbits.PEN);
					return 0;
				}
			}
			else // Read
			{
				I2C1CONbits.ACKDT =  !(count > 1);
				
				I2C1CONbits.RCEN = 1;
				while(I2C1CONbits.RCEN);

				I2C1CONbits.ACKEN = 1;
				while(I2C1CONbits.ACKEN);
				*(src++) = I2C1RCV;
			}
		} 
		while(--count);
	} 

	I2C1CONbits.PEN = stop;
	while(I2C1CONbits.PEN);
	
	return 1;
}