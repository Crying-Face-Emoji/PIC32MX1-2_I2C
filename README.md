
# UART for PIC32MX1XX/2XX

Author:  
Name: Rafael Silva  
Github: github.com/Crying-Face-Emoji  

## Dependencies
HardwareConfig.h  
stdint.h	  
xc.h		  
sys/attribs.h  	
  
interrupts must be enabled  
multivectored mode must be enabled  
pins must be set to input/output correspondingly  
pps must be configured  
    
## Software  
Compiler: xc32-gcc (v2.05)   
IDE: MPLAB X v4.00  
  
## Hardware  
Any PIC32MX from the 1XX/2XX family  
  
## summary of vars/funcions  
  
macros:  
UART_FIFO_SIZE			// size of the rx buffer on all enabled uarts  
GetPeripheralClock			// corresponds to the frquency(Hz) of the peripheral	s  

included funcions: 
UARTx.Init(Baud, DoubleSpeed)	// initializes module with given baud  
UARTx.BytesAvailable()		// Returns number of bytes available to read in the RX buffer  
UARTx.Flush()				// "clears" the RX buffer, flushes all pending data  
UARTx.ReadByte()			// return 1 byte of data from the bottom of the FIFO (removes from stack)  
UARTx.PeekByte()			// return 1 byte of data from the bottom of the FIFO (doesn't remove from stack)  
UARTx.Read(Dest , Count)		// puts x(Count) bytes in given location, Dest(pointer) (removes from stack)  
UARTx.WriteByte(Data)		// send 1 given byte  
UARTx.Write(Src, Count)		// send x(Count) bytes from src(pointer)  
UARTx.printf(Fmt, ...)			// Print/Send formatted data (behaves like normal printf)  
  
Variables:  
RX_read_index		// read index in buffer  
RX__write_index		// write index in buffer  
RXBuffer[]			// buffer with receive data, array  
  
base_addr			// base memory address of module, used for initialization, constant  
  
## TODO:  
* auto pps configure and port init  
* auto set double speed, by calculating error  
* properly coment the code  

### Sorry for this mess it was copy pasted ^^  
