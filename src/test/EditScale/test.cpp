// #include <opencv2/opencv.hpp>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main(int argc, char const *argv[])
{
    std::string path = "test.png"; //Original size: 300 x 168

    cv::Mat img = cv::imread(path);
    cv::Mat imgSize;

    std::cout << "Original size: " << img.size() << std::endl;

    resize(img,imgSize,cv::Size(),1.5,1.5,cv::INTER_LINEAR); //Resize image to 1.5 times the original size

    std::cout << "Scaled size: " << imgSize.size() << std::endl;

    cv::imshow("Image",img); //Show normal image

    cv::imshow("Image Scaled",imgSize); //Show resized image
    cv::waitKey(0);
    return 0;
}