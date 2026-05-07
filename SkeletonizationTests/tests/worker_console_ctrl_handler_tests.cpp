/**
 *
 * @file worker_console_ctrl_handler_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests console control handler utilities.
 *
 * This file contains unit tests for console control handler installation.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include "SkeletonizationWorker/infrastructure/platform/console_ctrl_handler.hpp"

TEST(WorkerConsoleCtrlHandler, InstallsHandler)
{
    worker::configuration::dependency_injection::cancellation_token_t token;

    EXPECT_NO_THROW({
        worker::infrastructure::platform::install_console_ctrl_handler(token);
    });
}
