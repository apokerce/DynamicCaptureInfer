#include "Queue.h"

Queue::Queue(int m)
{   
    frontt = 0;
    rear = 0;
    countt = 0;
    max_size_q = m;  
}

void Queue::Qinsert(cv::Mat frame)
{   
    if (QFull())
    {   
        std::cerr << "Queue overflow" << std::endl;
        exit(1);
    }
    countt++;
    qlist[rear] = frame;
    rear = (rear + 1) % 1000;   
}

cv::Mat Queue::Qdelete(void)
{   
    cv::Mat temp;
    if(QEmpty())
    {
        return temp;
    }
    temp = qlist[frontt];
    countt--;
    frontt = (frontt + 1) % max_size_q;
    return temp;    
}

cv::Mat Queue::QFront(void) const
{
    return qlist[frontt];
}