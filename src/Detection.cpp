#include "Detection.hpp"

using namespace cv;


Detection::Detection() : config(NULL) { /*Should be deleted*/
    Logger::getInstance().log("Detection object created without config!", LogLevel::WARNING);
}

Detection::Detection(ConfigReader &aConfig) : config(aConfig), tessOCR(OCR()), cfgFile(aConfig.getCfgFile()), weightsFile(aConfig.getWeightsFile()), classNamesFile(aConfig.getClassNamesFile()) {
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
    // this->nmsThreshold = aConfig.getNmsThreshold();

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
    try{
        this->drawBoundingBox(img, outs, classNames);
    } catch (const cv::Exception& e) {
        Logger::getInstance().log("Bounding box creation failed", LogLevel::ERRORLEVEL);
    }
}

void Detection::drawBoundingBox(cv::Mat &img, std::vector<cv::Mat> outs, std::vector<std::string> classNames) {
    std::vector<int> classIds;
    std::vector<float> confidences;
    std::vector<cv::Rect> boxes;

    // Iterate over all detections
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

                    classIds.push_back(classId);
                    confidences.push_back(confidence);
                    boxes.push_back(cv::Rect(left, top, width, height));

                    this->cropROI(img, boxes.back());
                }
            }
        }
    }

    // Apply Non-Maximum Suppression to remove overlapping boxes
    std::vector<int> indices;
    cv::dnn::NMSBoxes(boxes, confidences, this->confThreshold, this->nmsThreshold, indices);

    // Count the number of detected objects:
    Logger::getInstance().log("Detected " + std::to_string(indices.size()) + " objects", LogLevel::INFO);

    for (int idx : indices) {
        cv::Rect box = boxes[idx];
        cv::rectangle(img, box, cv::Scalar(190, 95, 0), 2);
        std::string label = classNames[classIds[idx]] + ": " + cv::format("%.2f", confidences[idx]);
        cv::putText(img, label, cv::Point(box.x, box.y - 10), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0), 2);
    }

    // Display the image with all detected bounding boxes
    cv::resize(img, img, cv::Size(1900, 1000));
    cv::imshow("Detected", img);
    cv::waitKey(0);
}


void Detection::cropROI(cv::Mat &img, cv::Rect& roi) {
    if (img.empty()) {
        Logger::getInstance().log("Empty image passed!", LogLevel::ERRORLEVEL);
        return;
    }

    // Ensure ROI is within image bounds
    int x = std::max(0, roi.x);
    int y = std::max(0, roi.y);
    int width = std::min(roi.width, img.cols - x);
    int height = std::min(roi.height, img.rows - y);

    // Only process if the width and height are valid
    if (width > 0 && height > 0) {
        cv::Rect boundedROI(x, y, width, height);
        cv::Mat cropped = img(boundedROI);

        // Detect text in the cropped image
        this->tessOCR.extractText(cropped);

        if(this->config.getSaveDetected()){
            saveImg(cropped);
        }

        cv::imshow("ROI", cropped);
        cv::waitKey(0);
    } else {
        Logger::getInstance().log("ROI is out of bounds!", LogLevel::ERRORLEVEL);
    }
}
