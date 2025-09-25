#ifndef UTILS_H
#define UTILS_H

#include <opencv2/opencv.hpp>
#include <opencv2/dnn.hpp>
#include <vector>
#include <string>

bool checkSaveRequest(char* kwargs[]);
std::vector<std::string> fetchClassNames();
cv::dnn::Net arrangeNetwork();

#endif // UTILS_H