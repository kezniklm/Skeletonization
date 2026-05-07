/**
 *
 * @file worker_job_processor_tests.cpp
 * @author Matej Keznikl (matej.keznikl@gmail.com)
 * @brief Tests worker job processor.
 *
 * This file contains unit tests for job processing behavior.
 *
 * @version 1.0
 * @date 2026-03-16
 */

#include "gtest/gtest.h"

#include <filesystem>

#include "SkeletonizationWorker/application/services/job_processor.hpp"

namespace
{
    class fake_image_service final : public worker::application::interfaces::i_image_service
    {
    public:
        std::expected<cv::Mat, std::string> load_image(const std::string& path) override
        {
            last_loaded = path;
            if (should_fail_load)
            {
                return std::unexpected("load error");
            }
            return cv::Mat(2, 2, CV_8UC1, cv::Scalar(255));
        }

        std::expected<void, std::string> save_image(const cv::Mat& image, const std::string& path) override
        {
            last_saved = path;
            if (image.empty())
            {
                return std::unexpected("empty");
            }
            if (should_fail_save)
            {
                return std::unexpected("save error");
            }
            return {};
        }

        bool should_fail_load = false;
        bool should_fail_save = false;
        std::string last_loaded;
        std::string last_saved;
    };

    class fake_object_storage final : public worker::application::interfaces::i_object_storage
    {
    public:
        std::expected<void, std::string> download_to_file(const std::string& key,
                                                          const std::filesystem::path& destination_path) override
        {
            last_download_key = key;
            last_download_path = destination_path.string();
            if (should_fail_download)
            {
                return std::unexpected("download error");
            }
            return {};
        }

        std::expected<void, std::string> upload_from_file(const std::filesystem::path& source_path,
                                                          const std::string& key,
                                                          const worker::application::interfaces::object_put_options&
                                                          options) override
        {
            last_upload_path = source_path.string();
            last_upload_key = key;
            last_upload_content_type = options.content_type.value_or("");
            if (should_fail_upload)
            {
                return std::unexpected("upload error");
            }
            return {};
        }

        std::expected<void, std::string> remove(const std::string& key) override
        {
            last_removed = key;
            return {};
        }

        bool is_remote_backend() const noexcept override
        {
            return remote_backend;
        }

        bool remote_backend = false;
        bool should_fail_download = false;
        bool should_fail_upload = false;
        std::string last_download_key;
        std::string last_download_path;
        std::string last_upload_path;
        std::string last_upload_key;
        std::string last_upload_content_type;
        std::string last_removed;
    };

    class fake_processor final : public worker::application::interfaces::i_skeletonization_processor
    {
    public:
        std::expected<cv::Mat, std::string> process(const cv::Mat& input_image,
                                                    const std::string& algorithm_name,
                                                    bool should_preprocess) override
        {
            last_algorithm = algorithm_name;
            last_should_preprocess = should_preprocess;
            if (should_fail)
            {
                return std::unexpected("process error");
            }
            return input_image.clone();
        }

        bool should_fail = false;
        bool last_should_preprocess = false;
        std::string last_algorithm;
    };

    class fake_result_publisher final : public worker::application::interfaces::i_result_publisher
    {
    public:
        std::expected<void, std::string> publish_task_result(
            const std::string& job_id,
            const std::string& image_path,
            const std::string& algorithm,
            const std::string& output_path,
            bool success,
            double processing_time_ms,
            const std::string& error_message) override
        {
            last_job_id = job_id;
            last_image_path = image_path;
            last_algorithm = algorithm;
            last_output_path = output_path;
            last_success = success;
            last_processing_time_ms = processing_time_ms;
            last_error_message = error_message;
            ++publish_count;
            return {};
        }

        int publish_count = 0;
        bool last_success = false;
        double last_processing_time_ms = 0.0;
        std::string last_job_id;
        std::string last_image_path;
        std::string last_algorithm;
        std::string last_output_path;
        std::string last_error_message;
    };

    worker::configuration::dependency_injection::output_directory_t make_output_directory()
    {
        const auto path = std::filesystem::temp_directory_path() / "worker_job_processor_tests";
        return { path.string() };
    }
}

TEST(WorkerJobProcessor, ProcessesLocalTaskSuccessfully)
{
    fake_image_service image_service;
    fake_object_storage object_storage;
    fake_processor processor;
    fake_result_publisher publisher;

    worker::application::services::job_processor job_processor(
        image_service,
        object_storage,
        processor,
        publisher,
        make_output_directory());

    job::job job_data;
    job_data.id = "job-1";
    job_data.tasks.push_back({"input.png", "Zhang-Suen", true, job::output_format::png});

    const auto result = job_processor.process_job(job_data);

    EXPECT_TRUE(result.has_value());
    EXPECT_EQ(publisher.publish_count, 1);
    EXPECT_TRUE(publisher.last_success);
    EXPECT_EQ(publisher.last_job_id, "job-1");
}

TEST(WorkerJobProcessor, ReportsFailureWhenAllTasksFail)
{
    fake_image_service image_service;
    fake_object_storage object_storage;
    fake_processor processor;
    fake_result_publisher publisher;

    image_service.should_fail_load = true;

    worker::application::services::job_processor job_processor(
        image_service,
        object_storage,
        processor,
        publisher,
        make_output_directory());

    job::job job_data;
    job_data.id = "job-2";
    job_data.tasks.push_back({"input.png", "Zhang-Suen", true, job::output_format::png});

    const auto result = job_processor.process_job(job_data);

    EXPECT_FALSE(result.has_value());
    EXPECT_EQ(publisher.publish_count, 1);
    EXPECT_FALSE(publisher.last_success);
}
