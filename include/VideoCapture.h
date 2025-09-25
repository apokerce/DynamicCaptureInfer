#ifndef VIDEO_CAPTURE_H
#define VIDEO_CAPTURE_H

#include <opencv2/opencv.hpp>
#include "Queue.h"

void captureVideo(Queue& frame, cv::VideoCapture cap);

#endif // VIDEO_CAPTURE_H