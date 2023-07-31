#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc.hpp"
#include <iostream>

cv::Mat getLaserMask_red(const cv::Mat& baseFrame, const cv::Mat& image) {
    cv::Mat diff = image - baseFrame;
    //cv::imshow("diff",diff);
    cv::Mat redDiffMask;
    //B G R
    cv::inRange(diff, cv::Scalar(0, 0, 100), cv::Scalar(255, 255, 255), redDiffMask);
    cv::Mat redMask;
    cv::inRange(image, cv::Scalar(0, 0, 150), cv::Scalar(150, 150, 255), redMask);
    cv::Mat whiteMask;
    cv::inRange(image, cv::Scalar(210, 210, 230), cv::Scalar(255, 255, 255), whiteMask);
    cv::Mat red_mask = (redMask | whiteMask) & redDiffMask;
    // cv::imshow("mask",red_mask);
    return red_mask;
}
cv::Mat getLaserMask_green(const cv::Mat& baseframe, const cv::Mat& image){
    cv::Mat diff = image - baseframe;
    cv::Mat greenDiffMask;
     //B G R
    cv::inRange(diff, cv::Scalar(0, 50, 0), cv::Scalar(255, 255, 255), greenDiffMask);
    cv::Mat greenMask;
    cv::inRange(image, cv::Scalar(0, 150, 0), cv::Scalar(150, 255, 150), greenMask);
    cv::Mat whiteMask;
    cv::inRange(image, cv::Scalar(210, 210, 230), cv::Scalar(255, 255, 255), whiteMask);
    cv::Mat green_mask = (greenMask | whiteMask) & greenDiffMask;
    //cv::imshow("mask",green_mask);
    return green_mask;
}
cv::Mat getLaserMask_bule(const cv::Mat& baseframe, const cv::Mat& image){
    cv::Mat diff = image - baseframe;
    cv::Mat blueDiffMask;
     //B G R
    cv::inRange(diff, cv::Scalar(50, 0, 0), cv::Scalar(255, 255, 255), blueDiffMask);
    cv::Mat blueMask;
    cv::inRange(image, cv::Scalar(150, 0, 0), cv::Scalar(255, 150, 150), blueMask);
    cv::Mat whiteMask;
    cv::inRange(image, cv::Scalar(210, 210, 230), cv::Scalar(255, 255, 255), whiteMask);
    cv::Mat blue_mask = (blueMask | whiteMask) & blueDiffMask;
    //cv::imshow("mask",green_mask);
    return blue_mask;
}

bool getPointerLocation(const cv::Mat& maskImage, cv::Point& location) {
    std::vector<std::vector<cv::Point>> contours;
    cv::findContours(maskImage, contours, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

    if(contours.empty()) {
        std::cout << "no lasers found" << std::endl;
        return false;
    }

    std::vector<cv::Point> maxCountour;
    double maxArea = 0;

    for(int i = 0; i < contours.size(); i++) {
        std::vector<cv::Point>& countour = contours[i];
        double area = cv::contourArea(countour);
        if(area > maxArea) {
            maxCountour = countour;
            maxArea = area;
        }
    }
    if(maxArea == 0) {
        return false;
    }
    std::cout << maxArea << " contour size " <<maxCountour.size() << std::endl;
    location = maxCountour[0];

    return true;
}

std::vector<cv::Point> getApproximateShape(const std::vector<cv::Point>& contour) {
    std::cout << "size" << contour.size();
    double perimeter = cv::arcLength(contour, true);

    std::vector<cv::Point> points;
    cv::approxPolyDP(contour, points, perimeter * 0.07, true);

    std::vector<cv::Point> convexPoints;
    cv::convexHull(points, convexPoints);

    return convexPoints;
}

std::string getPolygonName(int nPoints) {
    switch(nPoints) {
        case 1:
            return "Point";
        case 2:
            return "Line";
        case 3:
            return "Triangle";
        case 4:
            return "Quadrilateral";
        case 5:
            return "Pentagon";
        case 6:
            return "Hexagon";
        case 7:
            return "Heptagon";
        case 8:
            return "Octagon";
        default:
            return "Unknown";
    }
}

int main() {
    cv::VideoCapture cap(0);

    cv::waitKey(1000);

    cv::Mat baseFrame;
    cv::Mat prevFrame;
    cap.read(baseFrame);

    std::vector<std::vector<cv::Point>> contours(1);

    int framesNotFound = 0;

    if(!cap.isOpened())
    {
        std::cout<<"Error: camera open failed!"<<std::endl;
        return -1;
    }

    while((cv::waitKey(1) & 0xFF) != 27) {
        cv::Mat frame;
        cap.read(frame);
        if(frame.empty())
        {
            std::cout<<"Error:Frame empty!"<<std::endl;
            continue;
        }

        cv::Mat mask = getLaserMask_green(baseFrame, frame);
        baseFrame  = frame.clone();
        //cv::imshow("diff",mask);
        cv::Point pointerLocation;
        bool found = getPointerLocation(mask, pointerLocation);

        if(found) {
            contours[contours.size() - 1].push_back(pointerLocation);
            framesNotFound = 0;
        } else {
            framesNotFound++;
        }

        if(framesNotFound > 25 && contours[contours.size() - 1].size() > 2) {
            std::vector<cv::Point> approxPoints = getApproximateShape(contours.back());
            contours[contours.size() - 1] = approxPoints;
            contours.push_back(std::vector<cv::Point>());
            framesNotFound = 0;
        }

        cv::RNG rng(666648834);
        for(int i = 0; i < contours.size(); i++) {
            if(!contours[i].empty())
                cv::drawContours(frame, contours, i, cv::Scalar(rng.uniform(100,200), rng.uniform(100, 200), rng.uniform(100, 200)), 10);
        }
        if(contours.size() > 1) {
            std::string lastShape = getPolygonName(contours[contours.size() - 2].size());
            cv::putText(frame, "Last shape: " + lastShape, cv::Point(frame.cols / 2 - 450, frame.rows - 50), cv::FONT_HERSHEY_PLAIN, 3, cv::Scalar(255, 255, 255), 3);
        }
        cv::imshow("frame", frame);
    }
    return 0;
}