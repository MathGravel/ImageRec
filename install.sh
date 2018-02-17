#!/usr/bin/bash
cd $HOME/Desktop
if [ ! -d "$HOME/Desktop/ImageRec" ]; then
 mkdir ImageRec
fi
cd ImageRec
ImageRecPath=$(pwd)
git clone https://github.com/OpenKinect/libfreenect2.git
git clone https://github.com/opencv/opencv.git
git clone https://github.com/opencv/opencv_contrib.git
git clone https://github.com/BVLC/caffe.git
apt-get -y install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler
apt-get -y install --no-install-recommends libboost-all-dev
apt-get -y install build-essential cmake pkg-config git libgtk2.0-dev libavcodec-dev libavformat-dev libswscale-dev
apt-get -y install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
apt-get -y install libusb-1.0-0-dev
apt-get -y install libturbojpeg0-dev
apt-get -y install libglfw3-dev
apt-get -y install beignet-dev
apt-get -y install libva-dev libjpeg-dev
apt-get -y install libopenni2-dev
apt-get -y install libopenblas-dev
cd opencv
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -DOPENCV_EXTRA_MODULES_PATH=$ImageRecPath/opencv_contrib/modules ..
cd $ImageRecPath/libfreenect2
mkdir build
cmake -DCMAKE_INSTALL_PREFIX=$ImageRecPath/freenect2
make -j8 install
cd $ImageRecPath/opencv/build
make -j8 install
cd $ImageRecPath/caffe
mkdir build
cd build
cmake ..
make -j8
make install

