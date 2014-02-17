#include <iostream>
#include <math.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::Mat frame, globalHist;
bool t = false;
int key = 0;

int h_max=180;
int h_min=0;
int s_min=0;
int s_max=256;

void printParams() {
    std::cout << "S_MIN: " << s_min << '\n';
    std::cout << "S_MAX: " << s_max << '\n';
    std::cout << "H_MIN: " << h_min << '\n';
    std::cout << "H_MAX: " << h_max << '\n';
}

cv::Mat convertToFullValuedHSV(const cv::Mat &image){
    cv::Mat result;

    cv::cvtColor(image, result, CV_BGR2HSV);

    cv::Mat hsv[3];
    cv::split(result, hsv);

    // Remove Value
    hsv[2].setTo(255);

    cv::merge(hsv, 3, result);

    return result;
}

void onClick(cv::Point start) {
//  std::cout << "Clicked at: " << start << '\n';
}

void onDrag(cv::Point start, cv::Point current) {
    auto clone = frame.clone();
    // Draws a rectangle
    cv::rectangle(clone, start, current, CV_RGB(255, 0, 0));
//  std::cout << "Moved at: " << current << '\n';
    cv::imshow("result", clone);
}

bool inHistogram(const cv::Mat & hist, const int histSize[], const float* ranges[], const cv::Vec3b & pixel) {
    if ( hist.at<float>( ( pixel[0] * histSize[0] ) / ranges[0][1], ( pixel[1] * histSize[1] ) / ranges[1][1] ) != 0 )
        return true;
    return false;
}

cv::Mat getFilteredImage() {
    auto clone = convertToFullValuedHSV(frame);

    cv::Scalar min_color = cvScalar(h_min, s_min, 0);
    cv::Scalar max_color = cvScalar(h_max, s_max, 256);
    cv::inRange(clone, min_color, max_color, clone);
    cv::cvtColor( clone, clone, CV_GRAY2RGB);

    return clone;
}

bool matEq(const cv::Mat & lhs, const cv::Mat & rhs) {
    cv::Mat cmp;
    cv::compare(lhs, rhs, cmp, cv::CMP_NE);
    cv::cvtColor(cmp, cmp, CV_BGRA2GRAY, 1);
    return !cv::countNonZero(cmp);
}

void tuneParameter(int & param, int start, int end, const cv::Mat & pic) {
    cv::Mat filt;
    int change = -1;
    int truechange;
    int lowerbound = start, upperbound = end;

    param = lowerbound;

    while ( true ) {
        if ( change == 0 || change == 1 ) break;

        filt = getFilteredImage();
        cv::Mat result;
        cv::bitwise_and(filt, pic, result);

        if ( matEq(pic, result) ) {
            lowerbound = param;
            truechange = (upperbound - param)/2;
            param += (upperbound - param)/2;
        }
        else {
            upperbound = param;
            truechange = -(param - lowerbound)/2;
            param -= (param - lowerbound)/2;
        }
        change = ( param - lowerbound );
        std::cout << "Change = " << truechange << '\n';
    }
}

void tryExpand(const cv::Mat & pic) {

    std::cout << "### HMAX ###\n";
    tuneParameter(h_max, 180, h_min, pic);

    std::cout << "### HMIN ###\n";
    tuneParameter(h_min, 0, 180, pic);

    std::cout << "### SMAX ###\n";
    tuneParameter(s_max, 256, s_min, pic);

    std::cout << "### SMIN ###\n";
    tuneParameter(s_min, 0, 256, pic);



    printParams();

    auto filt = getFilteredImage();
    imshow("sel", pic);
    imshow("result", filt);
}

void onRelease(cv::Point start, cv::Point current) {
    if ( start.x > current.x ) std::swap(start.x, current.x);
    if ( start.y > current.y ) std::swap(start.y, current.y);

    // Perform actual work
    auto clone = convertToFullValuedHSV(frame);

    // Replace some value in selected square
    auto roi = clone(cv::Range(start.y, current.y), cv::Range(start.x, current.x));

    // Use histograms
    int hbins = 30, sbins = 32;
    int histSize[] = {hbins, sbins};
    float hranges[] = { 0, 180 };
    float sranges[] = { 0, 256 };
    const float* ranges[] = { hranges, sranges };
    int channels[] = {0, 1};

    cv::Mat hist;

    calcHist( &roi, 1, channels, cv::Mat(), // do not use mask
            hist, 2, histSize, ranges,
            true, // the histogram is uniform
            false );

    if ( t )  globalHist += hist;
    else { globalHist = hist; t = true; }

    for ( int i = 0; i < clone.rows; ++i )
        for ( int j = 0; j < clone.cols; ++j )
            if ( !inHistogram(globalHist, histSize, ranges, clone.at<cv::Vec3b>(i,j)) )
                clone.at<cv::Vec3b>(i,j) = {0,0,0};

    cv::cvtColor(clone, clone, CV_HSV2BGR);
    cv::rectangle(clone, start, current, CV_RGB(255, 0, 0));

    imshow("hist", globalHist);

    tryExpand(clone);

    //imshow("result", clone);
}

void mouseHandler(int event, int x, int y, int flags, void* param) {
    static cv::Point start(0,0);
    static bool drag = false;

    cv::Point mouse(x,y);

    /* user press left button */
    if (event == CV_EVENT_LBUTTONDOWN && !drag) {
        onClick(mouse);

        start = mouse;
        drag = true;
    }
    /* user drag the mouse */
    else if (event == CV_EVENT_MOUSEMOVE && drag) {
        onDrag(start, mouse);
    }
    /* user release left button */
    else if (event == CV_EVENT_LBUTTONUP && drag) {
        onRelease(start, mouse);

        drag = false;
    }
    /* user click right button: reset all */
    else if (event == CV_EVENT_RBUTTONUP) {
        imshow("result", frame);
        t = false;

        h_max=180;
        h_min=0;
        s_min=0;
        s_max=256;

        drag = false;
    }
}

void handleTrackbar(int, void*);

int main(int argc, char *argv[]) {
    if ( argc < 2 ) {
        std::cout << "Usage: " << argv[0] << " image_file_name\n";
        return 0;
    }
    frame = cv::imread(argv[1]);
    cv::Size kernel(8,8);
    cv::blur(frame, frame, kernel);

    /* create a window for the video */
    std::string window = "result";
    cv::namedWindow( window, CV_WINDOW_AUTOSIZE );

    cv::namedWindow( "hist", CV_WINDOW_AUTOSIZE );
    cv::namedWindow( "sel", CV_WINDOW_AUTOSIZE );
    cv::createTrackbar( "hmin:", window, &h_min, 180, handleTrackbar );
    cv::createTrackbar( "hmax:", window, &h_max, 180, handleTrackbar );
    cv::createTrackbar( "smin:", window, &s_min, 256, handleTrackbar );
    cv::createTrackbar( "smax:", window, &s_max, 256, handleTrackbar );
    cv::setMouseCallback("result", mouseHandler, NULL);
    cv::imshow(window, frame);

    while( key != 'q' )
        key = cv::waitKey( 1 );

    cv::destroyWindow("result");
    return 0;
}

void handleTrackbar(int, void*){
    auto clone = convertToFullValuedHSV(frame);

    cv::Mat imgResult;
    cv::Scalar min_color = cvScalar(h_min, s_min, 0);
    cv::Scalar max_color = cvScalar(h_max, s_max, 256);
    cv::inRange(clone, min_color, max_color, imgResult);
    cv::cvtColor( imgResult, imgResult, CV_GRAY2RGB);
    imshow("result", imgResult);
}

