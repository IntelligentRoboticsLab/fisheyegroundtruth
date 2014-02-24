#ifndef FISHEYE_UTILS_HEADER_FILE
#define FISHEYE_UTILS_HEADER_FILE

#include <opencv2/opencv.hpp>

/**
 * @brief This function returns whether two Mats are exactly the same.
 */
bool matEq(const cv::Mat & lhs, const cv::Mat & rhs);

/**
 * @brief This function converts an RGB Mat into HSV, setting value to 255.
 *
 * @return A clone of the original image.
 */
cv::Mat RGBToFullValuedHSV(const cv::Mat &image);

/**
 * @brief This function returns whether a given pixel is contained in the given histogram.
 *
 * This function currently works for a VERY STRICT subset of cases, as in it only works
 * on HSV images at full value (it ignores V) and on an appropriate histogram.
 *
 */
bool inHistogram(const cv::Mat & hist, const int histSize[], const float* ranges[], const cv::Vec3b & pixel);

/**
 * @brief This function returns the inner ROI cutting black borders.
 */
cv::Rect getCropRect(const cv::Mat & img);

#endif
