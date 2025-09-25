#ifndef QUEUE_H
#define QUEUE_H

#include <opencv2/opencv.hpp>
#include <iostream>
#include <cstdlib>

class Queue
{   
private:
    int frontt, rear, countt, max_size_q;
    cv::Mat qlist[1000];
public:
    Queue(int m);
    void Qinsert(cv::Mat frame);
    cv::Mat Qdelete(void);
    cv::Mat QFront(void) const;
    int Qlength(void) const {return countt;}
    int QEmpty(void) const {return (countt==0);}
    int QFull(void) const {return countt==max_size_q;}
};

#endif // QUEUE_H