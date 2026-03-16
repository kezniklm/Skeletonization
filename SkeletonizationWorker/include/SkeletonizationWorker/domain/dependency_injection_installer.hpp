/**
*
* @file dependency_injection_installer.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares domain-layer dependency injection installer.
 *
 * This file defines dependency injection installer for domain components.
 *
 * Main responsibilities:
 * - register domain model bindings
 * - expose domain installer for composition
 * - support layered dependency wiring
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <boost/di.hpp>

namespace worker::domain::dependency_injection
{
	namespace di = boost::di;

	inline auto make_installer()
	{
		return di::make_injector();
	}
}
