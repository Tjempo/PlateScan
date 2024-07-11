// #include <opencv2/opencv.hpp>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

cv::Mat imgOriginal, imgCanny, imgGray, imgBlur, imgDil, imgWarp;

const float a4Width = 420, a4Height = 596;


//Function Defs:
cv::Mat preProcessing(cv::Mat img);
std::vector<cv::Point> getContours(cv::Mat imgDil, cv::Mat img);
void drawPoints(std::vector<cv::Point> points, cv::Scalar color);
std::vector<cv::Point> reorderPoints(std::vector<cv::Point> points);
cv::Mat getWarp(cv::Mat img, std::vector<cv::Point> points, float width, float height);


int main(int argc, char const *argv[]){
    std::string path = "cropped_doc.jpg";
    imgOriginal = cv::imread(path);

    //Scale the image:
    cv::resize(imgOriginal, imgOriginal, cv::Size(), 0.3, 0.3);

    preProcessing(imgOriginal);
    std::vector<cv::Point> initialPoints = getContours(imgDil, imgOriginal);
    std::vector<cv::Point> docPoints = reorderPoints(initialPoints);
    drawPoints(docPoints, cv::Scalar(255, 0, 0));

    imgWarp = getWarp(imgOriginal, docPoints, a4Width, a4Height);

    // cv::imshow("Original", imgOriginal);
    cv::imshow("Dilation", imgDil);
    cv::imshow("Warp", imgWarp);
    cv::waitKey(0);
    return 0;
}

cv::Mat preProcessing(cv::Mat img){
    cv::cvtColor(imgOriginal, imgGray, cv::COLOR_BGR2GRAY);
    cv::GaussianBlur(imgGray, imgBlur, cv::Size(3, 3), 3, 0);
    cv::Canny(imgBlur, imgCanny, 25, 75);
    cv::dilate(imgCanny, imgDil, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
    return imgDil;
}


std::vector<cv::Point> getContours(cv::Mat imgDil, cv::Mat img) {
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    std::vector<cv::Point> biggest;

    // Documentation for findContours: https://docs.opencv.org/4.x/d3/dc0/group__imgproc__shape.html#gadf1ad6a0b82947fa1fe3c3d497f260e0
    cv::findContours(imgDil, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    // Define a vector of vectors to store the points of the polygon:
    std::vector<std::vector<cv::Point>> contoursPoly(contours.size());
    std::vector<cv::Rect> boundRect(contours.size());
    int maxArea = 0;

    for(unsigned short i = 0; i < contours.size(); i++){
        unsigned long area = cv::contourArea(contours[i]);
    
        if(area > 1000){
            float perimeter = cv::arcLength(contours[i], true);
            cv::approxPolyDP(contours[i], contoursPoly[i], 0.016 * perimeter, true);
    
            if(area > maxArea && contoursPoly[i].size() == 4){
                maxArea = area;
                biggest = {contoursPoly[i][0], contoursPoly[i][1], contoursPoly[i][2], contoursPoly[i][3]};
                cv::drawContours(imgCanny, contours, i, cv::Scalar(255, 0, 255), 2);
            }
        }
    }
    return biggest;
}


void drawPoints(std::vector<cv::Point> points, cv::Scalar color){
    for(unsigned short i = 0; i < 4; i++){
        cv::circle(imgCanny, points[i], 10, color, cv::FILLED);
        cv::putText(imgCanny, std::to_string(i), points[i], cv::FONT_HERSHEY_PLAIN, 2, color, 2);
    }
}

std::vector<cv::Point> reorderPoints(std::vector<cv::Point> points){
    std::vector<cv::Point> newPoints;
    std::vector<int> sumPoints, subPoints;

    for(unsigned short i = 0; i < points.size(); i++){
        sumPoints.push_back(points[i].x + points[i].y);
        subPoints.push_back(points[i].x - points[i].y);
    }

    newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]); 
    newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
    newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);
    newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);

    return newPoints;
    
}

cv::Mat getWarp(cv::Mat img, std::vector<cv::Point> points, float width, float height){
    cv::Point2f src[4] = {points[0], points[1], points[2], points[3]};
    cv::Point2f dst[4] = {{0.0f, 0.0f}, {width, 0.0f}, {0.0f, height}, {width, height}};

    cv::Mat matrix = cv::getPerspectiveTransform(src, dst);
    cv::warpPerspective(img, imgWarp, matrix, cv::Point(width, height));

    return imgWarp;
}