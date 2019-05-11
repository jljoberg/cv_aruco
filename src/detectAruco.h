
#ifndef DETECT_ARUCO_H
#define DETECT_ARUCO_H

#include <opencv2/core.hpp>
#include <opencv2/aruco.hpp>

void draw_aruco_marker(void);

void detect_aruco_marker(cv::Mat img);

#endif