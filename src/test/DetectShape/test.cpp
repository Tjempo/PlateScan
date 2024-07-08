#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>


// Constants
const unsigned short OUTPUT_WIDTH = 1280, OUTPUT_HEIGHT = 720;

// Trackbar values
int t1 = 79, t2 = 119;

// Callback functions for trackbars (no need to do anything here)
void onTrackbarChange(int, void*) {}

int main(int argc, char const *argv[]) {
    std::string path = "shapes.jpg"; // Original size: 3.035 × 2.199
    // std::string path = "shapes_dig.jpg"; // Original size: 962 × 864


    cv::Mat img = cv::imread(path), gray, blur, canny, dilated;

    /* For webcam:
    cv::Mat img, gray, blur, canny, dilated;
    cv::VideoCapture cap(0);
    cap.read(img);
    */

    if (img.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }
    
    // Scale down the image
    cv::resize(img, img, cv::Size(OUTPUT_WIDTH, OUTPUT_HEIGHT));

    // Create trackbars in "Trackbars" window
    cv::namedWindow("Trackbars", cv::WINDOW_AUTOSIZE); // Create a window called "Control"
    cv::createTrackbar("Threshold1", "Trackbars", &t1, 250, onTrackbarChange);
    cv::createTrackbar("Threshold2", "Trackbars", &t2, 250, onTrackbarChange);
    //Im not using the nullptr because i dont want to have to set the values everytime i launch the program

    while (1) {
        // cap.read(img);

        // Perform image processing
        cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY); // Convert to gray
        cv::GaussianBlur(gray, blur, cv::Size(5, 5), 3, 0); // Blur image
        cv::Canny(blur, canny, t1, t2); // Detect edges
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
        cv::dilate(canny, dilated, kernel); // Dilate edges


        // Display images:
        // cv::imshow("Original", img); // Show normal image
        // cv::imshow("Gray", gray); // Show gray image
        // cv::imshow("Blur", blur); // Show blurred image
        // cv::imshow("Canny", canny); // Show canny image
        cv::imshow("Dilate", dilated); // Show dilated image
        cv::waitKey(20);
    }

    return 0;
}