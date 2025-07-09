/**
 * @file logs.h
 * @author Cyprien Ménard
 * @brief
 * @version 0.1
 * @date 2025-01-12
 *
 * @copyright Copyright (c) 2025
 *
 * @note Don't use directly this file. Please use logStream instead.
 * @see logStream.h
 */
#ifndef __LOGS_H__
#define __LOGS_H__

/**
 * @brief Macro to enable / disable all the logs
 */
#define ENABLE_LOGS 1

/**
 * @brief Initialize the logs module
 *
 */
void logs_init();

/**
 * @brief Log a message
 *
 * @param format Format string
 * @param ... Arguments
 */
void log(const char *format, ...);

#endif
