====================
THS1206 ADC Firmware
====================
The THS1206 uses 4 (or 5, depending on configuration) digital control inputs, 1
signal output, and 12 I/O lines that both configure the device and deliver
conversion results.

Unfortunately, this puts it beyond the capability of a single PRU.

Both PRUs will be used to control the THS1206.

PRU0:

- Provide the clock
- Detect results ready
- Prepare THS1206 for read signal
- Signal PRU1 that THS1206 is ready to read
- Prepare THS1206 for write signal
- Signal PRU1 that THS1206 is ready to write

PRU1:

- Signal THS1206 read
- Read THS1206
- Pass results on
- Signal THS1206 write
- Write THS1206 configuration
- Signal PRU0 that THS1206 is configured

There will need to be significant synchronization between the PRUs, however the
synchronization required should be relatively basic.