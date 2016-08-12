/*
 * comms.h
 *
 * Description: Communications package between userspace driver and pru.
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 */

#ifndef COMMS_H_
#define COMMS_H_

void CO_wait_init();
void CO_wait_config();
void CO_wait_input_ready();

void CO_transfer_reads( void* buffer );

#endif // COMMS_H_
