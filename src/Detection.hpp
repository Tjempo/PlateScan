#ifndef Detection_HPP
#define Detection_HPP

#include "Logger.hpp"
#include "ConfigReader.hpp"
#include "FileSys.hpp"
#include <opencv2/opencv.hpp>

// Include the Darknet header file, we'll be using the Darknet library to detect objects.
extern "C"{
    #include "../../darknet/include/darknet.h"
    #include "../../darknet/include/yolo_v2_class.hpp"
}

class Detection {
public:
    Detection();
    Detection(ConfigReader &config);
    ~Detection();

    void detect(cv::Mat &img); //Not by const, because we modify the original image.
    void drawBoundingBox(cv::Mat &img, std::vector<cv::Mat> outs, std::vector<std::string> classNames);
    void cropROI(cv::Mat &img, cv::Rect &roi);

   private:
    cv::VideoCapture cap;
    ConfigReader config;

    cv::dnn::Net net;
    std::vector<std::string> classNames;


    std::string cfgFile = "USE_CONFIG!";
    std::string weightsFile = "USE_CONFIG!";
    std::string classNamesFile = "USE_CONFIG!";

    float confThreshold = 0.5; //Default value is 50%
};


#endif // Detection_HPP