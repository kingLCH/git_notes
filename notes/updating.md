## AndroidStudio

AndroidStudio SSL peer shut down incorrectly 问题  

* 添加文本 maven { url 'http://maven.aliyun.com/nexus/content/groups/public/' }

## caffe依赖库

* sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler

* sudo apt-get install --no-install-recommends libboost-all-dev

* sudo apt-get install libgflags-dev libblas-dev liblmdb-dev libatlas-base-dev

## docker

启动docker服务（ Is the docker daemon running?）

* service docker start  

## nnie安装

* cmake -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=/home/lch/protobuf/protobuflib-351 ../cmake

* LD_LIBRARY_PATH=/home/lch/protobuf/protobuflib-351/lib:$LD_LIBRARY_PATH
  export PKG_CONFIG_PATH=/home/lch/protobuf/protobuflib-351/lib/pkgconfig

交叉编译器安装好后编译出现问题 arm-himix200-linux-g++: error while loading shared libraries: libstdc++.so.6: cannot open shared object file: No such file or directory

* sudo apt-get install ia32-libs
* sudo apt-get install g++-multilib

nfs共享文件夹

* apt-get install nfs-kernel-server
* sudo vim /etc/exports
      #文件最后一行加上
      /home/lch/hi_share/ *(rw,sync,no_root_squash,no_subtree_check)
* sudo exportfs -rv

实现共享

* mount -t nfs -o nolock,rize=1024,wsize=1024 192.168.12.23:/home/lch/hi_share /share

nfs配置

* sudo service rpcbind start
* sudo service nfs-server start

查看可以挂载的目录

* showmount -e localhost

卸载镜像目录

* sudo umount /tmp

登录板端

* ssh root@192.168.12.100

opencv 安装好后测试

* gcc `pkg-config --cflags opencv` -o facedetect facedetect.cpp `pkg-config --libs opencv` -lstdc++

## Atlas

* 192.168.0.2
  HwHiAiUser
  Mind@123
  #Atlas usb 訪問
  ssh -p 22 HwHiAiUser@192.168.1.2

启动mind studio ,进入“~/tools/bin”路径

* bash start.h

查找当前文件夹下包含关键字的文本

* grep -r "关键字"  ./

## 交叉编译opencv
[编译参考](https://blog.csdn.net/sanallen/article/details/79022669?utm_source=blogxgwz27)

1. 设定cmake中的编译器

​	cmake的setup界面选择specify options for cross-compiling
​		Operating system随意命名
​		Compilers选择gCC和g++，例如/usr/bin/aarch64-linux-gnu-gcc和/usr/bin/aarch64-linux-gnu-g++
​		Target Root一定要选择编译器相关头文件和库等放在一起的地方，比如/usr/aarch64-linux-gnu
​		Include Mode一定要选择“search only in Target Root”

2. 接着关闭各种不需要的opencv的模块
   诸如：WITH_CUDA
   WITH_CUFFT                        # FLASE 禁用CUFFT
   WITH_FFMPEG                       # FLASE 禁用FFMPEG
   WITH_OPENCL                       # FLASE 禁用OPENCL
   WITH_OPENCLAMDBLAS                # FLASE 禁用OPENCLAMDBLAS
   WITH_OPENCLAMDFFT                 # FLASE 禁用OPENCLAMDFFT
   WITH_OPENCL_SVM                   # FLASE 禁用OPENCL_SVM

3. 指定安装路径（CMAKE_INSTALL_PREFIX），作为最后使用的路径
4. 后续问题修改参照[交叉编译opencv问题参考1](https://www.veryarm.com/116215.html)和[交叉编译opencv问题参考2](https://blog.csdn.net/qq_34533248/article/details/101203162)

