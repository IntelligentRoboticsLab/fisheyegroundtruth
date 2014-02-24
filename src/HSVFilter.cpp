#include <HSVFilter.hpp>

#include <ostream>

#include <Utils.hpp>

HSVFilter::HSVFilter() {
    reset();
}

void HSVFilter::reset() {
    hMin_ = HMIN;
    hMax_ = HMAX;
                
    sMin_ = SMIN;
    sMax_ = SMAX;
}

cv::Mat HSVFilter::operator()(const cv::Mat & img) {
    auto clone = img;

    cv::Scalar minColor = cvScalar(hMin_, sMin_, 0);
    cv::Scalar maxColor = cvScalar(hMax_, sMax_, 256);

    cv::inRange(clone, minColor, maxColor, clone);
    cv::cvtColor(clone, clone, CV_GRAY2RGB);

    return clone;
}

void HSVFilter::tuneParameters(const cv::Mat & img, const cv::Mat & mask) {
    reset();

    tuneParameter(hMax_, HMAX, 0, img, mask);
    tuneParameter(hMin_, 0, HMIN, img, mask);
    tuneParameter(sMax_, SMAX, 0, img, mask);
    tuneParameter(sMin_, 0, SMIN, img, mask);
}

void HSVFilter::tuneParameter(int & param, int start, int end, const cv::Mat & img, const cv::Mat & minMask) {
    cv::Mat filt;
    int change = -1;
    int lowerbound = start, upperbound = end;

    param = lowerbound;

    while ( true ) {
        if ( change == 0 ) break;

        filt = operator()(img);
        cv::Mat result;
        cv::bitwise_and(filt, minMask, result);

        if ( matEq(minMask, result) ) {
            lowerbound = param;
            change = (upperbound - param)/2;
        }
        else {
            upperbound = param;
            change = -(param - lowerbound)/2;
        }
        param += change;
        //std::cout << "Change = " << change << "\t- Value = " << param << '\n';
    }
}

std::ostream & operator<<(std::ostream & os, const HSVFilter & filter) {
    os << "HMin: " << filter.getHMin() << "\tHMax: " << filter.getHMax() << "\tSMin: " << filter.getSMin() << "\tgetSMax: " << filter.getSMax() << '\n';
    return os;
}

int  HSVFilter::getHMin() const { return hMin_; }
int  HSVFilter::getHMax() const { return hMax_; }
int  HSVFilter::getSMin() const { return sMin_; }
int  HSVFilter::getSMax() const { return sMax_; }

void HSVFilter::setHMin(int h) { hMin_ = h; } 
void HSVFilter::setHMax(int h) { hMax_ = h; }
void HSVFilter::setSMin(int s) { sMin_ = s; }
void HSVFilter::setSMax(int s) { sMax_ = s; }
