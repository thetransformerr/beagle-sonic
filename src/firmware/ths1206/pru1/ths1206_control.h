/*
 * ths1206_control.h
 *
 * Description:
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 */

#ifndef THS1206_CONTROL_H_
#define THS1206_CONTROL_H_

#include <stdint.h>

#include "pin_assignments.h"

#define TC_TRIGGER_LEVEL 1

#define TC_CSR0_BIT     PA_D10_BIT

#define TC_VREF_BIT     PA_D0_BIT
#define TC_MODE_BIT     PA_D1_BIT
#define TC_PD_BIT       PA_D2_BIT
#define TC_CHSEL0_BIT   PA_D3_BIT
#define TC_CHSEL1_BIT   PA_D4_BIT
#define TC_DIFF0_BIT    PA_D5_BIT
#define TC_DIFF1_BIT    PA_D6_BIT
#define TC_SCAN_BIT     PA_D7_BIT
#define TC_TEST0_BIT    PA_D8_BIT
#define TC_TEST1_BIT    PA_D9_BIT


#define TC_CSR1_BIT     PA_D11_BIT

#define TC_RESET_BIT    PA_D0_BIT
#define TC_OVFL_BIT     PA_D1_BIT
#define TC_FRST_BIT     PA_D1_BIT
#define TC_TRIG0_BIT    PA_D2_BIT
#define TC_TRIG1_BIT    PA_D3_BIT
#define TC_DATA_T_BIT   PA_D4_BIT
#define TC_DATA_P_BIT   PA_D5_BIT
#define TC_RW_BIT       PA_D6_BIT
#define TC_BIN2S_BIT    PA_D7_BIT
#define TC_OFFSET_BIT   PA_D8_BIT
#define TC_RBACK_BIT    PA_D9_BIT


#define TC_VREF_INTERNAL   0
#define TC_VREF_EXTERNAL   1

#define TC_MODE_CONTIG     0
#define TC_MODE_SINGLE     1

#define TC_PD_ACTIVE       0
#define TC_PD_DOWN         1

#define TC_SCAN_OFF        0
#define TC_SCAN_ON         1

#define TC_RESET_DO        1

#define TC_OVFL_NONE       0
#define TC_OVFL_OCCURED    1

#define TC_DATA_T_STATIC   0
#define TC_DATA_T_PULSE    1

#define TC_DATA_P_ALOW     0
#define TC_DATA_P_AHIGH    1

#define TC_RW_SEPARATE     0
#define TC_RW_TOGETHER     1

#define TC_OFFSET_NONE     0
#define TC_OFFSET_SET      1

#define TC_RBACK_NORMAL    0
#define TC_RBACK_DEBUG     1


// TODO: Figure out what options should be exposed
void TC_init();
void TC_write( uint32_t value );
uint16_t TC_read();
void TC_store_next_n_reads( uint32_t n );

#endif /* THS1206_CONTROL_H_ */
