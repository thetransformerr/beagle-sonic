/***************************************************************************************
 * MAIN.C
 *
 * Description: Driver for PRU0 - THS1206
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 **************************************************************************************/

#include <stdint.h>
#include "resource_table_empty.h"

#include <pru_intc.h>

#include "pin_assignments.h"
#include "pin_control.h"
#include "common_constants.h"

// 6MSPS
// (1/6,000,000) s / 5 ns / 2 = 16.67
#define DELAY_CYCLES 17


int main(void)
{
   uint32_t read_count;
   while( 1 )
   {
      while( read_pin(30) == 0 );   // Wait for interrupt from PRU1     TODO: Make comms available and move it there?
      CT_INTC.SICR = 18;            // Clear interrupt status

      for( read_count = 0; read_count < CC_READS_PER_ROUND; read_count++ )
      {
         // TODO: The delays are a little off. Should account for time to set and loop...
         //       unless the optimizer gets to it? Maybe find a way to ensure it's unrolled
         set_pin( PA_CONV_CLK_BIT );
         __delay_cycles( DELAY_CYCLES );
         clear_pin( PA_CONV_CLK_BIT );
         __delay_cycles( DELAY_CYCLES );
      }
   }

   __halt();

   /* Should never return */
   return 0;
}
