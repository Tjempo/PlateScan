#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <iostream>

#ifdef __MINGW32__
#include <windows.h>

#include <vector>
#elif __linux__
#include <limits.h>
#include <unistd.h>
#endif


std::string getCurrentDateTime();

const unsigned short OUTPUT_WIDTH = 1280, OUTPUT_HEIGHT = 720;

int main(int argc, char const *argv[]) {
    cv::VideoCapture cap(2);
    cv::Mat img;

    cap.set(cv::CAP_PROP_FRAME_WIDTH, OUTPUT_WIDTH);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, OUTPUT_HEIGHT);

    // Load the cascade:
    cv::CascadeClassifier faceCascade;
    faceCascade.load("haarcascade_licence_plate_rus_16stages.xml");

    if(faceCascade.empty()) {
        std::cout << "XML file not loaded" << std::endl;
        return 1;
    }


    std::vector<cv::Rect> plates;

    while (true) {
        cap.read(img);

        // Clear vector if not empty:
        if (!plates.empty()) {
            plates.clear();
        }

        // Detect plates:
        faceCascade.detectMultiScale(img, plates, 1.1, 8);  // Documentation (PY): https://docs.opencv.org/4.5.2/d1/de5/classcv_1_1CascadeClassifier.html#aaf8181cb63968136476ec4204ffca498

        // Iterate through all detected plates and draw rectangles around them
        for (int i = 0; i < plates.size(); i++) {
            cv::Mat imgCrop = img(plates[i]);
            cv::imwrite("plates/plate_" + getCurrentDateTime() + ".png", imgCrop);
            cv::rectangle(img, plates[i].tl(), plates[i].br(), cv::Scalar(255, 0, 255), 3);
        }

        cv::imshow("Plate Detection", img);
        cv::waitKey(20); 
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}


// Define cross-platform localtime function:
#if defined(_WIN32) || defined(_WIN64)
#define LOCALTIME_S(tm_ptr, time_ptr) localtime_s((tm_ptr), (time_ptr))
#else  // For Linux
#define LOCALTIME_S(tm_ptr, time_ptr) localtime_r((time_ptr), (tm_ptr))
#endif

std::string getCurrentDateTime() {
    struct tm timeinfo;
    time_t now = time(0);
    LOCALTIME_S(&timeinfo, &now);
    std::stringstream ss;
    ss << std::put_time(&timeinfo, "%Y-%m-%d_%H-%M-%S");
    return ss.str();
}