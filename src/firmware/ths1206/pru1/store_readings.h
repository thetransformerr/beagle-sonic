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

#define SR_MAX_BUFFER_SIZE 1750

uint32_t SR_store( uint32_t value );
void SR_transfer_readings();

#endif /* STORE_READINGS_H_ */
