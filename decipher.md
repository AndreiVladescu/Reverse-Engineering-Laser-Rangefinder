###  Analyzing I2C Packets

After correctly stating that I2C data flows from the microcontroller, we must now understand *how* it works. The microcontroller would be an I2C master, and the screen (because it's connected directly to the I2C line) is the slave. The keyboard is not an I2C device, as through many tries, there is no specific data to/from this device.

The screen's address is 0x3F in hexadecimal or 63 in decimal, and the microcontroller's address is 0x18 hex and 24 decimal. While capturing some data, after measuring once, there are  lots of packets going through the data line.
 ![I2C data packets](https://raw.githubusercontent.com/AndreiVladescu/Reverse-Engineering-Laser-Rangefinder/main/images/i2c_capture_preliminary.png)
 This is the measurement after pressing the button. Every little packet has lots of data, so you will get  really disoriented if you don't delimit it well. What i gathered from examining the structure of the packets :
 - a 2-byte word, 0x80B0 is sent before every packet
 - lots of 0xC00 are being sent, maybe as a padding
 - only some are different from 0xC000
 
 I think that the data being sent is as words, since data is being repeated in a 2-byte fashion.


###  Known-input sniffing
 Next step is to see which bytes are the measurement, so let's try measuring 1.111 meters.
 After some attempts, i got it to measure 1.111 meters. Let's now measure a different value altogether and make the difference between them.
