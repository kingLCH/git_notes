#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include "CNN.h"
#include "timing.h"
#include "detection.h"
#define CPU_ONLY

using std::string;
using namespace cv;


struct Detection
{
    float conf;
    cv::Rect_<float> box;
};




std::vector<Detection> yolo_result_parse(std::vector<float> feature)
{
    int batchsize = 1;
    int input_width=640, input_height=360;
    int map_chann = 5, map_width = 10, map_height = 6;
    int result_size = map_chann *  map_width * map_height;

    float grid_width = input_width/((float)(map_width));
    float grid_height = input_height/((float)(map_height-1));
    static int framenum;
    //fill detData
    vector<Detection> frame_result_boxes;
    for (size_t i = 0; i < batchsize; i++)
    {
        /* code */
        
        for(int j=0;j<map_height;j++){
            for(int k=0;k<map_width;k++){ //最后一组丢弃
                if(feature[result_size*i+map_width*j+k]>=0.5)
                {
                    int x = (int)(  (   (float)(k) + feature[result_size*i+map_width*map_height+map_width*j+k])    *grid_width);
                    int y = (int)(  (   (float)(j) + feature[result_size*i+2*map_width*map_height+map_width*j+k]) * grid_height);
                    int w = (int)((feature[result_size*i+3*map_width*map_height+map_width*j+k])* input_width );
                    int h = (int)((feature[result_size*i+4*map_width*map_height+map_width*j+k])* input_height);

                    int x0 = x - w/2;
                    int y0 = y - h/2;
                    int x1 = x + w/2;
                    int y1 = y + h/2;

                    Detection tb;
                    tb.conf = feature[result_size*i+map_width*j+k];
                    tb.box = Rect_<float>(Point_<float>(x0, y0), Point_<float>(x1, y1));
                    frame_result_boxes.push_back(tb);
                }
            }
        }
        printf("raw data:%d\n", frame_result_boxes.size());
    }
    return frame_result_boxes;
}

int main(int argc, char** argv) {
  //"deploy.prototxt", "gender_net.caffemodel","imagenet_mean.binaryproto"
  string file = "1.jpg";
  string model_file   = "darkNet15_2.prototxt";
  string trained_file = "darknet.caffemodel";
  string mean_file   = "imagenet_mean.binaryproto";

  CNN cnn(model_file, trained_file, mean_file);

  yolo_detection yolo_detector(model_file, trained_file, mean_file);

  std::cout << "---------- Prediction for "
            << file << " ----------" << std::endl;

  cv::Mat img = cv::imread(file, -1);
  CHECK(!img.empty()) << "Unable to decode image " << file;

  //double startTime = now();
  std::vector<float> predictions = cnn.Forward(img);
  // double nDetectTime = calcElapsed(startTime, now());
  // printf("timer: %d ms.\n ", (int)(nDetectTime * 1000));

  /* Print the top N predictions. */
  for (size_t i = 0; i < predictions.size(); ++i) {
    if (i%10==0)
    {
      /* code */
      std::cout << (i/10)+1 <<":";
    }
    
    std::cout << std::fixed << std::setprecision(8) << predictions[i]<<" ";
    if (i%10==9)
    {
      /* code */
      std::cout <<  std::endl;
    }
  }

  //visualized result
   std::vector<Detection>  raw_results = yolo_result_parse(predictions);

  double startTime = now();
   std::vector<DetectionBox>  resultss = yolo_detector.forward(img);
   double nDetectTime = calcElapsed(startTime, now());
  printf("timer: %d ms.\n ", (int)(nDetectTime * 1000));


  for (size_t i = 0; i < raw_results.size(); i++)
  {
    rectangle(img, resultss[i].box, Scalar(100*i%255, 0, (i+100)%255), 2);
  }
  cv::imwrite("result.jpg", img);
  cv::imshow("result.jpg", img);
  cv::waitKey(0);

}
