#include <glog/logging.h>
#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

#include "SkeletonizationCLI/logger/logger.hpp"

logger::logger(const std::string_view program_name)
	: program_name_(program_name)
{
}

logger::~logger()
{
	google::ShutdownGoogleLogging();
}

void logger::initialize()
{
	create_logs_directory();

	create_log_name();

	const auto log_file_prefix = std::string("logs/") + timestamp_ + "_";

	initialize_glog(log_file_prefix);
}

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

void logger::create_logs_directory()
{
	constexpr auto logs_folder_name = "logs";

	std::filesystem::create_directories(logs_folder_name);
}

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
