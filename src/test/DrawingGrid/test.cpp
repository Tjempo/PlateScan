// #include <opencv2/opencv.hpp>

#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char const *argv[]) {
    // Create a blank image:
    cv::Mat img(512, 512, CV_8UC3, cv::Scalar(255, 255, 255));  // 512 x 512 image with 3 channels white background
    // Scalar is the color of the image, The color is in BGR format (Blue, Green, Red)

    // Draw a grid on the image
    for (unsigned short x = 0; x < img.rows; x += 51) {  // 52 Would be logical but looks weird so 51 is used to make the grid more visible
        for (int i = 0; i < img.cols; i += 51) {
            cv::line(img, cv::Point(0, x), cv::Point(img.cols, x), cv::Scalar(0, 0, 0), 2);  // Draw horizontal lines
            cv::line(img, cv::Point(i, 0), cv::Point(i, img.rows), cv::Scalar(0, 0, 0), 2);  // Draw vertical lines

            // Draw checkerboard pattern
            if ((x / 51) % 2 == 0) {
                if ((i / 51) % 2 == 0) {
                    if (i + 51 <= img.cols && x + 51 <= img.rows) {
                        cv::rectangle(img, cv::Point(i, x), cv::Point(i + 51, x + 51), cv::Scalar(0, 0, 0), -1);
                    }
                }
            } else if ((i / 51) % 2 != 0) {
                if (i + 51 <= img.cols && x + 51 <= img.rows) {
                    cv::rectangle(img, cv::Point(i, x), cv::Point(i + 51, x + 51), cv::Scalar(0, 0, 0), -1);
                }
            }
        }
    }

    cv::imshow("Checkerboard", img);  // Show normal image
    cv::waitKey(0);
    return 0;
}