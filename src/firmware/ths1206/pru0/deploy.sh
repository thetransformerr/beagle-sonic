#! /bin/bash

set -e
make clean
make
cp gen/*.out /lib/firmware/am335x-pru0-fw
echo "4a334000.pru0" > /sys/bus/platform/drivers/pru-rproc/unbind 2> /dev/null
echo "4a334000.pru0" > /sys/bus/platform/drivers/pru-rproc/bind
