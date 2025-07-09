/**
 * @file adc.h
 * @author Cyprien M�nard
 * @brief ADC API
 * @version 0.1
 * @date 2025-01-14
 *
 * @copyright Copyright (c) 2025
 *
 * @todo Manage multiple ADC channels
 */
#ifndef __ADC_HPP__
#define __ADC_HPP__

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
