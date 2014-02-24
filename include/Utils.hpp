#ifndef FISHEYE_UTILS_HEADER_FILE
#define FISHEYE_UTILS_HEADER_FILE

#include <opencv2/opencv.hpp>

bool matEq(const cv::Mat & lhs, const cv::Mat & rhs);

cv::Mat RGBToFullValuedHSV(const cv::Mat &image);

#endif
