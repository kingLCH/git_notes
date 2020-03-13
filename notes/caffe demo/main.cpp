#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#define CPU_ONLY

using namespace caffe;  // NOLINT(build/namespaces)
using std::string;
using namespace cv;


struct Detection
{
    float conf;
    cv::Rect_<float> box;

};


/* Pair (label, confidence) representing a prediction. */
typedef std::pair<string, float> Prediction;

class CNN {
 public:
  CNN(const string& model_file,
             const string& trained_file,
             const string& mean_file);

  //std::vector<Prediction> Classify(const cv::Mat& img, int N = 5);
  std::vector<float> Forward(const cv::Mat& img);

 private:
  void SetMean(const string& mean_file);

  void WrapInputLayer(std::vector<cv::Mat>* input_channels);

  void Preprocess(const cv::Mat& img,
                  std::vector<cv::Mat>* input_channels);

 private:
  shared_ptr<Net<float> > net_;
  cv::Size input_geometry_;
  int num_channels_;
  cv::Mat mean_;
  std::vector<string> labels_;
};

CNN::CNN(const string& model_file,
                       const string& trained_file,
                       const string& mean_file) {

  std::cout<<"CNN: initlize\n";
#ifdef CPU_ONLY
  Caffe::set_mode(Caffe::CPU);
  std::cout<<"CNN: Set CPU mode\n";
#else
  Caffe::set_mode(Caffe::GPU);
  std::cout<<"CNN: Set GPU mode\n";
#endif

  /* Load the network. */
  net_.reset(new Net<float>(model_file, TEST));
  net_->CopyTrainedLayersFrom(trained_file);
  std::cout<<"CNN: Load network and copy model weights\n";

  CHECK_EQ(net_->num_inputs(), 1) << "Network should have exactly one input.";
  CHECK_EQ(net_->num_outputs(), 1) << "Network should have exactly one output.";

  Blob<float>* input_layer = net_->input_blobs()[0];
  num_channels_ = input_layer->channels();
  CHECK(num_channels_ == 3 || num_channels_ == 1)
    << "Input layer should have 1 or 3 channels.";
  
  std::cout<<"CNN: Set GPU mode\n";
  printf("CNN: Input layer shape %d*%d*%d*%d(num*chan*height*width)\n",
    input_layer->num(), input_layer->channels(), input_layer->height(), input_layer->width());
  input_geometry_ = cv::Size(input_layer->width(), input_layer->height());

  /* Load the binaryproto mean file. */
  // SetMean(mean_file);
  // std::cout<<"CNN: substract the mean file\n";


  Blob<float>* output_layer = net_->output_blobs()[0];

  std::cout<<"CNN: output layer dimension "<<output_layer->count()<<std::endl;
  printf("CNN: Output layer shape %d*%d*%d*%d(num*chan*height*width)\n",
    output_layer->num(), output_layer->channels(), output_layer->height(), output_layer->width());
  std::cout<<"CNN: initialized success\n";
}

// static bool PairCompare(const std::pair<float, int>& lhs,
//                         const std::pair<float, int>& rhs) {
//   return lhs.first > rhs.first;
// }
// /* Return the indices of the top N values of vector v. */
// static std::vector<int> Argmax(const std::vector<float>& v, int N) {
//   std::vector<std::pair<float, int> > pairs;
//   for (size_t i = 0; i < v.size(); ++i)
//     pairs.push_back(std::make_pair(v[i], i));
//   std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), PairCompare);
//   std::vector<int> result;
//   for (int i = 0; i < N; ++i)
//     result.push_back(pairs[i].second);
//   return result;
// }

/* Return the top N predictions. */
// std::vector<Prediction> Classifier::Classify(const cv::Mat& img, int N) {
//   std::vector<float> output = Predict(img);
//   N = std::min<int>(labels_.size(), N);
//   std::vector<int> maxN = Argmax(output, N);
//   std::vector<Prediction> predictions;
//   for (int i = 0; i < N; ++i) {
//     int idx = maxN[i];
//     predictions.push_back(std::make_pair(labels_[idx], output[idx]));
//   }
//   return predictions;
// }

/* Load the mean file in binaryproto format. */
void CNN::SetMean(const string& mean_file) {
  BlobProto blob_proto;
  ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);

  /* Convert from BlobProto to Blob<float> */
  Blob<float> mean_blob;
  mean_blob.FromProto(blob_proto);
  CHECK_EQ(mean_blob.channels(), num_channels_)
    << "Number of channels of mean file doesn't match input layer.";

  /* The format of the mean file is planar 32-bit float BGR or grayscale. */
  std::vector<cv::Mat> channels;
  float* data = mean_blob.mutable_cpu_data();
  for (int i = 0; i < num_channels_; ++i) {
    /* Extract an individual channel. */
    cv::Mat channel(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
    channels.push_back(channel);
    data += mean_blob.height() * mean_blob.width();
  }

  /* Merge the separate channels into a single image. */
  cv::Mat mean;
  cv::merge(channels, mean);

  /* Compute the global mean pixel value and create a mean image
   * filled with this value. */
  cv::Scalar channel_mean = cv::mean(mean);
  mean_ = cv::Mat(input_geometry_, mean.type(), channel_mean);
}

std::vector<float> CNN::Forward(const cv::Mat& img) {
  Blob<float>* input_layer = net_->input_blobs()[0];
  input_layer->Reshape(1, num_channels_,
                       input_geometry_.height, input_geometry_.width);
  /* Forward dimension change to all layers. */
  net_->Reshape();

  std::vector<cv::Mat> input_channels;
  WrapInputLayer(&input_channels);

  Preprocess(img, &input_channels);

  net_->Forward();

  /* Copy the output layer to a std::vector */
  Blob<float>* output_layer = net_->output_blobs()[0];
  const float* begin = output_layer->cpu_data();
  const float* end = begin + output_layer->channels()*output_layer->height()*output_layer->width();
  return std::vector<float>(begin, end);
}

/* Wrap the input layer of the network in separate cv::Mat objects
 * (one per channel). This way we save one memcpy operation and we
 * don't need to rely on cudaMemcpy2D. The last preprocessing
 * operation will write the separate channels directly to the input
 * layer. */
void CNN::WrapInputLayer(std::vector<cv::Mat>* input_channels) {
  Blob<float>* input_layer = net_->input_blobs()[0];

  int width = input_layer->width();
  int height = input_layer->height();
  float* input_data = input_layer->mutable_cpu_data();
  for (int i = 0; i < input_layer->channels(); ++i) {
    cv::Mat channel(height, width, CV_32FC1, input_data);
    input_channels->push_back(channel);
    input_data += width * height;
  }
}

void CNN::Preprocess(const cv::Mat& img,
                            std::vector<cv::Mat>* input_channels) {
  /* Convert the input image to the input image format of the network. */
  cv::Mat sample;
  if (img.channels() == 3 && num_channels_ == 1)
    cv::cvtColor(img, sample, cv::COLOR_BGR2GRAY);
  else if (img.channels() == 4 && num_channels_ == 1)
    cv::cvtColor(img, sample, cv::COLOR_BGRA2GRAY);
  else if (img.channels() == 4 && num_channels_ == 3)
    cv::cvtColor(img, sample, cv::COLOR_BGRA2BGR);
  else if (img.channels() == 1 && num_channels_ == 3)
    cv::cvtColor(img, sample, cv::COLOR_GRAY2BGR);
  else
    sample = img;

  cv::Mat sample_resized;
  if (sample.size() != input_geometry_)
    cv::resize(sample, sample_resized, input_geometry_);
  else
    sample_resized = sample;

  cv::Mat sample_float;
  if (num_channels_ == 3)
    sample_resized.convertTo(sample_float, CV_32FC3);
  else
    sample_resized.convertTo(sample_float, CV_32FC1);

  // cv::Mat sample_normalized;
  // cv::subtract(sample_float, mean_, sample_normalized);
  cv::normalize(sample_float,sample_float,0,1,CV_MINMAX);

  /* This operation will write the separate BGR planes directly to the
   * input layer of the network because it is wrapped by the cv::Mat
   * objects in input_channels. */
  cv::split(sample_float, *input_channels);

  CHECK(reinterpret_cast<float*>(input_channels->at(0).data)
        == net_->input_blobs()[0]->cpu_data())
    << "Input channels are not wrapping the input layer of the network.";
}


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

  std::cout << "---------- Prediction for "
            << file << " ----------" << std::endl;

  cv::Mat img = cv::imread(file, -1);
  CHECK(!img.empty()) << "Unable to decode image " << file;
  std::vector<float> predictions = cnn.Forward(img);

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

  for (size_t i = 0; i < raw_results.size(); i++)
  {
    rectangle(img, raw_results[i].box, Scalar(100*i%255, 0, (i+100)%255), 2);
  }
  cv::imwrite("result.jpg", img);
  cv::imshow("result.jpg", img);
  cv::waitKey(0);

}
