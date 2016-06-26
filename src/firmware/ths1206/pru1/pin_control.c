/*
 * pin_control.c
 *
 * Description:
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 */

#include "pin_control.h"

volatile register uint32_t __R30;
volatile register uint32_t __R31;

void set_pin( uint32_t bit )
{
   __R30 |= 1 << bit;
}

void clear_pin( uint32_t bit )
{
   __R30 &= ~(1 << bit);
}

// TODO: Convert to 0/1?
uint32_t read_pin( uint32_t bit )
{
   return __R31 & ~(1 << bit);
}

void write_reg( uint32_t value )
{
   __R30 = value;
}

uint32_t read_reg()
{
   return __R31;
}
