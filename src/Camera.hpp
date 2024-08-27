#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Logger.hpp"
#include "ConfigReader.hpp"
#include "Detection.hpp"

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

class Camera{
public:
    Camera();
    Camera(const ConfigReader &config);
    ~Camera();

    /// @brief Calibrates the camera using trackbars.
    void calibrate();

    /// @brief Processes the image captured by the camera.
    void detect();

    /// @brief Captures an image from the camera.
    cv::Mat capture();

    /// @brief gets the input source for the camera based on the config file.
    /// @return capture object for the media.
    cv::VideoCapture getInputSource();

    /// @brief Saves the image to the disk. 
    /// @return Name of the saved image, like 2024_29_07_12_42_32. 
    std::string saveImage();

private:
    //Functions:

    /// @brief Shows the image that has been processed in UI window.
    /// @param frame the image to show. Created by the process() function. 
    void show(cv::Mat &frame);

    int calibrateTime = 0;
    int waitTime = 40; //Default is 25 FPS
    cv::Mat frame;

    ConfigReader config;
    Detection detector;
    cv::VideoCapture cap;

};

#endif // CAMERA_HPP
