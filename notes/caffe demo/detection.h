#pragma once
#include "CNN.h"
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
using std::string;
using namespace cv;

struct DetectionBox
{
    float conf;
    cv::Rect_<float> box;
};

class yolo_detection:CNN {

public:
    yolo_detection(const string& model_file,
             const string& trained_file,
             const string& mean_file):CNN(model_file,trained_file, mean_file)
             {

             };
    
    std::vector<DetectionBox> forward(const cv::Mat& img);

protected:
    std::vector<DetectionBox> result_parse(std::vector<float> feature);

   
    std::vector<DetectionBox> _results;
};