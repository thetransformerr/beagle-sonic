/*
 * pin_control.h
 *
 * Description:
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 */

// TODO: Get rid of this duplication nonsense with PRU0/1

#ifndef PIN_CONTROL_H_
#define PIN_CONTROL_H_

#include <stdint.h>


void set_pin( uint32_t bit );

void clear_pin( uint32_t bit );

// TODO: Convert to 0/1?
uint32_t read_pin( uint32_t bit );

void write_reg( uint32_t value );

uint32_t read_reg();

#endif /* PIN_CONTROL_H_ */
