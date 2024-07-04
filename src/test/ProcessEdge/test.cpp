// #include <opencv2/opencv.hpp>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    std::string path = "test.png";

    cv::Mat img = cv::imread(path);
    cv::Mat imgCanny;
    cv::Mat imgBlur;
    cv::Mat imgBlur2;


    cv::GaussianBlur(img, imgBlur, cv::Size(5,5), 5,0);
    cv::GaussianBlur(img, imgBlur2, cv::Size(5,5), 5,0);

    //NOTE: It is good practice to use the blurred image for edge-detection
    cv::Canny(imgBlur, imgCanny, 50,50); 

    //Params: Threashold for detecting what is an edge. Higher = Less edges

    /*
    https://docs.opencv.org/4.x/d7/de1/tutorial_js_canny.html
    - threshold1	first threshold for the hysteresis procedure.
    - threshold2	second threshold for the hysteresis procedure.
    */


    cv::imshow("Edge 1", imgCanny);

    cv::Canny(imgBlur, imgCanny, 25,100); 

    cv::imshow("Edge 2", imgCanny);

    cv::waitKey(0);
    return 0;
}