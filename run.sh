#!/bin/bash

trap 'kill $(jobs -pr)' SIGINT SIGTERM EXIT

make

# Ensure uio_pruss module is up and pins configured
./src/firmware/prudaq/setup.sh

# Start up the prudaq capture
# TODO: At least take channels as input
# TODO: Get somebody else to make this work
./src/firmware/prudaq/prudaq_capture -f 6000000-i 0 -q 4 -o >(./src/firmware/prudaq/tof_analysis) pru0.bin pru1.bin &


# Start up sensor fusion script
./src/software/sensor-fusion/fusion.py