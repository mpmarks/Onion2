/*
 

Onion2 - MPM Feb 216
- a  more general onion i2c library
- neopixel support removed

Notes:

1) A global ONION object is created if you require the library. Like the SPI or Serial object.
	It will initialize the i2c and firemware download stuff.
2) In theary you could have as many 256 reader and writer routines, 
 but that would take more ram and there are predefined addresses for the firmware control. I just used
 0-15. And 0 is currently used for the Onion response.

*/

#ifndef _ONION2_H_
#define _ONION2_H_

#include <Arduino.h>
#include <Wire.h>
#include <avr/wdt.h>


#define	REBOOT_STRING                  (0xdead)
#define	ARDUINO_DOCK_I2C_DEVICE_ADDR   (0x08)

#define ARDUINO_DOCK_ADDR_READBACK		(0x00)
#define	ARDUINO_DOCK_ADDR_RESET			(0xde)
#define ARDUINO_DOCK_CMD_RESET	 		(0xad)



#define MAX_USER_ADDR 16

typedef void (*FP)();		// output function
typedef byte (*RFP)();		// input function


// Onion Library Class
class Onion2 {

public:
	Onion2();

	void 	Reset 				(void);

	void 	ArduinoDockSetup	(uint8_t i2c_addr = ARDUINO_DOCK_I2C_DEVICE_ADDR);
	
	int 	ReceiveEventHandler		(uint8_t addr);
	int 	RequestEventHandler		(uint8_t &data);
	void 	SoftwareReset 			(void);
	static void ReceiveEvent(int howmany);
	static void RequestEvent(void);

	bool	registerRcvHandler(byte addr, FP);
	bool	registerReqHandler(byte addr, RFP);
	

private:

	int 	_ReadTwiByte 	(uint8_t &rdByte);
	int 	_ReadTwiByte 	(int &rdByte);

	FP 		_userRcvHandler[MAX_USER_ADDR];
	RFP		_userReqHandler[MAX_USER_ADDR];

	// private members
	int 		lastCmdAddr;
	int 		lastCmdData;

};

extern Onion2 ONION;

#endif // _ONION2_H_