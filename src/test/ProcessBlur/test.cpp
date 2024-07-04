// #include <opencv2/opencv.hpp>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    std::string path = "test.png";

    cv::Mat img = cv::imread(path);
    cv::Mat imgBlur;
    cv::Mat imgCanny;

    cv::GaussianBlur(img, imgBlur, cv::Size(5,5), 5,0);


    cv::imshow("Image",img); //Show normal image
    cv::imshow("Blur", imgBlur); //Show blurry image
    

    cv::waitKey(0);
    return 0;
}