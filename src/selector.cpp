#include <iostream>
#include <math.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <HSVFilter.hpp>
#include <Utils.hpp>

cv::Mat image, flatImage, superFlatImage, globalHist;
cv::Rect bbox;
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

    cv::Size kernel(2,2);
    cv::blur(hist, hist, kernel);

    if ( t )  globalHist += hist;
    else { globalHist = hist; t = true; }

    for ( int i = 0; i < pic.rows; ++i )
        for ( int j = 0; j < pic.cols; ++j )
            if ( !inHistogram(globalHist, histSize, ranges, pic.at<cv::Vec3b>(i,j)) ) 
                pic.at<cv::Vec3b>(i,j) = {0,0,0};
}

void onRelease(cv::Point start, cv::Point current, const cv::Mat & base, std::string frame) {
    if ( start.x > current.x ) std::swap(start.x, current.x);
    if ( start.y > current.y ) std::swap(start.y, current.y);

    auto clone = base.clone();

    // Image selected
    auto roi = clone(cv::Range(start.y, current.y), cv::Range(start.x, current.x));

    tuneHist(clone, roi);

    cv::cvtColor(clone, clone, CV_HSV2BGR);
    cv::rectangle(clone, start, current, CV_RGB(255, 0, 0));

    if ( frame == "flat" )
        bbox = getCropRect(clone);

    imshow(frame, clone(bbox));
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
        cv::namedWindow("flat", CV_WINDOW_AUTOSIZE );
        cv::namedWindow("superflat", CV_WINDOW_AUTOSIZE );
        onRelease(start, mouse, flatImage, "flat");
        onRelease(start, mouse, superFlatImage, "superflat");

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

int main(int argc, char *argv[]) {
    if ( argc < 2 ) {
        std::cout << "Usage: " << argv[0] << " image_file_name\n";
        return 0;
    }
    image = cv::imread(argv[1]);

    flatImage = image.clone();

    superFlatImage = RGBToFullValuedHSV(image);
//    superFlatImage = RGBToFullValuedHSV(superFlatImage);

    cv::Size kernel(8,8);
    cv::blur(flatImage, flatImage, kernel);
    cv::blur(superFlatImage, superFlatImage, kernel);

    flatImage = RGBToFullValuedHSV(flatImage);
    superFlatImage = RGBToFullValuedHSV(superFlatImage);

    /* create a window for the video */
    cv::namedWindow("result", CV_WINDOW_AUTOSIZE );

    cv::setMouseCallback("result", mouseHandler, NULL);
    cv::imshow("result", image);

    while( key != 'q' && key != 27 /* ESC */ )
        key = cv::waitKey( 1 );

    cv::destroyWindow("result");
    return 0;
}
