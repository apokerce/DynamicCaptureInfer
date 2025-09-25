#include "Utils.h"
#include <cstring>
#include <fstream>

bool checkSaveRequest(char* kwargs[])
{   
    // User input is controlled if a save request is made condition is returned
    if(std::strcmp(kwargs[5], "true") == 0) // If label request is not made save request stored in fifth member of array
        return true;
    else if(std::strcmp(kwargs[5], "--save_image") == 0) 
    {
        if(std::strcmp(kwargs[6], "true") == 0)  // If label request is made save request stored in sixth member of array
            return true;
    }
        
    else 
        return false;
}

std::vector<std::string> fetchClassNames()
{
    /* This function reads a file where the class names which are available for Yolo and stores in a vector */
    std::vector<std::string> classNames;
    std::string classFile = "coco.names";
    std::ifstream names1(classFile.c_str());
    std::string line;
    while(std::getline(names1, line))
    {
        classNames.push_back(line);
    }
    return classNames;
}

cv::dnn::Net arrangeNetwork()
{
    // This function reads Config file and weights than creates a Yolo network
    std::string modelConfig = "yolov3.cfg";
    std::string modelWeights = "yolov3.weights";

    // Yolo network is created using below syntax and function is returned with network
    cv::dnn::Net network = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);
    network.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    return network;
}