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

// Assumes a 20khz sonic
// TODO: Figure out the best place for this
// It's not relevant elsewhere, but here it needlessly separates values that users might change
#define SONIC_DELAY_CYCLES    5000

#define CAPTURE_DELAY_CYCLES  CC_ADC_CAPTURE_DELAY_CYCLES
#define CLOCK_DELAY_CYCLES    CC_ADC_CLOCK_HALF_PERIOD_CYCLES


int main(void)
{
   uint32_t read_count;
   while( 1 )
   {
      while( read_pin(30) == 0 );   // Wait for interrupt from PRU1     TODO: Make comms available and move it there?
      CT_INTC.SICR = 18;            // Clear interrupt status

      // TODO: Assumes the pin starts low, is this correct? Should only impact the very first tx
      set_pin( PA_SONIC_1_BIT );
      __delay_cycles( SONIC_DELAY_CYCLES );
      clear_pin( PA_SONIC_1_BIT );
      __delay_cycles( CC_ADC_CAPTURE_DELAY_CYCLES - SONIC_DELAY_CYCLES );  // TODO: Off by a few cycles

      for( read_count = 0; read_count < CC_READS_PER_ROUND; read_count++ )
      {
         // TODO: The delays are a little off. Should account for time to set and loop...
         //       unless the optimizer gets to it? Maybe find a way to ensure it's unrolled
         set_pin( PA_CONV_CLK_BIT );
         __delay_cycles( CLOCK_DELAY_CYCLES );
         clear_pin( PA_CONV_CLK_BIT );
         __delay_cycles( CLOCK_DELAY_CYCLES );
      }

   }

   __halt();

   /* Should never return */
   return 0;
}
