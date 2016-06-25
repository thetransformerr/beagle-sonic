/***************************************************************************************
 * MAIN.C
 *
 * Description: Driver for PRU1 - THS1206
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 **************************************************************************************/

#include <stdint.h>
#include "resource_table_empty.h"

#include "ths1206_control.h"
#include "store_readings.h"

#define READS_PER_ROUND 2000

int main(void)
{
   while(1)
   {
      TC_store_next_n_reads( READS_PER_ROUND );
      SR_transfer_readings();
   }

   __halt();

   /* Should never return */
   return 0;
}
