.PHONY: clean all test deps tags 

CXX=g++
CXXFLAGS := -g -Wall -O -std=c++11 

#if use cpu
CXXFLAGS += -DCPU_ONLY=1

CAFFE_ROOT_DIR := /home/lch/caffe
OPENCV_ROOT_DIR := /home/lch/opencv-3.4.0/install
PROTO_ROOT_DIR := /home/lch/protobuf/protobuflib-351

DEPS_INCLUDE_PATH :=  -I $(CAFFE_ROOT_DIR)/include/    \
		      -I $(OPENCV_ROOT_DIR)/include	\
		      -I $(PROTO_ROOT_DIR)/include
				

CAFFELIB = -L $(CAFFE_ROOT_DIR)/build/lib/ 

#CUDALIB = -L /usr/local/cuda-9.0/lib64/

OPENCVLIBS :=-L $(OPENCV_ROOT_DIR)/lib



LIBS =  -lboost_system -lcaffe -lglog  -lprotobuf -lgflags #-lcudart   

# use opencv
LIBS += -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs

OBJS := $(patsubst %.cpp,%.o,$(wildcard *.cpp))

TARGET = main
  
$(TARGET): $(OBJS) 
	$(CXX)  -o $@  $^ $(LIBS)  $(OPENCVLIBS) $(CAFFELIB) $(CUDALIB)  $(DEPS_INCLUDE_PATH) 


%.o:%.cpp
	$(CXX) -c $(CXXFLAGS)  $< $(DEPS_INCLUDE_PATH) 


clean:
	rm -f *.o $(TARGET)


