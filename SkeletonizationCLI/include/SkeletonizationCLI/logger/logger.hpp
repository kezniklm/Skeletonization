#pragma once

#include <string>
#include <string_view>

class logger
{
public:
    explicit logger(std::string_view program_name);
    ~logger();

    void initialize();

private:
    void initialize_glog(const std::string& log_prefix) const;
    static void create_logs_directory();
    void create_log_name();

    std::string timestamp_;
    std::string_view program_name_;
};
