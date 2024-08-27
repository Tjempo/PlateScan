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

    // Prepare input blob
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

    // Process output
    for (const auto& out : outs) {
        for (int i = 0; i < out.rows; ++i) {
            const float* data = (float*)out.data + i * out.cols;
            float confidence = data[4];
            if (confidence > this->config.getConfThreshold()) {  // Confidence threshold
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
                    // Convert from YOLO format to bounding box
                    int centerX = (int)(data[0] * img.cols);
                    int centerY = (int)(data[1] * img.rows);
                    int width = (int)(data[2] * img.cols);
                    int height = (int)(data[3] * img.rows);
                    int left = centerX - width / 2;
                    int top = centerY - height / 2;

                    cv::Rect box(left, top, width, height);
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
