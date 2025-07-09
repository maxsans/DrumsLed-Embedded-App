/**
 * @file adc.h
 * @author Cyprien Ménard
 * @brief ADC API
 * @version 0.1
 * @date 2025-01-14
 *
 * @copyright Copyright (c) 2025
 *
 * @todo Manage multiple ADC channels
 */
#ifndef __ADC_H__
#define __ADC_H__

#include <stdint.h>

/**
 * @brief Type for ADC measures
 */
typedef uint8_t adc_measure_t;

/**
 * @brief Initialize the ADC
 */
void adc_init();

/**
 * @brief Read the ADC
 *
 * @return adc_measure_t The ADC measure
 */
adc_measure_t adc_read();

#endif
