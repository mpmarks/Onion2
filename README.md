# Onion - Aruino Dock Interface example #
This little Arduino library is a modified version of the official Onion library. It allows the user program to add a number of user handlers for I2C receive or requests. They will be added to the I2C interface (at I2C address 0x08) so that the Arduino program can get data from the Onion or supply data. The existing mechanism for loading Arduino firmware through the OTA interface is unchanged (i.e. it will reboot on a specific I2C command string 0xdead). 

An example program is provided that shows how to use the library. It returns analog data from A0, and also receives data from the Onion which it uses to control a neopixel. 

Registering a user handler for receive or request adds it to a set of handlers that will be called depending on the "register address" used in the I2C transaction. For instance if you add a receive handler to address 1:
ONION.registerRcvHandler(1, &recvEvent);
Your event routine will be called on the I2C receive interrupt for chip address 0x08 register address 1. It can then read additional data bytes from I2C.  Similar thing if you register a request handler. Your code will get called when that address is used in an I2C read. Whatever value you return will get passed back to the Omega.

For example if you register a receive handler at 0x01 and execute (in the command window):
i2cset -y 0 0x08 0x01 0x55 b
This will call your 0x01 routine. In that routine your can read the next I2C data byte (Wire.read()) and it will be 0x55. Of course you can also use a python of C++ program on the Omega to interface at higher speeds.

**Notes and Limitations**

1. Don't include the Onion library, just use this one. The Arduino firmware download should work as before.
2.  This library creates an ONION object, which is initialized on load to enable the firmware download functionality. There is no need for a "new" call or a specific name for the instance.
3.  You can add up to 15 or so user handlers. 0 is already used by Onion. So you should be able to use addresses 1-15 for your handlers. I kept it small to save memory.
4.  Because your routines will be called under interrupt don't do anything slow, or use Serial, etc. That will probably crash the Arduino.
5.  There is very little error handling in the code. Its just an experiment to make the Onion Arduino Dock a bit more useful.
