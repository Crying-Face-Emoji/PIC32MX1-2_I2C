/* 
 * File:   I2C_pic32mx.h
 * Author: Rafa
 *
 * Created on May 21, 2018, 8:13 PM
 */

#ifndef	I2C_PIC32MX_H
#define	I2C_PIC32MX_H

#include <xc.h>
#include <sys/attribs.h>
#include "../HardwareConfig.h"

#define F_SCL_NORMAL	100000UL
#define F_SCL_FAST		400000UL

#define I2C_RESTART	0
#define I2C_STOP		1

class I2C_pic32mx
{
	public:
	inline I2C_pic32mx() {}
	
	void Init(uint32_t speed);

	uint8_t Transmit(uint8_t address, uint8_t* src, uint16_t count, bool stop);

	inline uint8_t Write(uint8_t address, uint8_t* src, uint16_t count, bool stop)
	{
		if(((address < 0x08) || (address > 0x77)) && ((address != 0x00)))
			return 0;
		return Transmit((address << 1) & ~0x01, src, count, stop);
	}
	
	inline uint8_t WriteByte(uint8_t address, uint8_t data, bool stop)
	{
		return Transmit((address << 1) & ~0x01, &data, 1, stop);
	}

	inline uint8_t Read(uint8_t address, uint8_t* dst, uint16_t count, bool stop)
	{
		if(((address < 0x08) || (address > 0x77)) && ((address != 0x00)))
			return 0;
		
		return Transmit((address << 1) | 0x01, dst, count, stop);
	}
	
	inline uint8_t ReadByte(uint8_t address, bool stop)
	{
		uint8_t data = 0x00;

		Transmit((address << 1) | 0x01, &data, 1, stop);

		return data;
	}
} ;

#endif	/* I2C_PIC32MX_H */

//UART1.Printf("\nprogram init\n");
//		
//	UART1.Printf("performing I2C check\n");	
//	
//	for(uint8_t a = 0x01; a < 0x7F; a++)
//	{
//		if(I2C.Write(a, nullptr, 0, 1))
//		{
//			UART1.Printf("  I2C Address 0x%02X ACKed!\n", a);
//		}
//	}
//	
//	UART1.Printf("I2C check done\n");