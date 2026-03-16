/**
*
* @file image_factory.hpp
* @author Matej Keznikl (matej.keznikl@gmail.com)
* @brief Provides synthetic image generators for equivalence tests.
*
* This header defines deterministic binary patterns used for validating
* skeletonization output consistency.
*
* Main responsibilities:
* - create reusable synthetic test patterns
* - expose generator collection for parametrized tests
* - cache generated images for repeated test runs
*
* @version 1.0
* @date 2026-03-16
*/

#pragma once

#include <array>
#include <random>
#include <vector>

#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc.hpp>

namespace skeltest
{
	/**
	 * @brief Creates a zero-initialized binary image.
	 *
	 * @param width Image width.
	 * @param height Image height.
	 * @return Binary image matrix.
	 */
	[[nodiscard]] inline cv::Mat mk(int width, int height)
	{
		return cv::Mat(height, width, CV_8U, cv::Scalar(0));
	}

	/**
	 * @brief Generates plus-sign test pattern.
	 * @return Generated binary image.
	 */
	[[nodiscard]] inline cv::Mat generate_plus_sign()
	{
		auto image = mk(128, 128);

		constexpr int thickness = 5;

		cv::line(image, {10, 64}, {118, 64}, cv::Scalar(1), thickness);
		cv::line(image, {64, 10}, {64, 118}, cv::Scalar(1), thickness);

		return image;
	}

	/**
	 * @brief Generates hollow rectangle with tail extension.
	 * @return Generated binary image.
	 */
	[[nodiscard]] inline cv::Mat generate_hollow_rectangle_with_tail()
	{
		auto image = mk(160, 120);

		constexpr int thickness = 5;

		cv::rectangle(image, {20, 20}, {140, 100}, cv::Scalar(1), thickness);
		cv::line(image, {80, 100}, {80, 115}, cv::Scalar(1), thickness);

		return image;
	}

	/**
	 * @brief Generates two touching circular blobs.
	 * @return Generated binary image.
	 */
	[[nodiscard]] inline cv::Mat generate_two_touching_blobs()
	{
		auto image = mk(100, 100);

		cv::circle(image, {45, 50}, 22, cv::Scalar(1), cv::FILLED);
		cv::circle(image, {55, 50}, 22, cv::Scalar(1), cv::FILLED);

		return image;
	}

	/**
	 * @brief Generates zig-zag polyline shape.
	 * @return Generated binary image.
	 */
	[[nodiscard]] inline cv::Mat generate_zig_zag()
	{
		auto image = mk(200, 60);

		constexpr int thickness = 3;

		const std::vector<cv::Point> pts{
			{5, 50}, {40, 10}, {80, 50}, {120, 10}, {160, 50}, {195, 10}
		};

		for (std::size_t i = 1; i < pts.size(); ++i)
		{
			cv::line(image, pts[i - 1], pts[i], cv::Scalar(1), thickness);
		}

		return image;
	}

	/**
	 * @brief Generates filled blob with an interior hole.
	 * @return Generated binary image.
	 */
	[[nodiscard]] inline cv::Mat generate_blob_with_hole()
	{
		auto image = mk(128, 128);

		cv::circle(image, {64, 64}, 50, cv::Scalar(1), cv::FILLED);
		cv::circle(image, {64, 64}, 15, cv::Scalar(0), cv::FILLED);

		return image;
	}

	/**
	 * @brief Generates short diagonal stroke image.
	 * @return Generated binary image.
	 */
	[[nodiscard]] inline cv::Mat generate_small_diagonal()
	{
		auto image = mk(9, 9);

		cv::line(image, {1, 1}, {7, 7}, cv::Scalar(1), 1);

		return image;
	}

	/**
	 * @brief Generates image with one foreground pixel.
	 * @return Generated binary image.
	 */
	[[nodiscard]] inline cv::Mat generate_single_pixel()
	{
		auto image = mk(17, 17);

		image.at<uchar>(8, 8) = 1;

		return image;
	}

	/**
	 * @brief Generates empty (all-background) test image.
	 * @return Generated binary image.
	 */
	[[nodiscard]] inline cv::Mat generate_empty_image()
	{
		return mk(64, 64);
	}

	/**
	 * @brief Generates full (all-foreground) test image.
	 * @return Generated binary image.
	 */
	[[nodiscard]] inline cv::Mat generate_full_image()
	{
		auto image = mk(64, 64);
		image.setTo(1);
		return image;
	}

	/**
	 * @brief Generates deterministic random stroke pattern.
	 * @return Generated binary image.
	 */
	[[nodiscard]] inline cv::Mat gen_random_strokes()
	{
		auto image = mk(180, 180);

		std::mt19937 rng(42);
		std::uniform_int_distribution<int> x(5, 175), y(5, 175);

		for (int i = 0; i < 40; ++i)
		{
			const int thickness = i % 3 + 1;
			cv::line(image, {x(rng), y(rng)}, {x(rng), y(rng)}, cv::Scalar(1), thickness);
		}

		return image;
	}

	/// Alias for test image generator function pointer.
	using generator_function = cv::Mat(*)();

	/**
	 * @brief Returns all registered generator functions.
	 *
	 * @return Array of generator function pointers.
	 */
	inline const std::array<generator_function, 10>& generators()
	{
		static constexpr std::array<generator_function, 10> g{
			generate_plus_sign,
			generate_hollow_rectangle_with_tail,
			generate_two_touching_blobs,
			generate_zig_zag,
			generate_blob_with_hole,
			generate_small_diagonal,
			generate_single_pixel,
			generate_empty_image,
			generate_full_image,
			gen_random_strokes
		};

		return g;
	}

	/**
	 * @brief Generates all test images.
	 *
	 * @return Vector of generated binary test images.
	 */
	[[nodiscard]] inline std::vector<cv::Mat> make_images()
	{
		std::vector<cv::Mat> image_vector;
		const auto& gens = generators();

		for (const auto gen : gens)
		{
			image_vector.emplace_back(gen());
		}

		return image_vector;
	}

	/**
	 * @brief Returns cached test images.
	 *
	 * @return Reference to generated test images.
	 */
	inline const std::vector<cv::Mat>& test_images()
	{
		static const std::vector<cv::Mat> images = make_images();
		return images;
	}
}
