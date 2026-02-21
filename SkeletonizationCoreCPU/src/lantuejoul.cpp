#include "SkeletonizationCoreCPU/lantuejoul.hpp"

namespace skeletonizer::cpu::algorithms
{
	void lantuejoul::apply(cv::Mat& binary_image) const
	{
		CV_Assert(binary_image.type() == CV_8UC1);

		const cv::Mat B = cv::getStructuringElement(
			cv::MORPH_ELLIPSE,
			cv::Size(3, 3)
		);

		cv::Mat skelet = cv::Mat::zeros(binary_image.size(), CV_8UC1);
		cv::Mat eroded = binary_image.clone();
		cv::Mat tmp, opened, subset;

		while (cv::countNonZero(eroded) > 0)
		{
			// opened = (X ⊖ nB) ∘ B
			cv::erode(eroded, tmp, B, cv::Point(-1, -1), 1, cv::BORDER_CONSTANT, 0);
			cv::dilate(tmp, opened, B, cv::Point(-1, -1), 1, cv::BORDER_CONSTANT, 0);

			// subset = eroded \ opened  (set difference)
			subset = eroded.clone();
			subset.setTo(0, opened); // where opened==1 => remove

			// skelet = skelet ∪ subset
			cv::bitwise_or(skelet, subset, skelet);

			// eroded = eroded ⊖ B
			cv::erode(eroded, eroded, B, cv::Point(-1, -1), 1, cv::BORDER_CONSTANT, 0);
		}

		skelet.copyTo(binary_image);
	}
}
