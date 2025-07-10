module;

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <algorithm>

export module visual_inspector:visualiser;

import :image_container;

namespace visual_inspector
{
	export class visualiser
	{
	public:
		explicit visualiser(const int cell_width = 300, const int cell_height = 300, const int header_height = 40,
		                    const int name_col_width = 150)
			: cell_width_(cell_width), cell_height_(cell_height),
			  header_height_(header_height), name_col_width_(name_col_width)
		{
		}

		void add_benchmark_image_container(const image_container& image_container)
		{
			benchmark_image_containers_.push_back(image_container);
			max_cols_ = std::max(max_cols_, image_container.size());
		}

		void show(const std::string& window_name) const
		{
			if (benchmark_image_containers_.empty())
			{
				return;
			}

			const int rows = static_cast<int>(benchmark_image_containers_.size());
			const int columns = static_cast<int>(max_cols_);

			const int canvas_width = name_col_width_ + columns * cell_width_;
			const int canvas_height = header_height_ + rows * cell_height_;

			cv::Mat canvas(canvas_height, canvas_width, CV_8UC3, background_color_);

			draw_header_row(canvas, columns);

			draw_benchmark_rows(canvas, rows, columns);

			cv::namedWindow(window_name, cv::WINDOW_NORMAL);
			cv::resizeWindow(window_name, std::min(1920, canvas_width), std::min(1080, canvas_height));
			cv::imshow(window_name, canvas);
			cv::waitKey(0);
		}

	private:
		const cv::Scalar background_color_ = cv::Scalar(30, 30, 30);
		const cv::Scalar cell_border_color_ = cv::Scalar(80, 80, 80);
		const cv::Scalar text_color_ = cv::Scalar(230, 230, 230);
		const cv::Scalar text_shadow_color_ = cv::Scalar(0, 0, 0);
		const cv::Scalar header_bg_color_ = cv::Scalar(50, 50, 50);
		const cv::Scalar header_border_color_ = cv::Scalar(100, 100, 100);

		const int cell_width_;
		const int cell_height_;
		const int header_height_;
		const int name_col_width_;
		size_t max_cols_ = 0;

		std::vector<image_container> benchmark_image_containers_;

		void draw_header_row(cv::Mat& canvas, const int columns) const
		{
			constexpr auto header_label = "Skeletonization Algorithms Comparison";

			const auto total_width = columns * cell_width_;

			draw_text_box(canvas, header_label, name_col_width_, 0, total_width, header_height_);
		}

		void draw_benchmark_rows(cv::Mat& canvas, const int rows, const int cols) const
		{
			for (int row = 0; row < rows; ++row)
			{
				const auto& benchmark_image_container = benchmark_image_containers_[row];
				const auto y_offset = header_height_ + row * cell_height_;

				draw_text_box(canvas, benchmark_image_container.name(), 0, y_offset, name_col_width_, cell_height_,
				              true);

				for (auto col = 0; col < cols; ++col)
				{
					const auto x_offset = name_col_width_ + col * cell_width_;

					if (col < benchmark_image_container.size())
					{
						draw_image_cell(canvas, benchmark_image_container.image(col),
						                benchmark_image_container.label(col), x_offset, y_offset);
					}
					else
					{
						draw_empty_cell(canvas, x_offset, y_offset);
					}
				}
			}
		}

		void draw_text_box(cv::Mat& canvas, const std::string& text, const int x, const int y, const int width,
		                   const int height,
		                   const bool vertical_center = false) const
		{
			// Background & border
			cv::rectangle(canvas, cv::Rect(x, y, width, height), header_bg_color_, cv::FILLED);
			cv::rectangle(canvas, cv::Rect(x, y, width, height), header_border_color_, 1);

			// Text positioning
			auto baseline = 0;
			constexpr int font = cv::FONT_HERSHEY_SIMPLEX;
			constexpr auto font_scale = 0.7;
			constexpr auto thickness = 1;

			const auto text_size = cv::getTextSize(text, font, font_scale, thickness, &baseline);

			const auto text_x = x + (width - text_size.width) / 2;
			const auto text_y = vertical_center ? (y + (height + text_size.height) / 2) : (y + text_size.height + 5);

			// Text shadow
			cv::putText(canvas, text, {text_x + 1, text_y + 1}, font, font_scale, text_shadow_color_, thickness + 2);
			cv::putText(canvas, text, {text_x, text_y}, font, font_scale, text_color_, thickness);
		}

		void draw_image_cell(cv::Mat& canvas, const cv::Mat& image, const std::string& label, int x, int y) const
		{
			constexpr auto label_height = 40;

			const auto display_image = prepare_image_for_display(image);
			cv::Mat resized_image;
			cv::resize(display_image, resized_image, cv::Size(cell_width_, cell_height_ - label_height));

			const cv::Rect img_rect(x, y, cell_width_, cell_height_ - label_height);
			resized_image.copyTo(canvas(img_rect));

			// Draw border around cell
			cv::rectangle(canvas, cv::Rect(x, y, cell_width_, cell_height_), cell_border_color_, 1);

			if (!label.empty())
			{
				draw_label(canvas, label, x, y + cell_height_ - label_height, cell_width_, label_height);
			}
		}

		void draw_empty_cell(cv::Mat& canvas, const int x, const int y) const
		{
			constexpr auto label_height = 40;
			cv::rectangle(canvas, cv::Rect(x, y, cell_width_, cell_height_), cell_border_color_, 1);

			const cv::Rect img_rect(x, y, cell_width_, cell_height_ - label_height);
			const cv::Mat empty_placeholder(img_rect.size(), CV_8UC3, cv::Scalar(60, 60, 60));
			empty_placeholder.copyTo(canvas(img_rect));
		}

		static cv::Mat prepare_image_for_display(const cv::Mat& image)
		{
			if (image.channels() == 1)
			{
				cv::Mat rgb;
				cv::cvtColor(image, rgb, cv::COLOR_GRAY2BGR);
				return rgb;
			}
			return image;
		}

		void draw_label(cv::Mat& canvas, const std::string& label, const int x, const int y, const int width,
		                const int height) const
		{
			// Background for label
			cv::rectangle(canvas, cv::Rect(x, y, width, height), header_bg_color_, cv::FILLED);
			cv::rectangle(canvas, cv::Rect(x, y, width, height), header_border_color_, 1);

			auto baseline = 0;
			constexpr int font = cv::FONT_HERSHEY_SIMPLEX;
			constexpr auto font_scale = 0.6;
			constexpr auto thickness = 1;

			const auto text_size = cv::getTextSize(label, font, font_scale, thickness, &baseline);
			const auto text_x = x + (width - text_size.width) / 2;
			const auto text_y = y + (height + text_size.height) / 2;

			// Text shadow
			cv::putText(canvas, label, {text_x + 1, text_y + 1}, font, font_scale, text_shadow_color_, thickness + 2);
			cv::putText(canvas, label, {text_x, text_y}, font, font_scale, text_color_, thickness);
		}
	};
}
