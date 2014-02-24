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

void showField(int, void*)
{
    IplImage* test=cvCreateImage(cvGetSize(img), IPL_DEPTH_8U, 3);
    CvScalar min_color = CV_RGB(mR_val,mG_val,mB_val);
    CvScalar max_color = CV_RGB(MAR_val,MAG_val,MAB_val);
    cvInRangeS(imgHsv, min_color,max_color, imgResult); //search for the color in image

    cvCvtColor( imgResult, test,CV_GRAY2RGB);
    cvShowImage("Result", test);
    cvReleaseImage(&test);
}

int main(int argc, char* argv[]) {
    if ( argc < 1 ) {
        std::cout 2<< "Usage: " << argv[0] << " image_file_name\n";
        return 0;
    }

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

    char* window = "Image";
    cv::namedWindow(window, CV_WINDOW_AUTOSIZE);

    // Step 1: Load image
    image = cv::imread(argv[1], CV_LOAD_IMAGE_COLOR);

    // Step 2: Find green field / Color stuff
    bool found = false;
    // Step 2a: Select part that will be mostly green, tweak
    h_max=0;
    h_min=0;
    s_min=0;
    s_max=0;
    v_min=0;
    v_max=0;
    createTrackbar( "hmin:", window, &h_min, 256, showField );
    createTrackbar( "hmax:", window, &h_max, 256, showField );
    createTrackbar( "smin:", window, &s_min, 256, showField );
    createTrackbar( "smax:", window, &s_max, 256, showField );
    createTrackbar( "vmin:", window, &v_min, 256, showField );
    createTrackbar( "vmax:", window, &v_max, 256, showField );
    /**
    // Step 2b: Filter out non-field parts by masking them
    cv::Mat mask;

    // Step 2c: Detect points of interest (corners etc)
    distCoeffs = cv::Mat::zeros(8, 1, CV_64F);
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
        // First determine correct ordering of objectpoints (RANSAC?)

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
                                  CV_CALIB_FIX_K4|CV_CALIB_FIX_K5);

    }
    cout << "Re-projection error reported by calibrateCamera: "<< rms << endl;
    bool calibrated = (cv::checkRange(cameraMatrix) && cv::checkRange(distCoeffs));
    // Done for now.
    **/

    return 0;
}

void showField

static double computeReprojectionErrors( const vector<vector<cv::Point3f> >& objectPoints,
    const vector<vector<cv::Point2f> >& imagePoints,
    const vector<cv::Mat>& rvecs,
    const vector<cv::Mat>& tvecs,
    const cv::Mat& cameraMatrix ,
    const cv::Mat& distCoeffs,
    vector<float>& perViewErrors) {
        vector<cv::Point2f> imagePoints2;
        int i, totalPoints = 0;
        double totalErr = 0, err;
        perViewErrors.resize(objectPoints.size());

        for( i = 0; i < (int)objectPoints.size(); ++i )
        {
            projectPoints( cv::Mat(objectPoints[i]), rvecs[i], tvecs[i], cameraMatrix,
            distCoeffs, imagePoints2);
            err = norm(cv::Mat(imagePoints[i]), cv::Mat(imagePoints2), CV_L2);

            int n = (int)objectPoints[i].size();
            perViewErrors[i] = (float) std::sqrt(err*err/n);
            totalErr += err*err;
            totalPoints += n;
        }
    return std::sqrt(totalErr/totalPoints);
}
