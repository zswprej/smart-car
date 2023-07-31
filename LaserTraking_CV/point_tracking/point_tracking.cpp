#include <cstdio>
#include <opencv2/opencv.hpp>

using namespace cv;


int main(void)
{
    uint8_t RedThres = 100;
    uint8_t GreenThres = 100;
    Mat OriginalFrameRGB;   // original RGB frame
    Mat OriginalFrameLAB;   // original LAB frame
    Mat GrayFrame;          // gray frame
    Mat RedThresFrame;      // red thresholded frame
    Mat GreenThresFrame;    // green thresholded frame
    VideoCapture cap("/dev/video0");
    if(!cap.isOpened())
    {
        std::cout<<"Error:cannot open camera!"<<std::endl;
        return -1;
    }
    namedWindow("Video", WINDOW_AUTOSIZE);
    while(true)
    {
        cap>>OriginalFrameRGB;
        if(OriginalFrameRGB.empty())
        {
            std::cout<<"Error:cannot load camera!"<<std::endl;
            break;
        }
        /* process image below */
        // cvtColor(OriginalFrameRGB, OriginalFrameLAB, COLOR_BGR2Lab);
        // inRange(OriginalFrameLAB, Scalar(0, RedThres, 0), Scalar(255, 255, 255), RedThresFrame);
        // inRange(OriginalFrameLAB, Scalar(0, 0, 0), Scalar(255, GreenThres, 255), GreenThresFrame);
        cvtColor(OriginalFrameRGB, GrayFrame, COLOR_BGR2GRAY);
        threshold(GrayFrame, RedThresFrame, 120, 255, /*THRESH_OTSU | */THRESH_BINARY);
        imshow("Video", OriginalFrameRGB);
        imshow("Red", RedThresFrame);
        // imshow("Green", GreenThresFrame);
        char key = waitKey(5);
        switch(key)
        {
        case 'q':
            goto exit;
        case 'w':
            RedThres += 1;
            std::cout<<"RedThres: "<<(int)RedThres<<std::endl;
            break;
        case 's':
            RedThres -= 1;
            std::cout<<"RedThres: "<<(int)RedThres<<std::endl;
            break;
        case 'a':
            GreenThres += 1;
            std::cout<<"GreenThres: "<<(int)GreenThres<<std::endl;
            break;
        case 'd':
            GreenThres -= 1;
            std::cout<<"GreenThres: "<<(int)GreenThres<<std::endl;
            break;
        default:
            break;
        }
    }
exit:
    cap.release();
    destroyAllWindows();
    return 0;
}