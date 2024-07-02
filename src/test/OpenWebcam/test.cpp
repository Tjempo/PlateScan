#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

int main(int argc, char const *argv[]){

    unsigned long camID = 0; //Made it long so it wouldn't give errors.
    unsigned long DESIRED_WIDTH = 0;
    unsigned long DESIRED_HEIGHT = 0;
    
    if(argc < 2){
        std::cout << "No Camera ID provided. Defaulting to 0" << std::endl;
    }else{
        std::cout << "Provided ID = " << argv[1] << std::endl;
        camID = std::stoul(argv[1]);
        DESIRED_WIDTH = std::stoul(argv[2]);
        DESIRED_HEIGHT = std::stoul(argv[3]);
    }

    cv::VideoCapture cap(camID);
    cv::Mat img;

    cap.set(cv::CAP_PROP_FRAME_WIDTH, DESIRED_WIDTH);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, DESIRED_HEIGHT);

    double frame_width = cap.get(cv::CAP_PROP_FRAME_WIDTH);
    double frame_height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    std::cout << "Camera resolution: " << frame_width << "x" << frame_height << std::endl;

    int frame_count = 0;
    double fps = 0.0;
    int64 start_time = cv::getTickCount();

    while(true){
        try{
            cap.read(img);
            cv::imshow("Webcam Feed",img);
            cv::waitKey(1);

            //Print framerate of the camera:
            frame_count++;
        if (frame_count == 30) {
            int64 end_time = cv::getTickCount();
            double elapsed_time = (end_time - start_time) / cv::getTickFrequency();
            fps = frame_count / elapsed_time;
            std::cout << "Current FPS: " << fps << std::endl;

            // Reset for the next calculation
            frame_count = 0;
            start_time = cv::getTickCount();
        }

        }catch (const std::exception& e){ //Because after the last frame there will be an error
            // std::cout << "Exiting program: " << e.what() << std::endl;
            std::cout << "Feed stopped" << std::endl;
            break;
        }
    }
    cap.release();
    cv::destroyAllWindows();
    return 0;
}