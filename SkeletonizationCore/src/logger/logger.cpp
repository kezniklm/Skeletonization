/**
*
* @file logger.cpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Implements logger initialization and glog destination setup.
*
* This file provides logger lifecycle behavior, including timestamp generation,
* log directory preparation, and glog destination configuration.
*
* Main responsibilities:
* - initialize and shut down glog runtime
* - create timestamped log prefixes
* - configure file destinations for log severities
*
* @version 1.0
* @date 2026-03-16
*/

#include <glog/logging.h>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "SkeletonizationCore/logger/logger.hpp"

/**
 * @brief Constructs a new logger object.
 *
 * @param program_name Program name used by glog.
 */
logger::logger(const std::string_view program_name)
	: program_name_(program_name)
{
}

/**
 * @brief Destroys the logger and shuts down glog.
 */
logger::~logger()
{
	google::ShutdownGoogleLogging();
}

/**
 * @brief Initializes log directory, naming, and glog destinations.
 */
void logger::initialize()
{
	create_logs_directory();

	create_log_name();

	const auto log_file_prefix = std::string("logs/") + timestamp_ + "_";

	initialize_glog(log_file_prefix);
}

/**
 * @brief Initializes glog and configures output destinations.
 *
 * @param log_prefix Prefix prepended to log file names.
 */
void logger::initialize_glog(const std::string& log_prefix) const
{
	google::InitGoogleLogging(program_name_.data());

	FLAGS_logtostderr = false;
	FLAGS_alsologtostderr = true;

	google::SetLogDestination(google::GLOG_INFO, (log_prefix + "INFO_").c_str());
	google::SetLogDestination(google::GLOG_WARNING, (log_prefix + "WARNING_").c_str());
	google::SetLogDestination(google::GLOG_ERROR, (log_prefix + "ERROR_").c_str());
	google::SetLogDestination(google::GLOG_FATAL, (log_prefix + "FATAL_").c_str());
}

/**
 * @brief Creates the logs directory.
 */
void logger::create_logs_directory()
{
	constexpr auto logs_folder_name = "logs";

	std::filesystem::create_directories(logs_folder_name);
}

/**
 * @brief Generates a timestamp string for log file names.
 */
void logger::create_log_name()
{
	const auto now = std::chrono::system_clock::now();
	std::time_t now_time = std::chrono::system_clock::to_time_t(now);
	std::tm tm;

#ifdef _WIN32
	localtime_s(&tm, &now_time);
#else
	localtime_r(&now_time, &tm);
#endif

	std::ostringstream oss;
	oss << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S");
	timestamp_ = oss.str();
}
