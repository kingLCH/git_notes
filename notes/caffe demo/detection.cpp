#include "detection.h"


std::vector<DetectionBox>  yolo_detection::forward(const cv::Mat& img)
{
    std::vector<float> results = Forward(img);
    _results =result_parse(results);

    return _results;
}


std::vector<DetectionBox>  yolo_detection::result_parse(std::vector<float> feature)
{
    int batchsize = 1;
    int input_width=640, input_height=360;
    int map_chann = 5, map_width = 10, map_height = 6;

    int result_size = map_chann *  map_width * map_height;
    float grid_width = input_width/((float)(map_width));
    float grid_height = input_height/((float)(map_height-1));
    static int framenum;
    //fill detData
    std::vector<DetectionBox> frame_result_boxes;
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

                    DetectionBox tb;
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