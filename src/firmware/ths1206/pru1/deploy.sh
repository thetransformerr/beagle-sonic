#! /bin/bash

set -e
echo "4a338000.pru1" > /sys/bus/platform/drivers/pru-rproc/unbind 2> /dev/null
make clean
make
cp gen/*.out /lib/firmware/am335x-pru1-fw
echo "4a338000.pru1" > /sys/bus/platform/drivers/pru-rproc/bind
