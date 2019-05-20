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
#include "udp/udp.h"

#include <chrono>

namespace chr = std::chrono;

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

int cam_step(Withrobot::Camera* p_camera, cv::Mat *p_cam_img, Withrobot::camera_format *p_cam_format, bool calibrate, cv::Point2f* p_target_center)
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
        cv::Scalar mean;
        mean = cv::mean(*p_cam_img, mask);

        if(mean[0] < desired_meanPx_range[0]) cam_exposure_inc(p_camera, 1);
        if(mean[0] > desired_meanPx_range[1]) cam_exposure_inc(p_camera, -1);

        // return out_size
    }   // end if(calibrate)
    
#ifdef TIME_ARUCO
    static int i=0;
    static chr::milliseconds t_storage = chr::milliseconds(0);
    chr::time_point<chr::high_resolution_clock> t0, t;
    t0 = chr::high_resolution_clock::now();
#endif
    // cv::Point2f target_center;
    if(detect_aruco_marker(*p_cam_img, p_target_center) != 1) return out_size;
    cv::circle(*p_cam_img, *p_target_center, 20, cv::Scalar(255), -1, 8);
    
#ifdef TIME_ARUCO
    t = chr::high_resolution_clock::now();
    t_storage += ( chr::duration_cast<chr::milliseconds>(t-t0) );
    i++;
    if(i>=20)
    {
        std::cout << "Avg time spent detecting: " << t_storage.count()/i << std::endl;
        t_storage = chr::milliseconds(0);
        i = 0;
    }
#endif
    return out_size;
}

//#define BUILD_CAM_MAIN
#ifdef BUILD_CAM_MAIN
int main(int argc, char* argv[])
{
    udp_init();
    std::ios::sync_with_stdio(false);

    const char* vid_io_file = "/dev/video0";
    
    Withrobot::Camera* p_camera;
    p_camera = new Withrobot::Camera(vid_io_file);

    // TODO: Enable selection of different cameras
    p_camera->set_format(1280, 960, Withrobot::fourcc_to_pixformat('G','R','E','Y'), 1, 30);
    p_camera->set_control("Brightness", 32);

    // Get image size
    Withrobot::camera_format cam_format;
    p_camera->get_current_format(cam_format);
    cv::Size cam_size(cam_format.width, cam_format.height);

    // Initialize image storage 
    cv::Mat cam_img(cam_size, CV_8UC1);

    if (!p_camera->start()) {
    	perror("Failed to start.");
    	exit(0);
    }

    bool quit = false;
    int i = 0;
    while(!quit)
    {
#ifdef CAM_LOOP_PRINT // Setup start time
        static int print_i=0;
        static chr::milliseconds t_storage = chr::milliseconds(0);
        chr::time_point<chr::high_resolution_clock> t0, t;
        t0 = chr::high_resolution_clock::now();
#endif
        if( cam_step(p_camera, &cam_img, &cam_format, 1) < 0 ) continue;
#ifdef CAM_LOOP_PRINT // Set end time and print
        t = chr::high_resolution_clock::now();
        t_storage += ( chr::duration_cast<chr::milliseconds>(t-t0) );
        print_i++;
        if(print_i>=20)
        {
            std::cout << "Avg time each frame: " << t_storage.count()/print_i << std::endl;
            t_storage = chr::milliseconds(0);
            print_i = 0;
        }
#endif
        i++;
        if(!(i%10)){
            udp_bc((char*)cam_img.data, cam_format.image_size);
        }
        
    }
    delete p_camera;
    return 1;
}
#endif