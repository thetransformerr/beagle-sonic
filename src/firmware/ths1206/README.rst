====================
THS1206 ADC Firmware
====================
The THS1206 is controlled by both PRU0 and PRU1. The PRUs run largely
individually, and don't require synchronization beyond static timing.

Running
-------
Both pru0/ and pru1/ contain a simple deploy.sh script, which takes care of
compilation and installation of the firmware, as well as starting the core.

The PRU_CGT environmental variable needs to point to the TI CGT-PRU compiler
directory.

PRU0
----
* Triggers ultrasonic pulses
* Controls THS1206 clock

PRU1
----
* Configures THS1206
* Receives data available signal from THS1206
* Reads data from THS1206
* Transfers data to ARM using RPMsg

Interactions
------------
There are two axes to measure, and each measurement will involve a constant
number of ADC reads. This allows the PRUs to work together without using
explicit synchronization between them.

PRU0 triggers the pulse, and then begins the clock. As data fills the queue on
the THS1206, it signals PRU1 to begin reading. After all readings are collected,
PRU0 needs to stop the clock, and PRU1 needs to reconfigure the THS1206 to
select the other axis to read, and clear the pipeline. After that is done, PRU0
is free to trigger the pulse in the new axis and again begin the clock. As a
static number of readings will be collected, all of this can be done without
runtime communication.