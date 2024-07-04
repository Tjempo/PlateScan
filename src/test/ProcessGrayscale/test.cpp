// #include <opencv2/opencv.hpp>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    std::string path = "test.png";

    cv::Mat img = cv::imread(path);
    cv::Mat imgGray;

    cvtColor(img, imgGray, cv::COLOR_BGR2GRAY); //Transform image to grayscale


    cv::imshow("Image",img); //Show normal image
    cv::imshow("Grayscale",imgGray); //Show grayscale image

    cv::waitKey(0);
    return 0;
}

