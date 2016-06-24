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

uint32_t TC_read()
{
   uint32_t result;

   set_pin( PA_WR_BIT );
   clear_pin( PA_RD_BIT );
   __delay_cycles(2);   // 10ns delay until data ready

   result = read_reg();
   set_pin( PA_RD_BIT );
   __delay_cycles(1);   // Probably extraneous, needs 5 ns delay to CS invalid, but CS tied active
   return result;
}
