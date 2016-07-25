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

void SR_init();
uint32_t SR_store( uint16_t value );
void SR_transfer_readings();

#endif /* STORE_READINGS_H_ */
