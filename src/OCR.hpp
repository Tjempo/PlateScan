#ifndef OCR_HPP
#define OCR_HPP


#include "Logger.hpp"

#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <algorithm>
#include <unordered_set>
#include <regex>

#define TARGET_DPI 300

class OCR {
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
    const std::unordered_set<char> forbidden_chars = {'-'};


    /// @brief This function deskews the image.
    /// @param img the image to deskew.
    cv::Mat deskew(cv::Mat &img);

    /// @brief This function filters out forbidden characters from the input text. (Prepares it for database queries.)
    /// @param text the input string to filter.
    /// @return The input string with forbidden characters removed.
    std::string remove_dash(std::string &text);


    /// @brief This function finds a license plate in a string matching to a regex.
    /// @param input the input string to search for a license plate. Should contain dashes, So like "XX-XX-99". It may contain characters before and after the license plate.
    /// @return Returns a string containing the license plate if found, otherwise an empty string.
    std::string find_plate_in_string(const std::string &input);


    // Define the list of regex patterns
    std::vector<std::regex> regex_patterns = {
        std::regex(R"(([a-zA-Z]{2})-([0-9]{2})-([0-9]{2}))"),    // [1]  => XX-99-99 #from 1951
        std::regex(R"(([0-9]{2})-([0-9]{2})-([a-zA-Z]{2}))"),    // [2]  => 99-99-XX #from 1965
        std::regex(R"(([0-9]{2})-([a-zA-Z]{2})-([0-9]{2}))"),    // [3]  => 99-XX-99 #from 1973
        std::regex(R"(([a-zA-Z]{2})-([0-9]{2})-([a-zA-Z]{2}))"), // [4]  => XX-99-XX #from 1978
        std::regex(R"(([a-zA-Z]{2})-([a-zA-Z]{2})-([0-9]{2}))"), // [5]  => XX-XX-99 #from 1991
        std::regex(R"(([0-9]{2})-([a-zA-Z]{2})-([a-zA-Z]{2}))"), // [6]  => 99-XX-XX #from 1999
        std::regex(R"(([0-9]{2})-([a-zA-Z]{3})-([0-9]))"),       // [7]  => 99-XXX-9 #from 2008
        std::regex(R"(([0-9])-([a-zA-Z]{3})-([0-9]{2}))"),       // [8]  => 9-XXX-99 #from 2013
        std::regex(R"(([a-zA-Z]{2})-([0-9]{3})-([a-zA-Z]))"),    // [9]  => XX-999-X #from 2015
        std::regex(R"(([a-zA-Z])-([0-9]{3})-([a-zA-Z]{2}))"),    // [10] => X-999-XX #from 2019
        std::regex(R"(([a-zA-Z]{3})-([0-9]{2})-([a-zA-Z]))"),    // [11] => XXX-99-X #from 2024
        std::regex(R"(([0-9])-([a-zA-Z]{2})-([0-9]{3}))"),       // [12] => 9-XX-999 #(Currently only company vehicles)
        std::regex(R"(([0-9]{3})-([a-zA-Z]{2})-([0-9]))"),       // [13] => 999-XX-9 #Does not exist yet
        std::regex(R"(([a-zA-Z])-(\d{2})-([a-zA-Z]{3}))")        // [14] => X-99-XXX #Does not exist yet
    };
};

#endif // OCR_HPP