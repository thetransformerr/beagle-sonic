/***************************************************************************************
 * MAIN.C
 *
 * Description: Driver for PRU1 - THS1206
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 **************************************************************************************/

#include <stdint.h>
//#include "resource_table_1.h"  // Conflicts at link with #include in store_readings.c

#include "comms.h"
#include "ths1206_control.h"
#include "store_readings.h"
#include "common_constants.h"

int main(void)
{
   CO_wait_init();
   CO_wait_config();

   TC_init( TC_ACCEPT_DEFAULTS );

   while(1)
   {
      TC_store_next_n_reads( CC_READS_PER_ROUND );
      CO_transfer_reads( SR_get_buffer() );
      SR_reset();
   }

   __halt();

   /* Should never return */
   return 0;
}
