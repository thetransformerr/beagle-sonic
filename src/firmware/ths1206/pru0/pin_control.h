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


volatile register uint32_t __R30;
volatile register uint32_t __R31;

#define set_pin( bit ) ( __R30 |= (uint32_t)1 << bit )
#define clear_pin( bit ) ( __R30 &= (uint32_t)~(1 << bit ))
#define read_pin( bit ) ( __R31 & (uint32_t)(1 << bit) )
#define write_reg( value ) ( __R30 = value )
#define read_reg() ( __R31 )

#endif /* PIN_CONTROL_H_ */
