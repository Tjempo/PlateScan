#include "Camera.hpp"

Camera::Camera(): config(NULL){
}

Camera::Camera(const ConfigReader &config) : config(config){
    this->hueMin = config.getHueMin();
    this->satMin = config.getSatMin();
    this->valMin = config.getValMin();
    this->hueMax = config.getHueMax();
    this->satMax = config.getSatMax();
    this->valMax = config.getValMax();
    this->waitTime = 1000 / config.getFPS();
}

Camera::~Camera(){
}


void Camera::calibrate(){
    Logger::getInstance().log("Calibrating the camera...", LogLevel::INFO);
    Logger::getInstance().log("Press 'q' to quit the calibration.", LogLevel::INFO);

    cv::Mat frame;
    cv::VideoCapture cap;
    cap = getInputSource(cap);

    if (!cap.isOpened()){
        Logger::getInstance().log("Could not open stream.", LogLevel::ERRORLEVEL);
        return;
    }

    cv::namedWindow("Calibration", cv::WINDOW_AUTOSIZE);

    cv::createTrackbar("Hue Min", "Calibration", &this->hueMin, 179);
    cv::createTrackbar("Sat Min", "Calibration", &this->satMin, 255);
    cv::createTrackbar("Val Min", "Calibration", &this->valMin, 255);

    cv::createTrackbar("Hue Max", "Calibration", &this->hueMax, 179);
    cv::createTrackbar("Sat Max", "Calibration", &this->satMax, 255);
    cv::createTrackbar("Val Max", "Calibration", &this->valMax, 255);

    if(this->config.getMedia() == mediaTypes::IMAGE){
        cap >> frame;
        if (frame.empty()){
            Logger::getInstance().log("Could not capture the frame.", LogLevel::ERRORLEVEL);
            return;
        }
    }

    while(true){
        if(this->config.getMedia() != mediaTypes::IMAGE){
            cap >> frame;
            if (frame.empty()){
                Logger::getInstance().log("Could not capture the frame.", LogLevel::ERRORLEVEL);
                break;
            }
        }

        // Resize the frame:
        cv::resize(frame, frame, cv::Size(this->config.getDesiredWidth(), this->config.getDesiredHeight()));

        cv::Mat hsv;
        cv::cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

        cv::Mat mask;
        cv::inRange(hsv, cv::Scalar(this->hueMin, this->satMin, this->valMin), cv::Scalar(this->hueMax, this->satMax, this->valMax), mask);

        cv::imshow("Calibration", mask);

        // Check for 'q' key press
        int key = cv::waitKey(40);
        if (key == 113){ // ASCII code for 'q'
            Logger::getInstance().log("Calibration finished.", LogLevel::INFO);
            break;
        }
    }

    cv::destroyAllWindows();
    cap.release();

    // Update the config file with the new values:
    setConfigValues(this->hueMin, this->satMin, this->valMin, this->hueMax, this->satMax, this->valMax);
}


void Camera::process(){

}


void Camera::detect(cv::Mat &frame){

    if(this->config.getShowUI()){
        this->show(frame);
    }
}

void Camera::show(cv::Mat &frame){
    cv::imshow("PlateScanner", frame);
    cv::waitKey(waitTime); 
}


cv::VideoCapture Camera::getInputSource(cv::VideoCapture &cap){

    switch(this->config.getMedia()){
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


void Camera::setConfigValues(int hueMin, int satMin, int valMin, int hueMax, int satMax, int valMax){
    this->hueMin = hueMin;
    this->satMin = satMin;
    this->valMin = valMin;
    this->hueMax = hueMax;
    this->satMax = satMax;
    this->valMax = valMax;
}