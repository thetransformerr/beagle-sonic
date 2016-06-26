#! /bin/bash

make clean
make
cp gen/*.out /lib/firmware/am335x-pru$PRU_CORE-fw
echo "4a338000.pru1" > /sys/bus/platform/drivers/pru-rproc/unbind 2> /dev/null
echo "4a338000.pru1" > /sys/bus/platform/drivers/pru-rproc/bind