#pragma once

/**
 * @file exceptions.hpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Convenience header that includes all CLI exception types.
 *
 * This file provides aggregate includes for all CLI exception categories.
 *
 * Main responsibilities:
 * - include base CLI exception definitions
 * - include benchmark/config/export exception hierarchies
 * - provide single include entry for exceptions
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "cli_exception.hpp"
#include "configuration_exception.hpp"
#include "benchmark_exception.hpp"
#include "export_exception.hpp"
#include "visualization_exception.hpp"
