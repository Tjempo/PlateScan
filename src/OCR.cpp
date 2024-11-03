#include "OCR.hpp"


OCR::OCR() {
    this->ocr = new tesseract::TessBaseAPI();
    if (ocr->Init(NULL, "eng")) {
        std::cerr << "Could not initialize tesseract.\n";
        exit(1);
    }
    ocr->SetPageSegMode(tesseract::PSM_SINGLE_LINE);
    ocr->SetVariable("tessedit_char_whitelist", "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-");
}

OCR::~OCR(){
    ocr->End();
    delete ocr;
}


cv::Mat OCR::preProcess(cv::Mat &img) {
    cv::Mat processedImage;

    // Convert to grayscale
    cv::cvtColor(img, processedImage, cv::COLOR_BGR2GRAY);

    // Apply Gaussian blur to reduce noise
    cv::GaussianBlur(processedImage, processedImage, cv::Size(5, 5), 0);

    // Apply simple thresholding
    cv::threshold(processedImage, processedImage, 0, 255, cv::THRESH_BINARY | cv::THRESH_OTSU);

    // Optionally, dilate the image to make characters more prominent
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::dilate(processedImage, processedImage, kernel);
    
    cv::imshow("Preprocessed", processedImage);
    return processedImage;
}


std::string OCR::extractText(cv::Mat &img) {
    std::string extracted_text;

    // Preprocess the image
    cv::Mat processedImage = this->preProcess(img);

    // Optionally, increase DPI by resizing
    double scaleFactor = TARGET_DPI / processedImage.rows;  // Increase DPI to 300 from 162
    cv::resize(processedImage, processedImage, cv::Size(), scaleFactor, scaleFactor, cv::INTER_LINEAR);

    // Set the DPI in Tesseract
    ocr->SetImage(processedImage.data, processedImage.cols, processedImage.rows, processedImage.channels(), processedImage.step1());
    ocr->SetSourceResolution(300);  // Set to 300 DPI for better accuracy


    Logger::getInstance().log("DPI: " + std::to_string(processedImage.rows), LogLevel::DEBUG);

    // Get the OCR result
    char* outText = ocr->GetUTF8Text();

    // Assign the C-string to the std::string
    extracted_text = std::string(outText);
    Logger::getInstance().log("Extracted text: " + extracted_text, LogLevel::INFO);

    // Clean up
    delete[] outText;
    return extracted_text;
}


// Functions for text filtering and license plate extraction:

std::string OCR::remove_dash(std::string &text) {
    text.erase(std::remove_if(text.begin(), text.end(), [this](char c) {
        return forbidden_chars.find(c) != forbidden_chars.end();
    }), text.end());
    Logger::getInstance().log("Filtered text: " + text, LogLevel::DEBUG);
    return text;
}


std::string OCR::find_plate_in_string(const std::string& input) {
    // Iterate over all regex patterns and return the first match
    for (const auto& pattern : this->regex_patterns) {
        std::smatch match;
        if (std::regex_search(input, match, pattern)) {
            return match.str();
        }
    }
    return "";  // Return an empty string if no match is found
}