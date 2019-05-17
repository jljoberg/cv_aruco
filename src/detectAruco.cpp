
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/imgproc.hpp>

#include <algorithm>
#include <vector>
#include <iostream>

#include "detectAruco.h"

#ifdef PRINT_ARUCO
#define dbg_print(...) printf(__VA_ARGS__)
#else
#define dbg_print(...)
#endif
 
void draw_aruco_marker(void)
{
    cv::Mat markerImage;
    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_250);
    cv::aruco::drawMarker(dictionary, 3, 300, markerImage, 1);
    cv::imwrite("/home/jens/aruco_img_2_300.bmp", markerImage);
    cv::namedWindow("aruco", cv::WINDOW_AUTOSIZE|cv::WINDOW_KEEPRATIO);
    cv::imshow("aruco", markerImage);
}

int detect_aruco_marker(cv::Mat img, cv::Point2f* p_detected_center)
{
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(4,4) );
    //cv::morphologyEx(img, img, cv::MORPH_OPEN, kernel);

    std::vector<std::vector<cv::Point2f>> markerCorners, rejectedCandidates;
    cv::Ptr<cv::aruco::DetectorParameters> parameters = cv::aruco::DetectorParameters::create();
    //std::cout << "default : " << parameters->markerBorderBits << std::endl;
    
    parameters->markerBorderBits = 1;
    parameters->maxErroneousBitsInBorderRate = 0.5;
    parameters->errorCorrectionRate = 0.65;
    parameters->minDistanceToBorder = 0;
    parameters->perspectiveRemoveIgnoredMarginPerCell = 0.2;
    parameters->polygonalApproxAccuracyRate = 0.08;     // High ish helps with lens distortion
    
    //parameters->minMarkerPerimeterRate = 0.001;
    //parameters->maxMarkerPerimeterRate = 4;


    cv::Ptr<cv::aruco::Dictionary> dictionary = cv::aruco::getPredefinedDictionary(cv::aruco::DICT_4X4_50);
    std::vector<int> markerIds;

    cv::aruco::detectMarkers(img, dictionary, markerCorners, markerIds, parameters, rejectedCandidates);

    //cv::aruco::detectMarkers(img, dictionary, markerCorners, markerIds);
    cv::aruco::drawDetectedMarkers(img, markerCorners, markerIds);
    if(std::count(markerIds.begin(), markerIds.end(), 3)==1)        // more than one indicates an error
    {
        dbg_print("=== FOUND ID 3 ===\n");
        std::vector<int>::iterator it_id = std::find(markerIds.begin(), markerIds.end(), 3);
        int index_id = std::distance(markerIds.begin(), it_id);
        float avg_x = (markerCorners[index_id][0].x + markerCorners[index_id][1].x + \
                     markerCorners[index_id][2].x + markerCorners[index_id][3].x) / 4;
        float avg_y = (markerCorners[index_id][0].y + markerCorners[index_id][1].y + \
                     markerCorners[index_id][2].y + markerCorners[index_id][3].y) / 4;
        dbg_print("\t x=%f\t y=%f\n", avg_x, avg_y);
        *p_detected_center = cv::Point2f(avg_x, avg_y);
        return 1;
    }
    return 0;

    //std::cout << "Detected " << markerIds.size() << " markers | Rejected " << rejectedCandidates.size() << std::endl;

    //if(markerIds.size()>0) printf("======== IS DETECTION =================\n");
    //if(markerIds.size()==0) printf("no luck\n");
    
}
