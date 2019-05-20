
#ifndef OCAM_CAM_H
#define OCAM_CAM_H

#include <opencv2/core.hpp>
#include "withrobot/withrobot_camera.hpp"


int cam_exposure_inc(Withrobot::Camera* p_cam, int increment);
int cam_step(Withrobot::Camera* p_camera, cv::Mat *p_cam_img, Withrobot::camera_format *p_cam_format, bool calibrate, cv::Point2f* p_target_center);

#endif