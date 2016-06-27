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
* Waits for PRU1 ready
* Triggers ultrasonic pulses
* Controls THS1206 clock

PRU1
----
* Configures THS1206 and resets FIFO
* Signals PRU0 ready
* Receives data available signal from THS1206
* Reads data from THS1206
* Transfers data to ARM using RPMsg

Interactions
------------
There are two axes to measure, and each measurement will involve a constant
number of ADC reads. This allows the PRUs to work together with a minimum of
synchronization.

PRU0 triggers the pulse when PRU1 is ready, and then begins the clock. As data
fills the queue on the THS1206, it signals PRU1 to read. After all
readings are collected, PRU0 stops the clock and PRU1 transfers the readings to
the ARM core. PRU1 then reconfigures the THS1206 to select the other axis to
read and clears the FIFO before signaling PRU0. After that is done, PRU0 is free
to trigger the pulse in the other axis and again begin the clock. The static
number of reads required allows the PRUs to collectively work with the ADC
with a single one-way synchronization.