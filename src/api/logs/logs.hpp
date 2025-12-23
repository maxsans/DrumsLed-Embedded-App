/**
 * @file logs.hpp
 * @brief Low-level logging API for different platforms
 * @author Cyprien Ménard
 * @date 2025-01-12
 *
 * @note Don't use directly this file. Please use logStream instead.
 * @see logStream.hpp
 */
#ifndef __LOGS_HPP__
#define __LOGS_HPP__

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
