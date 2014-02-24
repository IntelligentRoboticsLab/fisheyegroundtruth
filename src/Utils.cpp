#include <Utils.hpp>

bool matEq(const cv::Mat & lhs, const cv::Mat & rhs) {
    cv::Mat cmp;
    cv::compare(lhs, rhs, cmp, cv::CMP_NE);
    cv::cvtColor(cmp, cmp, CV_BGRA2GRAY, 1);
    return !cv::countNonZero(cmp);
}

cv::Mat RGBToFullValuedHSV(const cv::Mat &image){
    cv::Mat result;
    cv::cvtColor(image, result, CV_BGR2HSV);
    cv::Mat hsv[3];
    cv::split(result, hsv);

    hsv[2].setTo(255); // Remove Value
    cv::merge(hsv, 3, result);

    return result;
}

cv::Mat crop(const cv::Mat & img) {
    auto cropped = img.clone();
    return cropped;
}
