## AndroidStudio

**AndroidStudio SSL peer shut down incorrectly 问题**

* 添加文本 maven { url 'http://maven.aliyun.com/nexus/content/groups/public/' }

**AndroidStudio 导入别的工程时出现 ERROR: Failed to resolve: com.android.support:appcompat-v7:29.0.0问题**

1. 涉及到不同版本的差异，注意修改build.gradle文件中的对应版本

2. 若目标平台是29，则额外需要在gradle.properties 文件中增加下面两句话

   android.useAndroidX=true
   android.enableJetifier=true

**AndroidStudio 打开别人得工程出现sync错误，提示找不到相关得包 **

- 查看具体什么包，可以在这里去找<https://maven.aliyun.com/mvn/search> 
- 拷贝到类似此路径`C:\Users\charlesliu\.gradle\caches\modules-2\files-2.1`



## caffe依赖库

* sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler

* sudo apt-get install --no-install-recommends libboost-all-dev

* sudo apt-get install libgflags-dev libblas-dev liblmdb-dev libatlas-base-dev

## docker

**启动docker服务（ Is the docker daemon running?）**

* service docker start  

## nnie安装

* cmake -Dprotobuf_BUILD_TESTS=OFF -Dprotobuf_BUILD_SHARED_LIBS=ON -DCMAKE_INSTALL_PREFIX=/home/lch/protobuf/protobuflib-351 ../cmake

* LD_LIBRARY_PATH=/home/lch/protobuf/protobuflib-351/lib:$LD_LIBRARY_PATH
  export PKG_CONFIG_PATH=/home/lch/protobuf/protobuflib-351/lib/pkgconfig

**交叉编译器安装好后编译出现问题 arm-himix200-linux-g++: error while loading shared libraries: libstdc++.so.6: cannot open shared object file: No such file or directory**

* sudo apt-get install ia32-libs
* sudo apt-get install g++-multilib

**nfs共享文件夹**

* apt-get install nfs-kernel-server
* sudo vim /etc/exports
      #文件最后一行加上
      /home/lch/hi_share/ *(rw,sync,no_root_squash,no_subtree_check)
* sudo exportfs -rv

**实现共享**

* ~~mount -t nfs -o nolock,rize=1024,wsize=1024 192.168.12.23:/home/lch/hi_share /share~~
* mount -t nfs -o nolock,rsize=1024,wsize=1024 192.168.12.23:/home/lch/hi_share share/

**nfs配置**

* sudo service rpcbind start
* sudo service nfs-server start

**查看可以挂载的目录**

* showmount -e localhost

**卸载镜像目录**

* sudo umount /tmp

**登录板端**

* ssh root@192.168.12.100

**opencv 安装好后测试**

* gcc `pkg-config --cflags opencv` -o facedetect facedetect.cpp `pkg-config --libs opencv` -lstdc++

## Atlas 200

* 192.168.0.2
* HwHiAiUser
* Mind@123

**Atlas usb 訪問**

* ssh -p 22 HwHiAiUser@192.168.1.2

**启动mind studio ,进入“~/tools/bin”路径**

* bash start.hc

**查找当前文件夹下包含关键字的文本**

* grep -r "关键字"  ./

**从host拷贝文件到device**

* `sudo scp -r ./include/* HwHiAiUser@192.168.1.2:/usr/local/include`

**新增opencv调用，须修改工程路径下的build文件夹下的CMakeLists.txt，增加头文件路径和库路径**

* 在工程目录下输入命令`cd build && cmake . && make`

## 交叉编译opencv
[编译参考](https://blog.csdn.net/sanallen/article/details/79022669?utm_source=blogxgwz27)

1. 设定cmake中的编译器:

   * cmake的setup界面选择specify options for cross-compiling

   * Operating system~~随意命名~~<font color=red>只能命名为Linux</font>，

   * Compilers选择gcc和g++，例如/usr/bin/aarch64-linux-gnu-gcc和/usr/bin/aarch64-linux-gnu-g++

   * Target Root一定要选择编译器相关头文件和库等放在一起的地方，比如/usr/aarch64-linux-gnu

   * Include Mode一定要选择“search only in Target Root”
   
2. 接着关闭各种不需要的opencv的模块

    * WITH_CUDA                                     **FLASE 关闭cuda**
    * WITH_CUFFT                                    **FLASE 禁用CUFFT**
    * WITH_FFMPEG                                 **FLASE 禁用FFMPEG**
    * WITH_OPENCL                                 **FLASE 禁用OPENCL**
    * WITH_OPENCLAMDBLAS               **FLASE 禁用OPENCLAMDBLAS**
    * WITH_OPENCLAMDFFT                  **FLASE 禁用OPENCLAMDFFT**
    * WITH_OPENCL_SVM                        **FLASE 禁用OPENCL_SVM**
    
3. 为了避免使用的时候库要放在其他地方，**CMAKE_FIND_ROOT_PATH_MODE_PROGRAME**需要设置成**NEVER**

4. 指定安装路径（CMAKE_INSTALL_PREFIX），作为最后使用的路径

5. 后续问题修改参照[交叉编译opencv问题参考1](https://www.veryarm.com/116215.html)和[交叉编译opencv问题参考2](https://blog.csdn.net/qq_34533248/article/details/101203162)

6. 编译好后如何应用在嵌入式平台上

    1. 将编译好的opencv的库拷贝到板子上

       `sudo scp -r ./opencvlib/lib/* HwHiAiUser@192.168.1.2:/home/username/opencvlib`

    2. 将lib所在的路径加到系统变量中

       `sudo vim /etc/profile`

       在该文件最下面添加这句话：

       `export LD_LIBRARY_PATH=/home/usrname/opencv/lib:$LD_LIBRARY_PATH`

    3. 保存退出后输入命令使它生效：`source /etc/profile`

        

## 常用linux命令

**查找可执行文件所涉及的库**

* `ldd filename`

**创建软链接(libx.so.3.4指向libx.so.3.4.0)**

* `ln -sf libx.so.3.4.0 libx.so.3.4`

**查看文件信息**

* `ls -lh finename` 

**查看库依赖具体信息**

* `readelf -d filename`

**查看软链接指向文本**

* `ls -lah *.so  `

**查看编译好的库所用的编译器 版本**

* `readelf filename.so -p .comment`



## 常见问题

**使用apt-get install时出现错误E: Sub-process /usr/bin/dpkg returned an error code (1) 查看软链接指向文本**

* 1.`sudo mv /var/lib/dpkg/info /var/lib/dpkg/info.bk  `
* 2. `sudo mkdir /var/lib/dpkg/info  `
  3.  `sudo apt-get update`
  4.  `sudo apt-get install -f`
  5.  `sudo mv /var/lib/dpkg/info/* /var/lib/dpkg/info.bk`
  6. `rm -rf /var/lib/dpkg/infoinfo`
  7. `sudo mv /var/lib/dpkg/info.bk/* /var/lib/dpkg/info`

**模型可视化神器**
https://github.com/lutzroeder/Netron
