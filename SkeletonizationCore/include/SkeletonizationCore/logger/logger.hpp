/**
*
* @file logger.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Declares application logging setup utilities.
*
* This header defines a small logger wrapper responsible for initializing glog
* output files and preparing timestamped log destinations.
*
* Main responsibilities:
* - initialize glog for application logging
* - create and manage log output directory naming
* - build timestamp-based file prefixes for log streams
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <string>
#include <string_view>

/**
 * @class logger
 * @brief Configures and initializes application logging.
 *
 * This class encapsulates glog initialization and log destination setup using
 * a timestamped file naming strategy.
 */
class logger
{
public:
	/**
	 * @brief Constructs a new logger object.
	 *
	 * @param program_name Program name passed to glog initialization.
	 */
	explicit logger(std::string_view program_name);

	/**
	 * @brief Destroys the logger and shuts down glog.
	 */
	~logger();

	/**
	 * @brief Initializes logging directories and destinations.
	 */
	void initialize();

private:
	/**
	 * @brief Initializes glog destinations with a prefix.
	 *
	 * @param log_prefix Prefix for generated log file names.
	 */
	void initialize_glog(const std::string& log_prefix) const;
	/**
	 * @brief Creates the logs directory when it does not exist.
	 */
	static void create_logs_directory();
	/**
	 * @brief Creates a timestamp used for log file naming.
	 */
	void create_log_name();

	/// Timestamp used in log file names.
	std::string timestamp_;
	/// Program name provided to glog initialization.
	std::string_view program_name_;
};
