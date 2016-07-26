/*
 * store_readings.h
 *
 * Description:
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 */

#ifndef STORE_READINGS_H_
#define STORE_READINGS_H_

#include <stdint.h>
#include "common_constants.h"

#define SR_MAX_BUFFER_SIZE CC_READS_PER_ROUND

uint32_t SR_store( uint16_t value );
void SR_reset();
uint16_t* SR_get_buffer();

#endif /* STORE_READINGS_H_ */
