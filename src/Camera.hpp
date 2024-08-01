#ifndef CAMERA_HPP
#define CAMERA_HPP

#include "Logger.hpp"
#include "ConfigReader.hpp"

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
    void process();

    /// @brief Detects the object in the image.
    /// @param frame the image to detect the object in. Created by the process() function.
    void detect(cv::Mat &frame);

    /// @brief Captures an image from the camera.
    void capture();

    /// @brief gets the input source for the camera based on the config file.
    /// @return capture object for the media.
    cv::VideoCapture getInputSource(cv::VideoCapture &cap);

    /// @brief Saves the image to the disk. 
    /// @return Name of the saved image, like 2024_29_07_12_42_32. 
    std::string saveImage();

private:
    //Functions:

    /// @brief Shows the image that has been processed in UI window.
    /// @param frame the image to show. Created by the process() function. 
    void show(cv::Mat &frame);

    /// @brief sets the values of the trackbars as the config values.
    void setConfigValues(int hueMin, int satMin, int valMin, int hueMax, int satMax, int valMax);

    //Values:
    int hueMin = 0; 
    int satMin = 0; 
    int valMin = 0;

    int hueMax = 179; 
    int satMax = 255; 
    int valMax = 255;

    int calibrateTime = 0;
    int waitTime = 40; //Default is 25 FPS

    ConfigReader config;
};

#endif // CAMERA_HPP
