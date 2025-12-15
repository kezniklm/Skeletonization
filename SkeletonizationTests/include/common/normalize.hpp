#pragma once

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace skeltest
{
    inline cv::Mat to01(const cv::Mat& in)
    {
        CV_Assert(in.channels() == 1);

        cv::Mat u8;

        if (in.type() == CV_8U)
        {
            u8 = in;
        }
        else
        {
            in.convertTo(u8, CV_8U);
        }

        cv::Mat out;
        cv::threshold(u8, out, 0, 1, cv::THRESH_BINARY);

        return out;
    }
}
