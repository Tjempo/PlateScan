#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main(int argc, char const *argv[]){

    std::string path = "video.mp4";
    cv::VideoCapture cap(path);
    cv::Mat img; 

    while(true){
        try{
            cap.read(img);
            cv::imshow("Video",img);
            cv::waitKey(42);
        }catch (const std::exception& e){ //Because after the last frame there will be an error
            // std::cout << "Exiting program: " << e.what() << std::endl;
            std::cout << "Video Finished" << std::endl;
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}