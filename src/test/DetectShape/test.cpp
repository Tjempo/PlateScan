#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

// Function definitions:
void getContours(cv::Mat imgDil, cv::Mat img);

// Constants
const unsigned short OUTPUT_WIDTH = 1280, OUTPUT_HEIGHT = 720;

// Trackbar values
int t1 = 78, t2 = 105;

// Callback functions for trackbars (no need to do anything here)
void onTrackbarChange(int, void *) {}

int main(int argc, char const *argv[]) {
    std::string path = "shapes.jpg"; // Original size: 3.035 × 2.199
    // std::string path = "shapes_dig.jpg";  // Original size: 962 × 864

    cv::Mat img = cv::imread(path), gray, blur, canny, dilated;
    if (img.empty()) {
        std::cerr << "Could not open or find the image!" << std::endl;
        return -1;
    }

    // Scale the image
    cv::resize(img, img, cv::Size(OUTPUT_WIDTH, OUTPUT_HEIGHT));

    // Perform image processing
    cv::cvtColor(img, gray, cv::COLOR_BGR2GRAY);         // Convert to gray
    cv::GaussianBlur(gray, blur, cv::Size(5, 5), 3, 0);  // Blur image
    cv::Canny(blur, canny, t1, t2);                      // Detect edges
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
    cv::dilate(canny, dilated, kernel);  // Dilate edges

    // Get contours
    getContours(dilated, img);

    // Display images:
    cv::imshow("Original", img);  // Show normal image
    cv::waitKey(0);

    return 0;
}

void getContours(cv::Mat imgDil, cv::Mat img) {
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    // Documentation for findContours: https://docs.opencv.org/4.x/d3/dc0/group__imgproc__shape.html#gadf1ad6a0b82947fa1fe3c3d497f260e0
    cv::findContours(imgDil, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    //Define a vector of vectors to store the points of the polygon:
    std::vector<std::vector<cv::Point>> contoursPoly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());

    for(unsigned short i = 0; i < contours.size(); i++){
        unsigned long area = cv::contourArea(contours[i]);

        if(area > 1000){
            float perimeter = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], contoursPoly[i], 0.016 * perimeter, true);
            // Documentation for drawContours: https://docs.opencv.org/4.x/d6/d6e/group__imgproc__draw.html#ga746c0625f1781f1ffc90
            // cv::drawContours(img, contours, i, cv::Scalar(255, 0, 255), 2);
            cv::drawContours(img, contoursPoly, i, cv::Scalar(255, 0, 255), 2);  // Draw contours on the image
            boundRect[i] = cv::boundingRect(contoursPoly[i]);
            // cv::rectangle(img, boundRect[i].tl(), boundRect[i].br(), cv::Scalar(0, 255, 0), 2);

            unsigned short vertices = (unsigned short)contoursPoly[i].size();
            float aspectRatio = (float)boundRect[i].width / (float)boundRect[i].height;
            switch (vertices) {  // This is a lazy switch case. Its better to use a function to determine the shape.
                case 2:
                    cv::putText(img, "Line", contours[i][0], cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
                    break;
                case 3:
                    cv::putText(img, "Triangle", (contours[i][0]), cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
                    break;

                case 4:
                    if (aspectRatio > 0.75 && aspectRatio < 1.3) {
                        cv::putText(img, "Square", contours[i][0], cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
                    } else {
                        cv::putText(img, "Rectangle", contours[i][0], cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
                    }
                    break;
                case 5:
                    cv::putText(img, "Half circle", contours[i][0], cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
                    break;

                case 6:
                    cv::putText(img, "Pentagon", contours[i][0], cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
                    break;

                case 7:
                    cv::putText(img, "Heptagon", contours[i][0], cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
                    break;

                default:
                    cv::putText(img, "Circle", contours[i][0], cv::FONT_HERSHEY_COMPLEX, 0.5, cv::Scalar(0, 0, 0), 1);
                    break;
            }
        }
    }
}