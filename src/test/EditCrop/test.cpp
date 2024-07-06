// #include <opencv2/opencv.hpp>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    std::string path = "test.png"; //Original size: 300 x 168
    cv::Mat img = cv::imread(path);
    cv::Mat imgCrop;

    //Show ROI (Region of Interest)
    cv::Rect roi(50,50,200,90); //x & y to start | width & height of the new image
    imgCrop = img(roi);

    std::cout << "Size of cropped image: " << imgCrop.size() << std::endl; //This will be 200 x 90 or the size of the ROI


    cv::imshow("Image",img); //Show normal image
    cv::imshow("Image Crop",imgCrop); //Show cropped image

    cv::waitKey(0);
    return 0;
}