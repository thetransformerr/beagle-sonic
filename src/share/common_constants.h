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

// Delay from start of sonic tx until ADC capture starts
// For now just use a zero-point around 343 m/s, assuming a distance of 0.5m
//    0.5 m / 343 m/s / 5ns = 292000 cycles for total travel
// TODO: Check to ensure it is calculated at compile time
// TODO: Remove reliance on distance assumption
// TODO: Determine if there is a better way to handle it
   // Could try to do it dynamically, but that will require communication between ARM and PRU0
   // It probably is more trouble than it's worth
#define CC_ADC_CAPTURE_DELAY_CYCLES (292000 - (CC_READS_PER_ROUND / 2) )
#define CC_ADC_CAPTURE_DELAY_NS     (CC_ADC_CAPTURE_DELAY_CYCLES * 5)

// Sample rate - 6MSPS
// (1/6,000,000) s / 5 ns / 2 = 16.67
#define CC_ADC_CLOCK_HALF_PERIOD_CYCLES   17
#define CC_ADC_BETWEEN_SAMPLES_NS         (CC_ADC_CLOCK_HALF_PERIOD_CYCLES * 2 * 5)

#define CC_REQ_CONFIG      1
#define CC_FIN_CONFIG      2

#define CC_REQ_INPUT_READY 3
#define CC_FIN_INPUT_READY 4

#define CC_REQ_CONSUMER    5
#define CC_FIN_CONSUMER    6


#endif //COMMON_CONSTANTS_H_
