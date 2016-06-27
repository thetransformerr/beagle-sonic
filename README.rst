=======================
Beagle Sonic Anemometer
=======================

About
-----
The Beagle Sonic Anemometer is a Beaglebone project for the 2016 Google Summer
of Code. The aim of this project is to create an open source anemometer using
ultrasonic time of flight measurements.


Repo Structure
--------------
src/
   firmware/ths1206/
      pru0 & pru1
         Contains the firmware for both PRUs.

   include/ & lib/
      RPMsg headers and library.
      Taken directly from the pru-support-package v5.
      Updated versions are available at
      https://git.ti.com/pru-software-support-package.
      Note: v5 uses interupt-based RPMsg, which requires a kernel >= 4.4.12-r31