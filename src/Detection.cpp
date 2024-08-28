#include "Detection.hpp"

using namespace cv;


Detection::Detection() : config(NULL) { /*Should be deleted*/
    Logger::getInstance().log("Detection object created without config!", LogLevel::WARNING);
}

Detection::Detection(ConfigReader &aConfig) : config(aConfig), cfgFile(aConfig.getCfgFile()), weightsFile(aConfig.getWeightsFile()), classNamesFile(aConfig.getClassNamesFile()) {
    // Load YOLOv4-tiny model
    try{
        net = cv::dnn::readNet(weightsFile, cfgFile);
        net.setPreferableBackend(cv::dnn::DNN_BACKEND_OPENCV);
        net.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    }
    catch(const std::exception& e){
        Logger::getInstance().log(e.what(), LogLevel::ERRORLEVEL);
    }

    this->confThreshold = aConfig.getConfThreshold();

    // Load class names
    std::ifstream classNamesStream(classNamesFile);
    std::string line;
    while (std::getline(classNamesStream, line)) {
        classNames.push_back(line);
    }
}

Detection::~Detection(){}

void Detection::detect(cv::Mat& img) {
    if (img.empty()) {
        Logger::getInstance().log("Empty image passed!", LogLevel::ERRORLEVEL);
        return;
    }

    cv::Mat blob;
    cv::dnn::blobFromImage(img, blob, 1.0 / 255.0, cv::Size(416, 416), cv::Scalar(0, 0, 0), true, false);
    std::vector<cv::Mat> outs;

    try {
        net.setInput(blob);
        std::vector<std::string> outNames = net.getUnconnectedOutLayersNames();
        net.forward(outs, outNames);
    } catch (const cv::Exception& e) {
        Logger::getInstance().log(e.what(), LogLevel::ERRORLEVEL);
        return;
    }

    // Draw bounding boxes
    this->drawBoundingBox(img, outs, classNames);

    /* // Show results 
    cv::resize(img, img, cv::Size(1900, 1000));
    cv::imshow("Detection", img);
    cv::waitKey(0);
    */
}

void Detection::drawBoundingBox(cv::Mat &img, std::vector<cv::Mat> outs, std::vector<std::string> classNames) {
    //TODO use the inverse of the pixles behind the bounding box to draw the box in a different color.
    // Process output - kinda ugly.
    for (const auto& out : outs) {
        for (int i = 0; i < out.rows; ++i) {
            const float* data = (float*)out.data + i * out.cols;
            float confidence = data[4];
            if (confidence > this->confThreshold) {
                // Object detected, process detection
                int classId = -1;
                float maxClassScore = -1;
                for (int j = 5; j < out.cols; ++j) {
                    if (data[j] > maxClassScore) {
                        maxClassScore = data[j];
                        classId = j - 5;
                    }
                }
                if (classId >= 0) {
                    // Convert to bounding box
                    int centerX = (int)(data[0] * img.cols);
                    int centerY = (int)(data[1] * img.rows);
                    int width = (int)(data[2] * img.cols);
                    int height = (int)(data[3] * img.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    cv::Rect box(left, top, width, height);
                    cv::Rect crop(left - 10, top - 10, (width + 20), (height + 20));

                    this->cropROI(img, crop);
                    outs.clear(); //Clear the outs vector to prevent multiple detections.

                    cv::rectangle(img, box, cv::Scalar(190, 95, 0), 2);
                    std::string label = classNames[classId] + ": " + cv::format("%.2f", confidence);
                    cv::putText(img, label, cv::Point(left, top - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
                }
            }
        }
    }

    // Show results
    cv::resize(img, img, cv::Size(1900, 1000));
    cv::imshow("Detection", img);
    cv::waitKey(0);

}

void Detection::cropROI(cv::Mat &img, cv::Rect& roi) {
    if (img.empty()) {
        Logger::getInstance().log("Empty image passed!", LogLevel::ERRORLEVEL);
        return;
    }

    if (roi.width <= 0 || roi.height <= 0) {
        Logger::getInstance().log("Invalid ROI passed!", LogLevel::ERRORLEVEL);
        return;
    }

    cv::Mat cropped = img(roi);

    if(this->config.getSaveDetected()){
        saveImg(cropped);
    }


    cv::imshow("ROI", cropped);
    cv::waitKey(0);
}