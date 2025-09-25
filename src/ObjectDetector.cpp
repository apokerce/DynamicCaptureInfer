#include "ObjectDetector.h"
#include <fstream>
#include <iostream>

ObjectDetector::ObjectDetector() : 
    modelConfig("yolov3.cfg"),
    modelWeights("yolov3.weights"),
    classFile("coco.names"),
    inputWidth(416),
    inputHeight(416)
{
}

ObjectDetector::~ObjectDetector()
{
}

cv::dnn::Net ObjectDetector::initializeNetwork()
{
    // Yolo network is created using below syntax and function is returned with network
    cv::dnn::Net network = cv::dnn::readNetFromDarknet(modelConfig, modelWeights);
    network.setPreferableTarget(cv::dnn::DNN_TARGET_CPU);
    return network;
}

std::vector<std::string> ObjectDetector::getClassNames()
{
    /* This function reads a file where the class names which are available for Yolo and stores in a vector */
    std::vector<std::string> classNames;
    std::ifstream names1(classFile.c_str());
    std::string line;
    while(std::getline(names1, line))
    {
        classNames.push_back(line);
    }
    return classNames;
}

std::vector<std::string> ObjectDetector::getLastLayerNames(const cv::dnn::Net& net)
{
    /* The forwarding process needs to know the layer where the process ends this function takes
       the last layer names and store in a string which is given to forwarding */

    std::vector<std::string> names_temp;
    std::vector<int> outLayers = net.getUnconnectedOutLayers(); 
    std::vector<std::string> layersNames = net.getLayerNames();
    names_temp.resize(outLayers.size());
    for (int i = 0; i < outLayers.size(); ++i)
        names_temp[i] = layersNames[outLayers[i] - 1];
    return names_temp;
}

void ObjectDetector::processDetection(cv::Mat& frame, const std::vector<cv::Mat>& outputs, 
                                    const std::vector<std::string>& classNames, const std::string& label)
{
    // This function take outputs that are given by the network and draw the bounding boxes of interests which determined by the user 
    
    const float nonMaxSuppressionThresh = 0.4; // Confidence below this value will be deleted directly in the NMS part
    const float confThresh = 0.5;              // Confidence score below this constant will now be shown
    std::vector<int> Ids;                      // Ids vector is used to store the detected class ID's
    std::vector<float> Confidence;             // This vector is storing the confidence scores that will be shown to user
    std::vector<cv::Rect> BoundingBoxes;       // Stores the bounding box coordinates

    /* In the loop below all the predictions are checked and the predictions' information that are exceeding
       the predetermined confidence threshold are saved in the vectors that are defined above */
    for(int i = 0; i < outputs.size(); i++)
    {
        float* data = (float*)outputs[i].data;
        for(int j = 0; j < outputs[i].rows; j++)
        {
            cv::Mat scores = outputs[i].row(j).colRange(5, outputs[i].cols);
            cv::Point ClassIds;
            double confidence;
            // This function returns the location of maximum score that is predicted by the last layer
            cv::minMaxLoc(scores, 0, &confidence, 0, &ClassIds);
            if(confidence > confThresh)
            {
                // Store coordinates of bounding boxes and arrange according to input size
                int width = (int)(data[2] * frame.cols);
                int height = (int)(data[3] * frame.rows);
                int left = (int)(data[0] * frame.cols) - (width / 2);
                int top = (int)(data[1] * frame.rows) - (height / 2);
                Ids.push_back(ClassIds.x);
                Confidence.push_back((float)confidence);
                BoundingBoxes.push_back(cv::Rect(left, top, width, height));
            }
            data += outputs[i].cols;
        }
    }
    
    // Non max supression is used to get rid of the overlaying bounding boxes that are belong to the same object
    // indexes each corresponding an object is returned by the function
    std::vector<int> indexes;
    cv::dnn::NMSBoxes(BoundingBoxes, Confidence, confThresh, nonMaxSuppressionThresh, indexes);

    /* In this loop, indexes are scanned and user input is checked whether all objects will be shown 
       or user requested a specific class will be shown */
    for(int i = 0; i < indexes.size(); i++)
    {
        // Index, class index and bounding box coordinates are extracted
        int id = indexes[i];
        int classId = Ids[id];
        cv::Rect box = BoundingBoxes[id];

        /* If there exists a special request from the user only that classes will be drawn so input and predicted class Id is checked
           If a request is made label will have requested label otherwise --save_image string will be in label so all objects shown*/
        if(std::strcmp(label.c_str(), classNames[classId].c_str()) == 0)
        {   
            // A rectangle is drawn on the frame using the bounding box coordinates 
            cv::rectangle(frame, cv::Point(box.x, box.y), cv::Point((box.x + box.width), (box.y + box.height)), cv::Scalar(0,0,255), 3);
            // A string is created using confidence level and name of the object
            std::string labelStr = cv::format("%.2f", Confidence[id]) + ":" + classNames[classId];
            // getTextSize returns the size of the text in pixels
            int baseline;
            cv::Size labelsize = cv::getTextSize(labelStr, cv::FONT_HERSHEY_COMPLEX, 0.5, 1, &baseline);
            // A rectangle is drawn for label and label name is put using putText
            cv::rectangle(frame, cv::Point(box.x, box.y - std::round(1.5*labelsize.height)), cv::Point(box.x + std::round(1.5*labelsize.width), box.y + baseline), cv::Scalar(0,0,0), cv::FILLED);
            cv::putText(frame, labelStr, cv::Point(box.x, box.y), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255,255,255),1);
        }
        else if(std::strcmp(label.c_str(), ("--save_image")) == 0)
        {
            // A rectangle is drawn on the frame using the bounding box coordinates 
            cv::rectangle(frame, cv::Point(box.x, box.y), cv::Point((box.x + box.width), (box.y + box.height)), cv::Scalar(0,0,255), 3);
            // A string is created using confidence level and name of the object
            std::string labelStr = cv::format("%.2f", Confidence[id]) + ":" + classNames[classId];
            // getTextSize returns the size of the text in pixels
            int baseline;
            cv::Size labelsize = cv::getTextSize(labelStr, cv::FONT_HERSHEY_COMPLEX, 0.5, 1, &baseline);
            // A rectangle is drawn for label and label name is put using putText
            cv::rectangle(frame, cv::Point(box.x, box.y - std::round(1.5*labelsize.height)), cv::Point(box.x + std::round(1.5*labelsize.width), box.y + baseline), cv::Scalar(0,0,0), cv::FILLED);
            cv::putText(frame, labelStr, cv::Point(box.x, box.y), cv::FONT_HERSHEY_SIMPLEX, 0.75, cv::Scalar(255,255,255),1);
        }
    }
}

void ObjectDetector::detectObjects(Queue& frames, Queue& saveFrames, const std::vector<std::string>& classNames, 
                                  cv::dnn::Net network, const std::string& label)
{
    cv::Mat frame, blob;
    if(!frames.QEmpty())
    {
        /* Frame is taken from queue, resized and normalized that given to the networks input
           outputs are taken as vector and network is forwarded meaning outputs according to input is taken than
           it is send ProcessData_DrawBoxes function to draw bounding boxes */
        frame = frames.Qdelete();
        cv::dnn::blobFromImage(frame, blob, 1/255.0, cv::Size(inputWidth, inputHeight), cv::Scalar(0,0,0), true, false);
        network.setInput(blob);
        std::vector<cv::Mat> outputs;
        network.forward(outputs, getLastLayerNames(network)); // getLastLayerNames returns the name of the last layer
        processDetection(frame, outputs, classNames, label);
        // Convert frame to CV_8U format for OpenCV to be able to save video
        cv::Mat detectedFrame;
        frame.convertTo(detectedFrame, CV_8U);
        saveFrames.Qinsert(detectedFrame); // Queue where video frames are saved
    }
}