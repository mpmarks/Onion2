#include "Onion2.h"

// constructor
Onion2::Onion2() 
{
	// reset class variables
	Reset();

	// Arduino Dock setup
	ArduinoDockSetup();
}


// reset function
void Onion2::Reset()
{
//  nothing yet
}


//// private functions
// read byte from I2C interface
//	 return 0 if byte is valid, 1 if byte is invalid
int Onion2::_ReadTwiByte (uint8_t &rdByte) 
{
	int status	= EXIT_FAILURE;

	if (Wire.available() > 0) {
		rdByte	= Wire.read();
		status	= EXIT_SUCCESS;
	}

	return status;
}

int Onion2::_ReadTwiByte (int &rdByte) 
{
	int status	= EXIT_FAILURE;
	uint8_t		byte;

	// use the uint8_t version
	status 	= _ReadTwiByte(byte);
	rdByte 	= (int)byte;

	return status;
}


//// public functions
// enable I2C interface and setup ISR
void Onion2::ArduinoDockSetup(uint8_t i2c_addr) {
	
	// disable watchdog timer
	wdt_disable();

	// join i2c bus 
	Wire.begin(i2c_addr);
	
	// register service routines
	Wire.onReceive(&ReceiveEvent);
	Wire.onRequest(&RequestEvent);
}

// Restarts program from beginning but does not reset the peripherals and registers
void Onion2::SoftwareReset() {
	wdt_enable(WDTO_15MS);
} 

// perform a specific action based on the i2c address received
int Onion2::ReceiveEventHandler(uint8_t addr)
{
	int 	status;
	int 	i, red, green, blue;
	uint8_t data;

	switch(addr) {
		case (ARDUINO_DOCK_ADDR_READBACK):
			// set the last command to: the address contained in the data
			status == _ReadTwiByte(data);
			if (status == EXIT_SUCCESS) {
				addr 	= data;
			}

			break;

		case (ARDUINO_DOCK_ADDR_RESET):
			status == _ReadTwiByte(data);
			if (status == EXIT_SUCCESS && data == ARDUINO_DOCK_CMD_RESET) {
				SoftwareReset();
			}

			break;


		default:
			FP fp = _userRcvHandler[addr];
			if (fp) (fp)(); // call user routine
			break;
	}

	// note the last address
	lastCmdAddr 	= addr;

	return status;
}



// perform a specific action based on the i2c address received
int Onion2::RequestEventHandler(uint8_t &data)
{
	int 	status = EXIT_SUCCESS;

	// respond based on the last addr that was written to
	switch(lastCmdAddr) {
		case (ARDUINO_DOCK_ADDR_READBACK):
			data 	= (uint8_t)lastCmdData;
			break;

		case (ARDUINO_DOCK_ADDR_RESET):
			data 	= 0xBE;
			break;
	
		default:
//			status 	= EXIT_FAILURE;
//			data 	= 0xff;
			RFP fp = _userReqHandler[lastCmdAddr];
			if (fp) data = (fp)();
			else {
				data = 0xff;
				status = EXIT_FAILURE;
			}
			break;
	}

	return status;
}

// global instance
Onion2 ONION;

// At global level
void Onion2::ReceiveEvent(int howmany) {
		int 	status;
		uint8_t  addr  = 0;

	// receive bytes from i2c bus
	while (Wire.available() > 0) {
		addr 	= Wire.read();

		// handle received commands and data
		status  = ONION.ReceiveEventHandler(addr);
	}

}

void Onion2::RequestEvent() {
	int      status;
	uint8_t	 data	= 0;

	// respond on the i2c bus
	status 	= ONION.RequestEventHandler(data);

	Wire.write(data);

}


bool Onion2::registerRcvHandler(byte adr, FP funToCall) {

	if (_userRcvHandler[adr] != 0 || adr == ARDUINO_DOCK_ADDR_READBACK || adr > MAX_USER_ADDR) return false; 
	else _userRcvHandler[adr] = funToCall;
	
//	Serial.print("Registered rcv "); Serial.println(adr);  // debug

	return true;
}


bool Onion2::registerReqHandler(byte adr, RFP funToCall) {

	if (_userReqHandler[adr] != 0 || adr == ARDUINO_DOCK_ADDR_READBACK || adr > MAX_USER_ADDR) return false;
	else _userReqHandler[adr] = funToCall;
	
//	Serial.print("Registered req "); Serial.println(adr); // debug

	return true;
}
