// #include <opencv2/opencv.hpp>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main(int argc, char const *argv[]){
    //Create a blank image: 
    cv::Mat img(512,512,CV_8UC3,cv::Scalar(255,255,255)); //512 x 512 image with 3 channels white background
    //Scalar is the color of the image, The color is in BGR format (Blue, Green, Red)

    cv::circle(img,cv::Point(250,250),150,cv::Scalar(0,0,255),cv::FILLED);
    //cv::circle(image,center,radius,color,thickness) (It looks like the japanese flag now)

    cv::rectangle(img,cv::Point(75,75),cv::Point(425,425),cv::Scalar(32,191,32),cv::LINE_4);
    //cv::rectangle(image,point1,point2,color,thickness)
    //It now has a green rectangle around the circle


    //Now add some fun text
    cv::putText(img, "Cheesecakes are an illusion.", cv::Point(10, 50), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 0), 2);
    cv::putText(img, "There is no cheese in cake.", cv::Point(10, 470), cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 0), 2);


    cv::imshow("Image",img); //Show normal image
    cv::waitKey(0);
    return 0;
}