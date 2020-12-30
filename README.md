# Teletype-BRPE
Arduino code to drive a teletype BRPE paper tape punch via USB serial

Currently runs at 1/6th the designed speed.

The teletype BRPE uses a 28V 1A high speed parallel interface. There is a "synchro-pulse" magnet that is used to provide timing to the control electronics.

The code looks for an A, then a second A, then a 1 byte file size, and then the data.

Once all data is received, the system checks to ensure that the timing is making sense, and then it will ounch 4 bytes, then the next 4 bytes, and then punch the index hole and advance the tape.

Each step takes 2 cycles with this code, higher speeds are possible, up to 110 operations/bytes per second.

Video of this code in action is available at https://youtu.be/-aZl8rMT8Aw
