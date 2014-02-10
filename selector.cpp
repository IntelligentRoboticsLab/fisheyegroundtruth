#include <iostream>
#include <math.h>
#include <string>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

cv::Mat frame, img1;
int key = 0;

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
        std::cout << "Released at: " << mousePoint << '\n';
        std::cout << "Range is from " << point << " to " << mousePoint << '\n';

        if ( point.x > mousePoint.x ) std::swap(point.x, mousePoint.x);
        if ( point.y > mousePoint.y ) std::swap(point.y, mousePoint.y);

        auto roi = img1(cv::Range(point.y, mousePoint.y), cv::Range(point.x, mousePoint.x));
        // Perform actual work
        bitwise_not(roi, roi);

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
    cv::namedWindow( "result", CV_WINDOW_AUTOSIZE );
    cv::setMouseCallback("result", mouseHandler, NULL);

    cv::imshow("result", frame);

    while( key != 'q' )
        key = cv::waitKey( 1 );

    cv::destroyWindow("result");
    return 0;
}
