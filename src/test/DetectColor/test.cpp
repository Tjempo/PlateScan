#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

int hMin = 0, sMin = 0, vMin = 0;
int hMax = 179, sMax = 255, vMax = 255;

const unsigned short OUTPUT_WIDTH = 240, OUTPUT_HEIGHT = 320;

// Callback functions for trackbars (no need to do anything here)
void onTrackbarChange(int, void*) {}

int main(int argc, char const *argv[]) {
    std::string path = "colors.jpg"; // Original size: 2.024 × 2.823
    cv::Mat img = cv::imread(path), imgHSV, mask;
    cv::resize(img, img, cv::Size(OUTPUT_WIDTH, OUTPUT_HEIGHT));
    cv::cvtColor(img, imgHSV, cv::COLOR_BGR2HSV);

    // Create trackbars in "Trackbars" window
    cv::namedWindow("Trackbars", cv::WINDOW_AUTOSIZE); // Create a window called "Control"
    cv::createTrackbar("Hue Min", "Trackbars", nullptr, 179, onTrackbarChange);
    cv::createTrackbar("Sat Min", "Trackbars", nullptr, 255, onTrackbarChange);
    cv::createTrackbar("Val Min", "Trackbars", nullptr, 255, onTrackbarChange);
    cv::createTrackbar("Hue Max", "Trackbars", nullptr, 179, onTrackbarChange);
    cv::createTrackbar("Sat Max", "Trackbars", nullptr, 255, onTrackbarChange);
    cv::createTrackbar("Val Max", "Trackbars", nullptr, 255, onTrackbarChange);

    while (1) {
        // Get the trackbar values
        hMin = cv::getTrackbarPos("Hue Min", "Trackbars");
        sMin = cv::getTrackbarPos("Sat Min", "Trackbars");
        vMin = cv::getTrackbarPos("Val Min", "Trackbars");
        hMax = cv::getTrackbarPos("Hue Max", "Trackbars");
        sMax = cv::getTrackbarPos("Sat Max", "Trackbars");
        vMax = cv::getTrackbarPos("Val Max", "Trackbars");

        cv::Scalar lower(hMin, sMin, vMin);
        cv::Scalar upper(hMax, sMax, vMax);
        cv::inRange(imgHSV, lower, upper, mask);

        cv::imshow("Original", img); // Show normal image
        cv::imshow("Mask", mask); // Show mask
        cv::waitKey(20);
    }
    return 0;
}
