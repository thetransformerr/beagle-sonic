/*
 * store_readings.c
 *
 * Description:
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 */


#include "store_readings.h"

void SR_store( uint32_t reading )
{
   // TODO: fill in
   // Probably have to store to RAM, needs to be relatively quick
   // Limited size is a problem. Even with all of PRU1 RAM and Shared PRU RAM only ~5k readings
   // Max coverage of ~0.8 ms, rough estimate of average tof is ~1.5ms
   // Will need to delay recording
   return;
}

void SR_transfer_readings()
{
   // TODO: fill in
   // Transfers all readings to userspace
   return;
}
