#ifndef Detection_HPP
#define Detection_HPP

#include "Logger.hpp"
#include "ConfigReader.hpp"
#include <opencv2/opencv.hpp>

// Include the Darknet header file, we'll be using the Darknet library to detect objects.
extern "C" {
    #include "/home/tjempo/Documents/GIT/darknet/include/darknet.h"
    #include "/home/tjempo/Documents/GIT/darknet/include/yolo_v2_class.hpp"
}

class Detection {
public:
    Detection();
    Detection(ConfigReader &config);
    ~Detection();

    void detect(const std::string &path);
    // void detect(const cv::Mat& img);

private:
    cv::VideoCapture cap; // Assuming you have a VideoCapture object
    ConfigReader config;

    cv::dnn::Net net;
    std::vector<std::string> classNames;


    std::string cfgFile = "USE_CONFIG!";
    std::string weightsFile = "USE_CONFIG!";
    std::string classNamesFile = "USE_CONFIG!";
};


#endif // Detection_HPP