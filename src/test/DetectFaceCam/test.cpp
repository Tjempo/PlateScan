#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

const unsigned short OUTPUT_WIDTH = 1280, OUTPUT_HEIGHT = 720;

int main(int argc, char const *argv[]) {
    cv::VideoCapture cap(0);
    cv::Mat img;

    cap.set(cv::CAP_PROP_FRAME_WIDTH, OUTPUT_WIDTH);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, OUTPUT_HEIGHT);

    // Load the cascade:
    cv::CascadeClassifier faceCascade;
    faceCascade.load("haarcascade_frontalface_default.xml");

    if(faceCascade.empty()) {
        std::cout << "XML file not loaded" << std::endl;
        return 1;
    }


    std::vector<cv::Rect> faces;

    while (true) {
        cap.read(img);

        // Clear vector if not empty:
        if (!faces.empty()) {
            faces.clear();
        }

        // Detect faces:
        faceCascade.detectMultiScale(img, faces, 1.1, 10);  // Documentation (PY): https://docs.opencv.org/4.5.2/d1/de5/classcv_1_1CascadeClassifier.html#aaf8181cb63968136476ec4204ffca498

        // Iterate through all detected faces and draw rectangles around them
        for (int i = 0; i < faces.size(); i++) {
            cv::rectangle(img, faces[i].tl(), faces[i].br(), cv::Scalar(255, 0, 255), 3);
        }

        cv::imshow("Face Detection", img);
        cv::waitKey(20); 
    }

    cap.release();
    cv::destroyAllWindows();
    return 0;
}
