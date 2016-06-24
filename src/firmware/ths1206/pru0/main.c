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

#include "pin_assignments.h"
#include "pin_control.h"

// 6MSPS
// (1/6,000,000) s / 5 ns / 2 = 16.67
#define DELAY_CYCLES 17


int main(void)
{
   // TODO: Figure out if there is a need to wait for PRU1 to initialize

   while( 1 )
   {
      set_pin( PA_CONV_CLK_BIT );
      __delay_cycles( DELAY_CYCLES );
      clear_pin( PA_CONV_CLK_BIT );
      __delay_cycles( DELAY_CYCLES );
   }

   __halt();

   /* Should never return */
   return 0;
}
