#!/bin/bash

# TODO: Should probably stop the PRUs too
trap 'kill $(jobs -pr)' SIGINT SIGTERM EXIT

make

# Shutdown both prus if running
echo "4a334000.pru0" > /sys/bus/platform/drivers/pru-rproc/unbind 2> /dev/null
echo "4a338000.pru1" > /sys/bus/platform/drivers/pru-rproc/unbind 2> /dev/null

# Load the firmwares
cp src/firmware/ths1206/pru0/gen/*.out /lib/firmware/am335x-pru0-fw &&
cp src/firmware/ths1206/pru1/gen/*.out /lib/firmware/am335x-pru1-fw ||
   exit 1

# Start up both prus
echo "4a334000.pru0" > /sys/bus/platform/drivers/pru-rproc/bind
echo "4a338000.pru1" > /sys/bus/platform/drivers/pru-rproc/bind

# Wait for rpmsg driver to set up
# TODO: maybe add a check that /dev/rpmsg_pru31 exists?
sleep 0.5

# Start up userspace adc driver
./src/software/adc-controller/adc-controller.out >/dev/null &

# Start up sensor fusion script
./src/software/sensor-fusion/fusion.py