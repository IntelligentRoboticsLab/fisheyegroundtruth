#ifndef HSV_FILTER_HEADER_FILE
#define HSV_FILTER_HEADER_FILE

#include <opencv2/opencv.hpp>
#include <iosfwd>

class HSVFilter {
    public:
        static constexpr int HMIN = 0;
        static constexpr int HMAX = 180;

        static constexpr int SMIN = 0;
        static constexpr int SMAX = 256;

        HSVFilter();

        void reset();

        void tuneParameters(const cv::Mat & img, const cv::Mat & mask);

        void setHMin(int);
        int  getHMin() const;

        void setHMax(int);
        int  getHMax() const;

        void setSMin(int);
        int  getSMin() const;

        void setSMax(int);
        int  getSMax() const;

        cv::Mat operator()(const cv::Mat &);

    private:
        void tuneParameter(int & param, int start, int end, const cv::Mat & img, const cv::Mat & minMask);

        int hMin_;
        int hMax_;
        int sMin_;
        int sMax_;
};

std::ostream & operator<< (std::ostream &, const HSVFilter &);

#endif
