#include "Detection.hpp"
#include "Logger.hpp"

using namespace cv;


Detection::Detection() : config(NULL) { /*Should be deleted*/
    Logger::getInstance().log("Detection object created without config!", LogLevel::WARNING);
}

Detection::Detection(ConfigReader &aConfig) : config(aConfig), tessOCR(OCR()), cfgFile(aConfig.getCfgFile()), weightsFile(aConfig.getWeightsFile()), classNamesFile(aConfig.getClassNamesFile()) {
    // Load YOLOv4-tiny model
    try{
        //Print paths:
        Logger::getInstance().log("cfgFile: " + cfgFile, LogLevel::DEBUG);
        Logger::getInstance().log("weightsFile: " + weightsFile, LogLevel::DEBUG);
        Logger::getInstance().log("classNamesFile: " + classNamesFile, LogLevel::DEBUG);

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
        Logger::getInstance().log(e.what(), LogLevel::ERRORLEVEL);
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
                    int centerX = (int)(data[0] * abs(img.cols));
                    int centerY = (int)(data[1] * abs(img.rows));
                    int width = (int)(data[2] * abs(img.cols));
                    int height = (int)(data[3] * abs(img.rows));

                    int left = std::clamp(centerX - width / 2, 0, img.cols -1);
                    int top = std::clamp(centerY - height / 2, 0, img.rows - 1);
                    int right = std::clamp(centerX + width / 2, 0, img.rows - 1);
                    int bottom = std::clamp(centerY + height / 2, 0, img.cols - 1);

                    classIds.push_back(classId);
                    confidences.push_back(confidence);
                    boxes.push_back(cv::Rect(left, top, width, height));

                    // Ensure bounding box dimensions are valid before cropping
                    if (width > 0 && height > 0) {
                        this->cropROI(img, boxes.back());
                    } else {
                        Logger::getInstance().log("Invalid bounding box dimensions!", LogLevel::ERRORLEVEL);
                    }
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

        this->warpImage(cropped, boundedROI);
        cv::imshow("Cropped Image", cropped); // Keep only this display
        // Detect text in the cropped image
        this->tessOCR.extractText(cropped);

        if(this->config.getSaveDetected()){
            saveImg(cropped);
        }

        cv::waitKey(0); // Wait for a key press
    } else {
        Logger::getInstance().log("ROI is out of bounds!", LogLevel::ERRORLEVEL);
    }
}



void Detection::warpImage(cv::Mat& img, cv::Rect& roi) {
    if (img.empty()) {
        Logger::getInstance().log("Empty image passed!", LogLevel::ERRORLEVEL);
        return;
    }

    // Warp the image to a rectangle so the text is straight.
    std::vector<cv::Point2f> roiCorners;
    roiCorners.push_back(cv::Point2f(0, 0));                             // Top-left
    roiCorners.push_back(cv::Point2f(img.cols - 1, 0));                 // Top-right
    roiCorners.push_back(cv::Point2f(0, img.rows - 1));                 // Bottom-left
    roiCorners.push_back(cv::Point2f(img.cols - 1, img.rows - 1));     // Bottom-right

    // Optional: simulate perspective skew correction
    float perspectiveAdjustment = 10.0f;
    roiCorners[0].y += perspectiveAdjustment;
    roiCorners[1].y -= perspectiveAdjustment;
    roiCorners[2].y -= perspectiveAdjustment;
    roiCorners[3].y += perspectiveAdjustment;

    // Destination points are a straight rectangle
    std::vector<cv::Point2f> destinationCorners = {
        {0, 0},
        {(float)img.cols, 0},
        {0, (float)img.rows},
        {(float)img.cols, (float)img.rows}
    };

    cv::Mat matrix = cv::getPerspectiveTransform(roiCorners, destinationCorners);
    cv::warpPerspective(img, img, matrix, img.size());  // ← overwrite input img
}
