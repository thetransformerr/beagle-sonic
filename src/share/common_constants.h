/*
 * common_constants.h
 *
 * Description: A listing of constants shared between firmware and software.
 *
 * (C) 2016 Visaoni
 * Licensed under the MIT License.
 */

#ifndef COMMON_CONSTANTS_H_
#define COMMON_CONSTANTS_H_

// The number of ADC reads per sonic tx
#define CC_READS_PER_ROUND 2000

#define CC_REQ_CONFIG      1
#define CC_FIN_CONFIG      2

#define CC_REQ_INPUT_READY 3
#define CC_FIN_INPUT_READY 4

#define CC_REQ_CONSUMER    5
#define CC_FIN_CONSUMER    6


#endif //COMMON_CONSTANTS_H_
