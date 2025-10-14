module;

#include <sstream>
#include <vector>
#include <gtest/gtest.h>
#include <opencv2/core.hpp>

export module tests_common:asserts;

import :normalize;

export namespace skeltest
{
	[[nodiscard]] inline bool is_cv8u1(const cv::Mat& m) noexcept
	{
		return m.type() == CV_8U && m.channels() == 1;
	}

	[[nodiscard]] inline std::string size_str(const cv::Size& s)
	{
		std::ostringstream o;

		o << s.width << 'x' << s.height;

		return o.str();
	}

	[[nodiscard]] inline ::testing::AssertionResult is_binary01(const cv::Mat& m) noexcept
	{
		if (!is_cv8u1(m))
		{
			return ::testing::AssertionFailure() << "Expected CV_8UC1, got type=" << m.type() << " channels=" << m.
				channels();
		}

		cv::Mat gt1;

		cv::compare(m, 1, gt1, cv::CMP_GT);

		return cv::countNonZero(gt1) != 0
			       ? ::testing::AssertionFailure() << "Values > 1 present (expected binary {0,1})."
			       : ::testing::AssertionSuccess();
	}

	[[nodiscard]] inline ::testing::AssertionResult mat_equal01(const cv::Mat& a, const cv::Mat& b) noexcept
	{
		if (a.size() != b.size())
		{
			return ::testing::AssertionFailure() << "Size mismatch: " << a.cols << "x" << a.rows << " vs " << b.cols <<
				"x" << b.rows;
		}
		if (a.type() != b.type())
		{
			return ::testing::AssertionFailure() << "Type mismatch: " << a.type() << " vs " << b.type();
		}

		cv::Mat diff;
		cv::compare(a, b, diff, cv::CMP_NE);

		const int nz = cv::countNonZero(diff);

		if (nz == 0)
		{
			return ::testing::AssertionSuccess();
		}

		std::vector<cv::Point> idx;

		cv::findNonZero(diff, idx);

		if (idx.empty())
		{
			return ::testing::AssertionFailure() << "Matrices differ (" << nz << " pixels).";
		}

		const cv::Point p = idx[0];

		const int av = a.at<uchar>(p);

		const int bv = b.at<uchar>(p);

		std::ostringstream oss;

		oss << "Matrices differ at (" << p.x << "," << p.y << ")"
			<< " a=" << av << " b=" << bv
			<< " (total diffs=" << nz << ")";

		return ::testing::AssertionFailure() << oss.str();
	}

	[[nodiscard]] inline ::testing::AssertionResult subset01(const cv::Mat& sk01, const cv::Mat& in01) noexcept
	{
		if (sk01.size() != in01.size())
		{
			return ::testing::AssertionFailure() << "Size mismatch in subset check.";
		}

		if (sk01.type() != in01.type())
		{
			return ::testing::AssertionFailure() << "Type mismatch in subset check.";
		}

		cv::Mat not_in;
		cv::compare(in01, 0, not_in, cv::CMP_EQ);

		cv::Mat bad;
		cv::bitwise_and(sk01, not_in, bad);

		const int nz = cv::countNonZero(bad);

		if (nz == 0)
		{
			return ::testing::AssertionSuccess();
		}

		std::vector<cv::Point> idx;

		cv::findNonZero(bad, idx);

		if (idx.empty())
		{
			return ::testing::AssertionFailure() << "Skeleton leaks outside input (" << nz << " pixels).";
		}

		const cv::Point p = idx[0];

		return ::testing::AssertionFailure()
			<< "Skeleton leaks outside input at (" << p.x << "," << p.y
			<< "), total leaks=" << nz;
	}
}
