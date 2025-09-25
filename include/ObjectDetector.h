#ifndef OBJECT_DETECTOR_H
#define OBJECT_DETECTOR_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>
#include <string>

class ObjectDetector
{
public:
    ObjectDetector();
    ~ObjectDetector();
    
    cv::dnn::Net initializeNetwork();
    std::vector<std::string> getClassNames();
    std::vector<std::string> getLastLayerNames(const cv::dnn::Net& net);
    void processDetection(cv::Mat& frame, const std::vector<cv::Mat>& outputs, 
                         const std::vector<std::string>& classNames, const std::string& label);
    void detectObjects(Queue& frames, Queue& saveFrames, const std::vector<std::string>& classNames, 
                      cv::dnn::Net network, const std::string& label);
    
private:
    std::string modelConfig;
    std::string modelWeights;
    std::string classFile;
    const int inputWidth;
    const int inputHeight;
};

#endif // OBJECT_DETECTOR_H