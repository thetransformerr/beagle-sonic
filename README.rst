=======================
Beagle Sonic Anemometer
=======================

About
-----
The Beagle Sonic Anemometer is a Beaglebone project for the 2016 Google Summer
of Code. The aim of this project is to create an open source anemometer using
ultrasonic time of flight measurements.

`Final Report`_

The main branch now focuses on using the prudaq_ cape with custom firmware. As a
cape it is much better suited to the BeagleBone than using a more DIY solution
like the previous THS1206. However prudaq was announced fairly late in GSoC
2016, so the THS1206 was the original focus. That code is still available under
the ths1206 branch. However at this point it is still untested.

Quickstart_
----------

Hardware
--------
- Beaglebone (intended for Green, but should be little to no
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
.. _Quickstart: https://github.com/Visaoni/beagle-sonic-anemometer/wiki/Quickstart
.. _`Final Report`: https://groups.google.com/d/msg/beagleboard-gsoc/CMmqEzv77bQ/g5OLpv1CBQAJ