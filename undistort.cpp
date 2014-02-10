#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/calib3d/calib3d.hpp>

#include <vector>
#include <fstream>
#include <iostream>
#include <stdio.h>

int main() {
    // Step 0: Initialize stuff
    cv::Size imagesize = cv::Size(640, 480);
    cv::Mat img = cv::Mat(imageSize, CV_8UC1);
    vector<vector<cv::Point2f> > imagePoints;
    cv::Mat cameraMatrix, distCoeffs;

    cv::Size fieldDimensions = cv::Size();
    std::vector<std::vector<cv::Point3f> > imagePoints;
    std::vector<std::vector<cv::Point3f> > objectPoints;
    // points will correspond to found poi in the image
    std::vector<cv::Point3f > objectPoint();


    char* windowname = "Image";
    cv::namedWindow(windowname, CV_WINDOW_AUTOSIZE);

    // Step 1: Load image
    image = cv::imread(img_name, CV_LOAD_IMAGE_COLOR);

    // Step 2: Find green field / Color stuff
    bool found = false;
    // Step 2a: Select part that will be mostly green, tweak
    // Step 2b: Filter out non-field parts by masking them
    cv::Mat mask;
    // Step 2c: Detect points of interest (corners etc)
    std::vector<cv::Point2f > corners;
    int maxCorners = 20;
    double qualityLevel = 0.01;
    double minDistance = 10;
    cv::goodFeaturesToTrack(image, corners, maxCorners, qualityLevel, minDistance,
                            mask, blockSize=3, useHarrisDetector=false, k=0.04 );

    imagePoints.push_back(corners)
    // Step 2d: Draw feedback

    // Step 3: Transform what we think is the field into a rectangle
    if(found)
    {
        // First determine correct ordering of objectpoints

        // Add found POI to the stack
        imagePoints.push_back(corners);
        // This'll be fine, as long as we don't edit objectPoint
        objectPoints.push_back(objectPoint);

        rms = cv::calibrateCamera(objectPoints,
                                  imagePoints,
                                  imageSize,
                                  cameraMatrix,
                                  distCoeffs,
                                  rvecs,
                                  tvecs,
                                  CV_CALIB_USE_INTRINSIC_GUESS|CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);

    }
    cout << "Re-projection error reported by calibrateCamera: "<< rms << endl;

    // Done for now.

    return 0;
}
