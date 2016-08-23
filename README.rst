=======================
Beagle Sonic Anemometer
=======================

About
-----
The Beagle Sonic Anemometer is a Beaglebone project for the 2016 Google Summer
of Code. The aim of this project is to create an open source anemometer using
ultrasonic time of flight measurements.

The main branch now focuses on using the prudaq_ cape with custom firmware. As a
cape it is much better suited to the BeagleBone than using a more DIY solution
like the previous THS1206. However prudaq was announced fairly late in GSoC
2016, so the THS1206 was the original focus. That code is still available under
the ths1206 branch. However at this point it is still untested. 

Hardware
--------
- Beaglebone (intended for Green/Green Wireless, but should be little to no
  trouble for Black)
- PRUDAQ Cape
- DHT22 or other DHT sensor compatible with Adafruit_Python_DHT_.
- BMP180 or other BMP sensor compatible with Adafruit_Python_BMP_.
- 20khz Sonic Transducers

Setup
-----
- Attach PRUDAQ cape
- Attach receiver to a channel 0 hookup (0 - 3) on the PRUDAQ
- Attach the transmitter to P8.42

Requirements
------------
- uio_pruss
- HDMI disabled

Build
-----
::

   # make

Run
---
Ideally run.sh would be used but it doesn't actually work. Suggestions or a pull
request would be great. It displays some extremely odd behavior that I can't
begin to explain.

For now, use two shells. You may alter the -i parameter to match your setup.
Give it the hookup number you used for the receiver on the PRUDAQ.

Shell 1::

   # cd /src/firmware/prudaq
   # ./setup.sh
   # ./prudaq_capture -f 6000000 -i 0 -q 4 | ./tof_analysis

Shell 2::

   # ./src/software/sensor-fusion/fusion.py

Dependencies
------------
- Adafruit_Python_DHT_
- Adafruit_Python_BMP_
- ZeroMQ_
- PyZMQ_


.. _prudaq: https://github.com/google/prudaq/wiki
.. _Adafruit_Python_DHT: https://github.com/adafruit/Adafruit_Python_DHT
.. _Adafruit_Python_BMP: https://github.com/adafruit/Adafruit_Python_BMP
.. _ZeroMQ: http://zeromq.org/distro:debian
.. _PyZMQ: https://github.com/zeromq/pyzmq
