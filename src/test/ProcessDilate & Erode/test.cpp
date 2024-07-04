// #include <opencv2/opencv.hpp>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    std::string path = "test.png";

    cv::Mat img = cv::imread(path);
    cv::Mat imgCanny, imgBlur, imgDil, imgErode; 


    cv::GaussianBlur(img, imgBlur, cv::Size(5,5), 5,0);

    //NOTE: It is good practice to use the blurred image for edge-detection
    cv::Canny(imgBlur, imgCanny, 25,100);


    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3)); //Decrease will dilate less
    cv::dilate(imgCanny, imgDil, kernel);

    cv::erode(imgDil, imgErode, kernel);
    
    
    cv::imshow("Edge", imgCanny);
    cv::imshow("Dilation", imgDil);
    cv::imshow("Erode", imgErode);

    cv::waitKey(0);
    return 0;
}