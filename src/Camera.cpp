#include "Camera.hpp"

Camera::Camera() : config(NULL){
}

Camera::Camera(const ConfigReader &aConfig): config(aConfig), detector(config), cap(getInputSource()) {
    
    this->waitTime = 1000 / config.getFPS();

    if (!cap.isOpened()) {
        Logger::getInstance().log("Error opening video stream or file", LogLevel::ERRORLEVEL);
        throw std::runtime_error("Failed to open video stream or file");
    }
}

Camera::~Camera() {
    if (cap.isOpened()) {
        cap.release(); // Properly release the camera
    }
}

void Camera::detect() {
    if (cap.isOpened()) {
        cv::Mat img = this->capture();
        detector.detect(img);
    } else {
        Logger::getInstance().log("Camera is not opened", LogLevel::ERRORLEVEL);
    }
}

cv::Mat Camera::capture(){
    cap >> this->frame;
    if (this->frame.empty()) {
        Logger::getInstance().log("End of video stream", LogLevel::WARNING);
        throw std::runtime_error("End of video stream - Stopping.");
    }
    return this->frame;
}

/* Should move to GUI class*/
void Camera::show(cv::Mat &frame) {
    if (!frame.empty()) {
        cv::imshow("PlateScanner", frame);
        cv::waitKey(waitTime);
    }
}

cv::VideoCapture Camera::getInputSource() {
    cv::VideoCapture cap;
    switch (this->config.getMedia()) {
    case mediaTypes::IMAGE:
        cap.open(this->config.getPath());
        break;
    case mediaTypes::VIDEO:
        cap.open(this->config.getPath());
        break;
    case mediaTypes::CAMERA:
        cap.open(this->config.getCameraID());
        break;
    default:
        Logger::getInstance().log("Invalid type, defaulting to built-in webcam", LogLevel::ERRORLEVEL);
        cap.open(0); // default to built-in webcam
        break;
    }
    return cap;
}
