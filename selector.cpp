#include <iostream>
#include <math.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#define CV_HSV( h, s, v ) cvScalar( (h), (s), (v) )

cv::Mat frame, img1;
int key = 0;

int h_max=0;
int h_min=0;
int s_min=0;
int s_max=0;

void processImage(cv::Mat &image){
    cv::cvtColor(image, image, CV_BGR2HSV);
    cv::Mat hsv[3];
    cv::split(img1, hsv);
    hsv[2].setTo(255);   // TODO remove this?
    cv::merge(hsv, 3, image);
}

void handleTrackbar(int, void*){
    img1 = frame.clone();
    processImage(img1);
    cv::Mat imgResult;
    cv::Scalar min_color = CV_HSV(h_min, s_min, 0);
    cv::Scalar max_color = CV_HSV(h_max, s_max, 256);
    cv::inRange(img1, min_color, max_color, imgResult);
    cv::cvtColor( imgResult, imgResult, CV_GRAY2RGB);
    imshow("result", imgResult);
}

void mouseHandler(int event, int x, int y, int flags, void* param) {
    static cv::Point point;
    static bool drag = false;
    /* user press left button */
    if (event == CV_EVENT_LBUTTONDOWN && !drag) {
        point = cv::Point(x, y);
        std::cout << "Clicked at: " << point << '\n';
        drag = 1;
    }
    /* user drag the mouse */
    else if (event == CV_EVENT_MOUSEMOVE && drag) {
        img1 = frame.clone();
        auto mousePoint = cv::Point(x, y);
        // Draws a rectangle
        cv::rectangle(img1, point, mousePoint, CV_RGB(255, 0, 0));
        std::cout << "Moved at: " << mousePoint << '\n';
        cv::imshow("result", img1);
    }
    /* user release left button */
    else if (event == CV_EVENT_LBUTTONUP && drag) {
        img1 = frame.clone();
        auto mousePoint = cv::Point(x, y);
        //std::cout << "Released at: " << mousePoint << '\n';
        //std::cout << "Range is from " << point << " to " << mousePoint << '\n';
        if ( point.x > mousePoint.x ) std::swap(point.x, mousePoint.x);
        if ( point.y > mousePoint.y ) std::swap(point.y, mousePoint.y);

        // Perform actual work
        processImage(img1);

        // Replace some value in selected square
        auto roi = img1(cv::Range(point.y, mousePoint.y), cv::Range(point.x, mousePoint.x));

        // Use histograms
        int hbins = 30, sbins = 32;
        int histSize[] = {hbins, sbins};
        float hranges[] = { 0, 180 };
        float sranges[] = { 0, 256 };
        const float* ranges[] = { hranges, sranges };
        cv::MatND hist;
        int channels[] = {0, 1};

        calcHist( &roi, 1, channels, cv::Mat(), // do not use mask
                  hist, 2, histSize, ranges,
                  true, // the histogram is uniform
                  false );

        // Display histogram
        double maxVal = 0;
        minMaxLoc(hist, 0, &maxVal, 0, 0);
        int scale = 10;
        double threshold = 0.1;
        cv::Mat histImg = cv::Mat::zeros(sbins*scale, hbins*scale, CV_8UC3);
        std::vector< cv::Point > usedBins;

        for( int h = 0; h < hbins; h++ )
        {
            for( int s = 0; s < sbins; s++ )
            {
                float binVal = hist.at<float>(h, s);
                int intensity = std::round(binVal*255/maxVal);
                cv::rectangle( histImg, cv::Point(h*scale, s*scale),
                               cv::Point( (h+1)*scale - 1, (s+1)*scale - 1),
                               cv::Scalar::all(intensity), CV_FILLED );
                if (intensity > threshold) {
                    //std::cout << h << "," << s << " : " << binVal << std::endl;
                    usedBins.push_back(cv::Point(h,s));
                }
            }
        }

        cv::namedWindow( "H-S Histogram", 1 );
        cv::imshow( "H-S Histogram", histImg );

        // Convert bins to actual hsv values, note that regions can be separate

        // Colorfilter
        h_min = 15; h_max = 256; s_min = 16; s_max = 256;
        cv::Mat imgResult(img1.size(), IPL_DEPTH_8U, 3);
        cv::Scalar min_color = CV_RGB(h_min, s_min, 254);
        cv::Scalar max_color = CV_RGB(h_max, s_max, 256);
        cv::inRange(img1, min_color, max_color, imgResult);

        cv::cvtColor(img1, img1, CV_HSV2BGR);
        cv::rectangle(img1, point, mousePoint, CV_RGB(255, 0, 0));
        imshow("result", img1);
        drag = 0;
    }
    /* user click right button: reset all */
    else if (event == CV_EVENT_RBUTTONUP) {
        imshow("result", frame);
        drag = 0;
    }
}

int main(int argc, char *argv[]) {
    if ( argc < 2 ) {
        std::cout << "Usage: " << argv[0] << " image_file_name\n";
        return 0;
    }
    frame = cv::imread(argv[1]);

    /* create a window for the video */
    std::string window = "result";
    cv::namedWindow( window, CV_WINDOW_AUTOSIZE );
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
