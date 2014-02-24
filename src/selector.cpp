#include <iostream>
#include <math.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <HSVFilter.hpp>

cv::Mat image, flatImage, globalHist;
bool t = false;
int key = 0;

HSVFilter filter;



void onClick(cv::Point start) {
    //  std::cout << "Clicked at: " << start << '\n';
}

void onDrag(cv::Point start, cv::Point current) {
    auto clone = image.clone();
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

void tuneHist(cv::Mat & pic, const cv::Mat & roi) {
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

    for ( int i = 0; i < pic.rows; ++i )
        for ( int j = 0; j < pic.cols; ++j )
            if ( !inHistogram(globalHist, histSize, ranges, pic.at<cv::Vec3b>(i,j)) ) 
                pic.at<cv::Vec3b>(i,j) = {0,0,0};

    imshow("hist", pic);
}

void onRelease(cv::Point start, cv::Point current) {
    if ( start.x > current.x ) std::swap(start.x, current.x);
    if ( start.y > current.y ) std::swap(start.y, current.y);

    auto clone = image.clone();

    // Image selected
    auto roi = clone(cv::Range(start.y, current.y), cv::Range(start.x, current.x));

    tuneHist(clone, roi);

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

    // tuneParameters(clone);

    imshow("result", clone);
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
        imshow("result", image);
        t = false;

        filter.reset();

        drag = false;
    }
}

void handleTrackbar(int, void*);

int main(int argc, char *argv[]) {
    if ( argc < 2 ) {
        std::cout << "Usage: " << argv[0] << " image_file_name\n";
        return 0;
    }
    image = cv::imread(argv[1]);

    flatImage = convertToFullValuedHSV(image);
    flatImage = convertToFullValuedHSV(flatImage);
    cv::Size kernel(8,8);
    cv::blur(flatImage, flatImage, kernel);
    flatImage = convertToFullValuedHSV(flatImage);

    /* create a window for the video */
    cv::namedWindow("result", CV_WINDOW_AUTOSIZE );
    cv::namedWindow("hist", CV_WINDOW_AUTOSIZE );
    //cv::namedWindow("filter", CV_WINDOW_AUTOSIZE );

#if 0
    cv::createTrackbar( "hmin:", window, &h_min, 180, handleTrackbar );
    cv::createTrackbar( "hmax:", window, &h_max, 180, handleTrackbar );
    cv::createTrackbar( "smin:", window, &s_min, 256, handleTrackbar );
    cv::createTrackbar( "smax:", window, &s_max, 256, handleTrackbar );
#endif
    cv::setMouseCallback("result", mouseHandler, NULL);
    cv::imshow("result", image);

    while( key != 'q' )
        key = cv::waitKey( 1 );

    cv::destroyWindow("result");
    return 0;
}

void handleTrackbar(int, void*){
    auto clone = convertToFullValuedHSV(image);
    cv::Mat imgResult = filter(clone);
    imshow("result", imgResult);
}

