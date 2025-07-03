module;

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <ranges>

export module image_viewer;

export class image_viewer
{
public:
    image_viewer(const std::string &windowName, const int cellWidth = 300, const int cellHeight = 300, const int gridCols = 3)
        : windowName_(windowName), cellWidth_(cellWidth), cellHeight_(cellHeight), gridCols_(gridCols)
    {
    }

    void add_image(const cv::Mat &image)
    {
        if (image.empty())
        {
            throw std::runtime_error("Attempted to add empty image.");
        }

        images_.push_back(image);
        labels_.push_back("");
    }

    void add_image(const cv::Mat &image, const std::string &label)
    {
        if (image.empty())
        {
            throw std::runtime_error("Attempted to add empty image.");
        }

        images_.push_back(image);
        labels_.push_back(label);
    }

    void show()
    {
        auto number_of_images = images_.size();

        if (number_of_images == 0)
        {
            return;
        }

        int rows = std::ceil(number_of_images / (float)gridCols_);

        constexpr auto default_label_height = 30;

        cv::Mat canvas(rows * cellHeight_, gridCols_ * cellWidth_, CV_8UC3, cv::Scalar(50, 50, 50));

        for (const auto &[index, image] : std::views::enumerate(images_))
        {
            cv::Mat rgb_image;

            if (image.channels() == 1)
            {
                cv::cvtColor(image, rgb_image, cv::COLOR_GRAY2BGR);
            }
            else {
                rgb_image = image;
            }

            cv::Mat resized_image;

            cv::resize(rgb_image, resized_image, cv::Size(cellWidth_, cellHeight_ - default_label_height));

            auto row = index / gridCols_;

            auto column = index % gridCols_;

            cv::Rect image_placeholder(column * cellWidth_, row * cellHeight_, cellWidth_, cellHeight_ - default_label_height);

            resized_image.copyTo(canvas(image_placeholder));

            auto label = labels_[index];

            if (!label.empty())
            {
                draw_label(label, column, row, default_label_height, canvas);
            }
        }

        cv::namedWindow(windowName_, cv::WINDOW_NORMAL);

        cv::imshow(windowName_, canvas);

        cv::waitKey(0);
    }

private:
    std::string windowName_;
    int cellWidth_;
    int cellHeight_;
    int gridCols_;
    std::vector<cv::Mat> images_;
    std::vector<std::string> labels_;

    void draw_label(cv::String &label, long long column, long long row, const int default_label_height, cv::Mat &canvas)
    {
        int baseline = 0;

        constexpr auto font = cv::FONT_HERSHEY_SIMPLEX;

        constexpr double font_scale = 0.6;

        constexpr int thickness = 1;

        auto text_size = cv::getTextSize(label, font, font_scale, thickness, &baseline);

        int text_x = column * cellWidth_ + (cellWidth_ - text_size.width) / 2;

        int text_y = row * cellHeight_ + cellHeight_ - (default_label_height - text_size.height) / 2;

        cv::putText(canvas, label, cv::Point(text_x, text_y), font, font_scale, cv::Scalar(255, 255, 255), thickness);
    }
};
