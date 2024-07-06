// #include <opencv2/opencv.hpp>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

const double OUTPUT_WIDTH = 240, OUTPUT_HEIGHT = 320;

int main(int argc, char const *argv[]){
    std::string path = "cards.jpg"; //Original size: 300 x 168
    cv::Mat img = cv::imread(path), KDWarp, C7Warp, AOSWarp;

    //I wanted to use cv::Point2d instead of cv::Point2f because i dont care about performance for 1 image.
    //But it seems that cv::getPerspectiveTransform only accepts cv::Point2f

    //Store original points:
    std::vector<cv::Point2f> originalPointsKD = {cv::Point2f(559,1270),cv::Point2f(1027,988),cv::Point2f(1106,1811),cv::Point2f(1553,1437)};
    std::vector<cv::Point2f> originalPointsC7 = {cv::Point2f(1419,562),cv::Point2f(1824,507),cv::Point2f(1554,1059),cv::Point2f(2010,972)};
    std::vector<cv::Point2f> originalPointsAOS = {cv::Point2f(914,259),cv::Point2f(1282,342),cv::Point2f(576,621),cv::Point2f(997,725)};

    //Store destination points:
    std::vector<cv::Point2f> destinationPointsKD = {cv::Point2f(0.0,0.0),cv::Point2f(OUTPUT_WIDTH,0.0),cv::Point2f(0.0,OUTPUT_HEIGHT),cv::Point2f(OUTPUT_WIDTH,OUTPUT_HEIGHT)};

    //Warp the KD image. 
    cv::Mat matrixKD = cv::getPerspectiveTransform(originalPointsKD,destinationPointsKD);
    cv::warpPerspective(img,KDWarp,matrixKD,cv::Size(OUTPUT_WIDTH,OUTPUT_HEIGHT));

    //Warp the C7 image
    cv::Mat matrixC7 = cv::getPerspectiveTransform(originalPointsC7,destinationPointsKD);
    cv::warpPerspective(img,C7Warp,matrixC7,cv::Size(OUTPUT_WIDTH,OUTPUT_HEIGHT));

    //Warp the AOS image
    cv::Mat matrixAOS = cv::getPerspectiveTransform(originalPointsAOS,destinationPointsKD);
    cv::warpPerspective(img,AOSWarp,matrixAOS,cv::Size(OUTPUT_WIDTH,OUTPUT_HEIGHT));

    //Draw the points on the original image
    for(unsigned int i = 0; i < originalPointsKD.size(); i++){
        cv::circle(img,originalPointsKD[i],10,cv::Scalar(105,27,194),cv::FILLED);
    }

    for(unsigned int i = 0; i < originalPointsC7.size(); i++){
        cv::circle(img,originalPointsC7[i],10,cv::Scalar(105,27,194),cv::FILLED);
    }

    for(unsigned int i = 0; i < originalPointsAOS.size(); i++){
        cv::circle(img,originalPointsAOS[i],10,cv::Scalar(105,27,194),cv::FILLED);
    }

    //Show image scaled to 1280 x 720
    cv::resize(img,img,cv::Size(1280,720));
    cv::imshow("Original image",img); //Show normal image
    cv::imshow("King of Diamonds",KDWarp); //Show warped image
    cv::imshow("7 of Clubs",C7Warp); //Show warped image
    cv::imshow("Ace of Spades",AOSWarp); //Show warped image

    cv::waitKey(0);
    return 0;
}