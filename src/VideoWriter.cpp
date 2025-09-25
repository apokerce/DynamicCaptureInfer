#include "VideoWriter.h"

void writeVideo(Queue& framesForVideo, cv::VideoWriter video)
{
    /* This function takes the saved frames and using video.write OpenCV creates the video */
    if(!framesForVideo.QEmpty())
    {
        cv::Mat frame = framesForVideo.Qdelete();
        video.write(frame);
    }
}