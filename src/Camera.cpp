#include "Camera.hpp"

Camera::Camera(){
}

Camera::Camera(const ConfigReader &config){
    this->hueMin = config.getHueMin();
    this->satMin = config.getSatMin();
    this->valMin = config.getValMin();

    this->hueMax = config.getHueMax();
    this->satMax = config.getSatMax();
    this->valMax = config.getValMax();

    this->camID = config.getCameraID();
}

Camera::~Camera(){
}


void Camera::calibrate(){
    Logger::getInstance().log("Calibrating the camera...", LogLevel::INFO);
    Logger::getInstance().log("Press 'q' to quit the calibration.", LogLevel::INFO);

    cv::VideoCapture cap(static_cast<int>(this->camID));
    if (!cap.isOpened()){
        Logger::getInstance().log("Error: Could not open the camera.", LogLevel::ERRORLEVEL);
        return;
    }

    cv::Mat frame;
    cv::namedWindow("Calibration", cv::WINDOW_AUTOSIZE);

    cv::createTrackbar("Hue Min", "Calibration", &this->hueMin, 179);
    cv::createTrackbar("Sat Min", "Calibration", &this->satMin, 255);
    cv::createTrackbar("Val Min", "Calibration", &this->valMin, 255);

    cv::createTrackbar("Hue Max", "Calibration", &this->hueMax, 179);
    cv::createTrackbar("Sat Max", "Calibration", &this->satMax, 255);
    cv::createTrackbar("Val Max", "Calibration", &this->valMax, 255);

    while(true){
        cap >> frame;
        if (frame.empty()){
            Logger::getInstance().log("Could not capture the frame.", LogLevel::ERRORLEVEL);
            break;
        }

        cv::Mat hsv;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        cv::Mat mask;
        cv::inRange(hsv, cv::Scalar(this->hueMin, this->satMin, this->valMin), cv::Scalar(this->hueMax, this->satMax, this->valMax), mask);

        cv::imshow("Calibration", mask);

        //TODO: This does not work right at the moment 
        if (cv::waitKey(1) == 'q'){
            Logger::getInstance().log("Calibration finished.", LogLevel::INFO);
            break;
        }
    }

    cv::destroyWindow("Calibration");
    cap.release();
}