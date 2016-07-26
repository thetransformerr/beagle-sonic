/*
 * store_readings.c
 *
 * Description:
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 */


#include "store_readings.h"

#include <stdint.h>
#include <stdlib.h>

static uint16_t buffer[ SR_MAX_BUFFER_SIZE ];
static size_t head = 0;


uint32_t SR_store( uint16_t reading )
{
   // TODO: Added in shared RAM too? Currently using just 7KB of PRU1 RAM
   // Limited size is a problem. Even with all of PRU1 RAM and Shared PRU RAM only ~10k readings
   // Max coverage of ~1.6 ms, rough estimate of average tof is ~1.5ms
   // Current coverage is just under 0.6ms with 3500 readings
   // Will need to delay recording

   if( head < SR_MAX_BUFFER_SIZE )
   {
      buffer[head++] = reading;
      return 0;
   }
   return 1;   // Buffer full
}

void SR_reset()
{
   head = 0 ;
}

uint16_t* SR_get_buffer()
{
   return buffer;
}


#undef HOST_INT
#undef TO_ARM_HOST
#undef CHAN_NAME
#undef CHAN_DESC
#undef CHAN_PORT
#undef VIRTIO_CONFIG_S_DRIVER_OK
#undef MAX_ELEMENTS_TO_TRANSFER
