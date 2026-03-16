/**
*
* @file console_ctrl_handler.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements console control handling for graceful worker shutdown.
*
* This file implements platform control signal handling.
*
* Main responsibilities:
* - register console control callbacks
* - propagate stop signals to cancellation token
* - support graceful worker termination
*
* @version 1.0
* @date 2026-03-16
*/

#include "SkeletonizationWorker/infrastructure/platform/console_ctrl_handler.hpp"

#ifdef _WIN32
#include <Windows.h>
#else
#include <csignal>
#endif

namespace worker::infrastructure::platform
{
	namespace
	{
		configuration::dependency_injection::cancellation_token_t* g_token = nullptr;

#ifdef _WIN32
		BOOL WINAPI handler(const DWORD ctrl_type)
		{
			switch (ctrl_type)
			{
			case CTRL_C_EVENT:
			case CTRL_CLOSE_EVENT:
			case CTRL_BREAK_EVENT:
			case CTRL_SHUTDOWN_EVENT:
				{
					if (g_token)
					{
						g_token->request_cancel();
					}
					return TRUE;
				}
			default:
				return FALSE;
			}
		}
#else
		void handler(int)
		{
			if (g_token)
			{
				g_token->request_cancel();
			}
		}
#endif
	}

	void install_console_ctrl_handler(configuration::dependency_injection::cancellation_token_t& token)
	{
		g_token = &token;

#ifdef _WIN32
		SetConsoleCtrlHandler(handler, TRUE);
#else
		std::signal(SIGINT, handler);
		std::signal(SIGTERM, handler);
#endif
	}
}
