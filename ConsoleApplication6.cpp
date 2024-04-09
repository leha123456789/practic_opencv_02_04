#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>

using namespace cv;
using namespace std;

int main() 
{
    Scalar minHSV = Scalar(95, 12, 138); 
    Scalar maxHSV = Scalar(255, 50, 255);
    VideoCapture video("video1.mp4");
    video.isOpened();
    int width = static_cast<int>(video.get(CAP_PROP_FRAME_WIDTH));
    int height = static_cast<int>(video.get(CAP_PROP_FRAME_HEIGHT));
    VideoWriter outputVideo("output_video.avi", VideoWriter::fourcc('M', 'J', 'P', 'G'), 30, Size(width, height));
    namedWindow("Исходное видео", WINDOW_AUTOSIZE);
    namedWindow("Обнаружение цвета", WINDOW_AUTOSIZE);
    while (true)
    {
        Mat frame, frame_hsv, mask, thresh;
        video >> frame;
        if (frame.empty()) 
        {
            break;
        }
        cvtColor(frame, frame_hsv, COLOR_BGR2HSV);
        inRange(frame_hsv, minHSV, maxHSV, mask);
        GaussianBlur(mask, mask, Size(5, 5), 0);
        threshold(mask, thresh, 100, 255, THRESH_BINARY);
        vector<vector<Point>> contours;
        findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        for (const auto& contour : contours) 
        {
            Rect boundingBox = boundingRect(contour);
            rectangle(frame, boundingBox, Scalar(0, 255, 0), 2);
            if (boundingBox.width > boundingBox.height * 0.8 && boundingBox.width < boundingBox.height * 1.2)
            {
                putText(frame, "Rectangle", Point(boundingBox.x, boundingBox.y - 10), FONT_HERSHEY_SIMPLEX, 0.5, Scalar(0, 255, 0), 2);
            }
        }
        outputVideo.write(frame);
        imshow("Исходное видео", frame);
        imshow("Обнаружение цвета", thresh);
        if (waitKey(30) == 27) 
        {
            break;
        }
    }
    video.release();
    outputVideo.release();
    destroyAllWindows();
    return 0;
}
