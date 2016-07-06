/*
 * ths1206_control.c
 *
 * Description:
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 */

// TODO: ensure timing works out with abstractions
// May need to macro and/or switch to precalculated masks

#include "ths1206_control.h"
#include "pin_control.h"
#include "store_readings.h"

void TC_init()
{
   // TODO: better to ensure D0-11 are output or assume it starts valid?
   // TODO: Make this not reliant on assumed pinout
   TC_write( 0x401 );  // Sets reset in CR1
   TC_write( 0x400 );  // Clear reset in CR1

   // TODO: Config CR0
   // TODO: Config CR1
   // TODO: set D0-11 to input
}

void TC_write( uint32_t value )
{
   set_pin( PA_WR_BIT );   // Needed? shouldn't hurt
   set_pin( PA_RD_BIT );
   clear_pin( PA_WR_BIT );

   write_reg( value );
   __delay_cycles(2);

   set_pin( PA_WR_BIT );
   __delay_cycles(1);   // Probably extraneous, needs 2 ns
}

uint16_t TC_read()
{
   uint16_t result;

   set_pin( PA_WR_BIT );
   clear_pin( PA_RD_BIT );
   __delay_cycles(2);   // 10ns delay until data ready

   result = (uint16_t)read_reg();
   set_pin( PA_RD_BIT );
   __delay_cycles(1);   // Probably extraneous, needs 5 ns delay to CS invalid, but CS tied active
   return result;
}

void TC_store_next_n_reads( uint32_t n )
{
   // TODO: Handle n % TC_TRIGGER_LEVEL != 0 case better
   uint32_t i;
   for( i = 0; i < n; i += TC_TRIGGER_LEVEL )
   {
      // Note: DATA_AV defaults to an active high pulse with width half CONV_CLK input
      // Busy wait here should catch it
      while( !read_pin( PA_DATA_AV_BIT ) );

      int j;
      for( j = 0; j < TC_TRIGGER_LEVEL; j++ )
      {
         SR_store( TC_read() );
      }
      // Shouldn't need a delay here - typical time to DATA_AV inactive is 12ns
         // No range given though
   }
}
