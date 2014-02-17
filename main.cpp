#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>
int h_max;
int h_min;
int s_min;
int s_max;
int v_min;
int v_max;


int main(int argc, char* argv[]) {
    if ( argc < 4 ) {
        std::cout << "Usage: " << argv[0] << " image_file_name blur_kernel_w blur_kernel_h\n";
        return 0;
    }

    int blur_kernel_w = std::stoi(std::string(argv[2]));
    int blur_kernel_h = std::stoi(std::string(argv[3]));

    // Step 1: Load image
    cv::Mat image = cv::imread(argv[1], CV_LOAD_IMAGE_GRAYSCALE);

    // Find the field
    // 1. blur
    cv::Mat blurred;
    cv::Size kernel(blur_kernel_w, blur_kernel_h);
    cv::blur(image, blurred, kernel);

    cv::Mat blurredcolor;
    cvtColor(blurred, blurredcolor, CV_GRAY2RGB);


    // 2. find corners
    std::vector<cv::Point2f> corners;
    cv::goodFeaturesToTrack( blurred, corners, 280, 0.05 /* quality magic */, 3 /* min distance corners magic */, cv::Mat(), 3 /* Block size Shi-Tomasi magic */, false /* fuck harris */, 0.04 );

    // Select points
    // Find point correspondences
    // Undistort
    // print result

    std::string window = "result";
    cv::namedWindow( window, CV_WINDOW_AUTOSIZE );

    for (auto & p : corners) {
        cv::circle( blurredcolor, p, 4, CV_RGB(255, 0, 0), -1, 8, 0 );
    }
    cv::imshow(window, blurredcolor);

    int key = 0;
    while( key != 'q' )
        key = cv::waitKey( 1 );

    cv::destroyWindow("result");
    return 0;
}

