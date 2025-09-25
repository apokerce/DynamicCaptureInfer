#ifndef VIDEO_WRITER_H
#define VIDEO_WRITER_H

#include <opencv2/opencv.hpp>
#include "Queue.h"

void writeVideo(Queue& framesForVideo, cv::VideoWriter video);

#endif // VIDEO_WRITER_H