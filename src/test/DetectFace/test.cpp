#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>

int main(int argc, char const *argv[]) {
    std::string path = "linus.jpg";  // Original size: 640 × 480
    cv::Mat img = cv::imread(path), imgHSV, mask;

    cv::CascadeClassifier faceCascade;
    faceCascade.load("haarcascade_frontalface_default.xml");

    if(faceCascade.empty()) {
        std::cout << "XML file not loaded" << std::endl;
        return 1;
    }

    std::vector<cv::Rect> faces;
    faceCascade.detectMultiScale(img, faces, 1.1, 10); //Documentation (PY): https://docs.opencv.org/4.5.2/d1/de5/classcv_1_1CascadeClassifier.html#aaf8181cb63968136476ec4204ffca498


    // Iterate through all detected faces and draw rectangles around them
    for(int i = 0; i < faces.size(); i++) {
        cv::rectangle(img, faces[i].tl(), faces[i].br(), cv::Scalar(255, 0, 255), 3);
    }


    cv::imshow("Original", img);  // Show normal image
    cv::waitKey(0);
    return 0;
}
