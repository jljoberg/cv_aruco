
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include "udp/udp.h"

int main()
{

    cv::Mat rcv_img(cv::Size(1280, 960), CV_8UC1);
    udp_init(1);
    
    const char* window_name = "Display oCam";
    cv::namedWindow(window_name, cv::WINDOW_AUTOSIZE|cv::WINDOW_KEEPRATIO);

    bool quit=false;
    while(!quit)
    {
        udp_rcv_bc((char*)rcv_img.data, 1280*960);
        cv::imshow(window_name, rcv_img);
        char key = cv::waitKey(10);

        if(key=='q' || key=='Q') quit = true;
    }
}
