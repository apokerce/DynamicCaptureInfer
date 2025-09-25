#include <iostream>
#include <thread>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <cstring>
#include <opencv2/dnn.hpp>
#include "include/Queue.h"
#include "include/ObjectDetector.h"
#include "include/VideoCapture.h"
#include "include/VideoWriter.h"
#include "include/Utils.h"

int main(int argc, char* kwargs[])
{
    /* frames queue is created to store the image frames that are taken from camera or video 
       FramesToSave queue is used for storing the frames that will be used in video since I 
       pick the images in object detection from frames queue VideoWrite function may take wrong frame */ 
    Queue frames(1000);
    Queue FramesToSave(1000);

    int Saveimage_index = 0; // Variable to differentiate the images that will be saved during streaming the video
    const char* Option1 = "c"; const char* Option2 = "v"; // Option1 and Option2 is used to understand whether the streaming will be made from video or camera
    
    bool saveCond = checkSaveRequest(kwargs); // saveCond controls the streaming video will be saved or not
    
    // Network arrangement and fetching available class names made here since both will be used in both camera and video input
    cv::dnn::Net net = arrangeNetwork();
    std::vector<std::string> classname = fetchClassNames();
    
    // User input is checked if input is chosen to be camera process is starting else if video else part starts
    if(kwargs[1][2] == Option1[0])
    {
        // Taken frames from camera is started using cap.open
        cv::VideoCapture cap;
        cap.open(atoi(kwargs[2]), cv::CAP_ANY);
        // Create output video writer
        cv::VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('M','J','P','G'), 5, cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

        while(true)
        {   
            // a thread is created for video capturing from camera std::ref was required in my compiler version 
            std::thread thread1(captureVideo, std::ref(frames), cap);
            thread1.join();

            // a thread is created for objectdetection phase
            std::thread thread2(ObjectDetector().detectObjects, std::ref(frames), std::ref(FramesToSave), classname, net, kwargs[4]);
            thread2.join();

            // This junction of code is used to show the user real time flow of the video
            if(!FramesToSave.QEmpty())
                cv::imshow("image", FramesToSave.QFront());

            // This part controls whether user wants to stop the process, change camera or take screenshot
            int Key = cv::waitKey(1);  // Controls if any key is pressed
            if(Key == 27)   // Press ESC for stop
                break;
            else if(Key == 48)   // Press 0 for internal camera
                cap.open((Key-48), cv::CAP_ANY);
            else if(Key == 49)   // Press 1 for external camera
                cap.open((Key-48), cv::CAP_ANY);
            else if(Key == 50)   // Press 2 for another external camera
                cap.open((Key-48), cv::CAP_ANY);
            if(Key == 97 && !FramesToSave.QEmpty()) // Press lower case "a" to save the image
            {
                // Image name is changing with number not to overlap with previously saved image
                std::string name = "image" + std::to_string(Saveimage_index++) + ".jpg";
                cv::imwrite(name, FramesToSave.QFront());
            }

            if(saveCond == true)
            {
                /* VideoWrite function deletes the frames that are used for saving video if condition fails else part is there to
                   not to take screenshot of the same image since above part only picks the frame in the queue */
                std::thread thread3(writeVideo, std::ref(FramesToSave), video);
                thread3.join();
            }
            else
                cv::Mat Empty = FramesToSave.Qdelete();
        }
        
    }
    else if(kwargs[1][2] == Option2[0]) 
    {
        // video name is converted to string since cap.open takes string argument fails when kwargs directly given
        cv::VideoCapture cap;
        std::string input(kwargs[2]);
        cap.open(input, cv::CAP_ANY);

        // Create output video writer
        cv::VideoWriter video("outcpp.avi", cv::VideoWriter::fourcc('M','J','P','G'), 10, cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)));

        while(true)
        {   
            // a thread is created for video capturing from camera std::ref was required in my compiler version 
            std::thread thread1(captureVideo, std::ref(frames), cap);
            thread1.join();

            // a thread is created for objectdetection phase
            std::thread thread2(ObjectDetector().detectObjects, std::ref(frames), std::ref(FramesToSave), classname, net, kwargs[4]);
            thread2.join();

            // This junction of code is used to show the user real time flow of the video
            if(!FramesToSave.QEmpty())
                cv::imshow("image", FramesToSave.QFront());

            // This part controls whether user wants to stop the process or take screenshot
            int Key = cv::waitKey(1);
            if(Key == 27)     // Press ESC for stop
                break;
            if(Key == 97 && !FramesToSave.QEmpty())  // Press lower case "a" to save the image
            {
                std::string name = "image" + std::to_string(Saveimage_index++) + ".jpg";
                cv::imwrite(name, FramesToSave.QFront());
            }

            if(saveCond == true)
            {
                /* Same purpose as in the camera streaming */
                std::thread thread3(writeVideo, std::ref(FramesToSave), video);
                thread3.join();
            }
            else
                cv::Mat Empty = FramesToSave.Qdelete();
        }

    }
    return 0;
}
