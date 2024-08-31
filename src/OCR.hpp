#ifndef OCR_HPP
#define OCR_HPP


#include "Logger.hpp"

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <algorithm>
#include <unordered_set>

class OCR
{
public:
    OCR();
    // OCR(/* args */);
    ~OCR();

    /// @brief Preprocess the image before OCR.
    /// @param img the image to preprocess.
    cv::Mat preProcess(cv::Mat &img);

    /// @brief Extract text from the image.
    /// @param img the image to extract text from.
    /// @return the extracted text as a string.
    std::string extractText(cv::Mat &img);

   private:
    tesseract::TessBaseAPI *ocr;
    std::string filter(std::string &text);
    const std::unordered_set<char> forbidden_chars = {'-', '[', ']', '(', ')', '{', '}'};
};



#endif // OCR_HPP