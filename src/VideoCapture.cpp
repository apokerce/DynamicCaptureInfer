#include "VideoCapture.h"

void captureVideo(Queue& frame, cv::VideoCapture cap)
{
    // cap.read reads the camera frame and write to frame1 then its pushed to queue as its requeired
    cv::Mat frame1;
    cap.read(frame1);   
    frame.Qinsert(frame1);
}