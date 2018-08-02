#!/bin/bash

CMakeList=$(pwd)
### save sudo password for the next sudo commands
sudo apt-get -y install build-essential cmake pkg-config git libgtk2.0-dev libavcodec-dev libavformat-dev libswscale-dev

cd /usr/local
ImageRecPath=$(pwd)

### Downloading git projects
git clone https://github.com/OpenKinect/libfreenect2.git
git clone https://github.com/opencv/opencv.git
git clone https://github.com/BVLC/caffe.git

### Installing needed packages
sudo apt-get -y install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler
sudo apt-get -y install --no-install-recommends libboost-all-dev
sudo apt-get -y install python-dev python-numpy python-setuptools python-pip libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev
sudo apt-get -y install libusb-1.0-0-dev
sudo apt-get -y install libturbojpeg libjpeg-turbo8-dev
sudo apt-get -y install libglfw3-dev
sudo apt-get -y install beignet-dev
sudo apt-get -y install libva-dev libjpeg-dev
sudo apt-get -y install libopenni2-dev
sudo apt-get -y install libgtkmm-3.0-dev

cd ~
sudo pip install --upgrade pip
sudo pip install --upgrade virtualenv
sudo pip install --upgrade google-cloud-vision

echo 'deb http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo xenial main' | sudo tee /etc/apt/sources.list.d/realsense-public.list
sudo apt-key adv --keyserver keys.gnupg.net --recv-key 6F3EFCDE
sudo apt-get -y update
sudo apt-get -y install realsense-uvcvideo
sudo apt-get -y install librealsense2-utils
sudo apt-get -y install librealsense2-dev
sudo apt-get -y install librealsense2-dbg
sudo apt-get -y install python3-pydot python-pydot-ng graphviz


### Compiling and installing previous git projects
cd $ImageRecPath/opencv
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
make
make -j8 install

cd $ImageRecPath/libfreenect2
mkdir build
cd build
cmake .. -DCMAKE_INSTALL_PREFIX=$ImageRecPath/freenect2 -Dfreenect2_DIR=$ImageRecPath/freenect2/lib/cmake/freenect2
make
make -j8 install

cd $ImageRecPath/caffe
rm CMakeLists.txt
cp $CMakeList/CMakeListsCaffe.txt CMakeLists.txt
mkdir build
cd build
cmake ..
make all -j8
make install -j8
cd ..
cp -R build/include/caffe/proto include/caffe/

echo 'export PYTHONPATH=${PYTHONPATH}:${CMakeList}/cnnModel' >> $HOME/$USER/.bashrc
echo 'export PYTHONPATH=${PYTHONPATH}:${ImageRecPath}/caffe/python' >> $HOME/$USER/.bashrc

### Rebooting computer
echo "Your computer needs to restart to finish the installation, please save everything you're working on and press enter to reboot."

read -s -n 1 key

while [[ $key != "" ]]; do
	if [[ $key = "" ]]; then
		shutdown -r now
	else
		echo "You pressed the wrong key, please press enter to restart your computer."
	fi

	read -s -n 1 key
done
