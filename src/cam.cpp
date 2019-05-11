#include <stdio.h>
#include <errno.h>
#include <iostream>

#include <unistd.h> // usleep/
#include <ctime>    // clock

//#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <opencv2/imgproc.hpp>

#include "detectAruco.h"
#include "withrobot/withrobot_camera.hpp"	

//using namespace cv;

int cam_exposure_inc(Withrobot::Camera* p_cam, int increment)
{
    //int brgt = p_cam->get_control("Brightness");
    // original p_cam->set_control("Brightness", 64);
    //p_cam->set_control("Brightness", 32);
    
    int exposure = p_cam->get_control("Exposure (Absolute)");
    exposure += increment;
    p_cam->set_control("Exposure (Absolute)", exposure);
    return 0;
}

int cam_step(Withrobot::Camera* p_camera, cv::Mat *p_cam_img, Withrobot::camera_format *p_cam_format, bool calibrate)
{
    int desired_meanPx_range[2] = {100, 140};
    int out_size = p_camera->get_frame(p_cam_img->data, p_cam_format->image_size, 1);
    if(out_size == -1)
    {
        std::cout << "step fail\n";
        p_camera->stop();
        p_camera->start();
        return -1;
    }
    if(calibrate)
    {
        cv::Mat mask(p_cam_img->size(), CV_8UC1);
        mask.setTo(cv::Scalar(255));
        cv::Scalar mean, std_dev;
        // cv::meanStdDev(*p_cam_img, mean, std_dev, *p_cam_img);
        mean = cv::mean(*p_cam_img, mask);
        printf("Mean is %f\n", mean[0]);

        if(mean[0] < desired_meanPx_range[0]) cam_exposure_inc(p_camera, 1);
        if(mean[0] > desired_meanPx_range[1]) cam_exposure_inc(p_camera, -1);

        //return 0;
    }
    detect_aruco_marker(*p_cam_img);
    return 0;
}

int main(int argc, char* argv[])
{
    //draw_aruco_marker();
    std::ios::sync_with_stdio(false);

    const char* vid_io_file = "/dev/video0";
    Withrobot::Camera camera(vid_io_file);

    // TODO: Enable selection of different cameras
    camera.set_format(1280, 960, Withrobot::fourcc_to_pixformat('G','R','E','Y'), 1, 30);
    // camera.set_control("Exposure, Auto", 0x3);
    camera.set_control("Brightness", 32);

    // Get image size
    Withrobot::camera_format cam_format;
    camera.get_current_format(cam_format);
    cv::Size cam_size(cam_format.width, cam_format.height);

    const char* window_name = "Display oCam";
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE|cv::WINDOW_KEEPRATIO);

    // Initialize image storage 
    cv::Mat cam_img(cam_size, CV_8UC1);

    if (!camera.start()) {
    	perror("Failed to start.");
    	exit(0);
    }

    bool quit = false;
    while(!quit)
    {
        clock_t t0 = clock();
    #define USE_FUNCTION
    #ifdef USE_FUNCTION
        if( cam_step(&camera, &cam_img, &cam_format, 1) < 0 ) continue;
    #else
        int out_size = camera.get_frame(cam_img.data, cam_format.image_size, 1);
        if(out_size == -1)
        {
            std::cout << "step fail\n";
            camera.stop();
            camera.start();
            continue;
        }
        detect_aruco_marker(cam_img);
    #endif
        //std::cout << "Time spent: " << (float)(clock()-t0)/CLOCKS_PER_SEC << std::endl;
        
        cv::imshow(window_name, cam_img);
        char key = cv::waitKey(10);

        if(key=='q' || key=='Q') quit = true;
    }
}