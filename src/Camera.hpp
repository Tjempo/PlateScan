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

    /// @brief Captures an image from the camera.
    void capture();

    /// @brief Saves the image to the disk. 
    /// @return Name of the saved image, like 2024_29_07_12_42_32. 
    std::string saveImage();

private:
    int hueMin = 0; 
    int satMin = 0; 
    int valMin = 0;

    int hueMax = 179; 
    int satMax = 255; 
    int valMax = 255;

    int camID = 0;

};

#endif // CAMERA_HPP
