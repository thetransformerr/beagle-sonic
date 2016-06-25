/*
 * store_readings.c
 *
 * Description:
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 */


#include "store_readings.h"

static uint32_t buffer[ SR_MAX_BUFFER_SIZE ];
static uint32_t head = 0;

uint32_t SR_store( uint32_t reading )
{
   // TODO: Added in shared RAM too? Currently using just 7KB of PRU1 RAM
   // Limited size is a problem. Even with all of PRU1 RAM and Shared PRU RAM only ~5k readings
   // Max coverage of ~0.8 ms, rough estimate of average tof is ~1.5ms
   // Current coverage is just under 0.3ms with 1750 readings
   // Will need to delay recording

   if( head < SR_MAX_BUFFER_SIZE )
   {
      buffer[head++] = reading;
      return 0;
   }
   return 1;   // Buffer full
}

void SR_transfer_readings()
{
   // TODO: fill in
   // Transfers all readings to userspace
   return;
}
