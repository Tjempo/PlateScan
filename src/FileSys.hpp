#ifndef FileSys_HPP
#define FileSys_HPP

#include "Logger.hpp"
#include "System.hpp"

extern "C" {
#include <openssl/md5.h>
#include <openssl/evp.h>
}

#include <opencv2/opencv.hpp>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

// ----- File System Functions -----


/// @brief Saves the image to the disk.
/// @param img The image to save.
void saveImg(cv::Mat &img);

/// @brief gets the current date and time.
/// @return The current date and time in the format "YYYY-MM-DD_HH-MM-SS".
std::string getCurrentDateTime();

// Hash functions for naming:
/// @brief Hashes the image using MD5.
/// @param data The image data.
/// @param length The length of the image data.
/// @return The MD5 hash of the image.
std::string hashMD5(const unsigned char* data, size_t length);

#endif  // FileSys_HPP
