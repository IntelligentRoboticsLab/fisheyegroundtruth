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

cv::Rect getCropRect(const cv::Mat & img) {
    auto cropped = img.clone();
    cv::cvtColor(cropped, cropped, CV_RGB2GRAY);
    cv::threshold(cropped, cropped, 1, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contour;
    cv::findContours(cropped, contour, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_SIMPLE);

    std::vector<cv::Point> result;
    for ( auto & v : contour )
        result.insert(result.end(), v.begin(), v.end());

    return cv::boundingRect(result);
}

bool inHistogram(const cv::Mat & hist, const int histSize[], const float* ranges[], const cv::Vec3b & pixel) {
    if ( hist.at<float>( ( pixel[0] * histSize[0] ) / ranges[0][1], ( pixel[1] * histSize[1] ) / ranges[1][1] ) != 0 )
        return true;
    return false;
}

