#!/bin/bash

### Save sudo password for the next sudo commands
sudo apt-get -y install build-essential cmake

### Downloading git projects
cd /opt
git clone https://github.com/opencv/opencv.git
git clone https://github.com/pjreddie/darknet.git

### Installing needed packages
sudo apt-get -y install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler
sudo apt-get -y install --no-install-recommends libboost-all-dev
sudo apt-get -y install python-dev python-numpy python-setuptools python-pip libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev
sudo apt-get -y install libusb-1.0-0-dev
sudo apt-get -y install beignet-dev
sudo apt-get -y install libva-dev libjpeg-dev
sudo apt-get -y install python3-pip

cd ~
sudo -H pip install --upgrade pip
sudo -H pip3 install --upgrade pip
sudo -H pip install --upgrade virtualenv
sudo -H pip install --upgrade google-cloud-vision

sudo apt-key adv --keyserver hkp://keys.gnupg.net:80 --recv-key C8B3A55A6F3EFCDE
sudo add-apt-repository "deb http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo bionic main" -u
sudo apt-get -y update
sudo apt-get install librealsense2-dkms
sudo apt-get install librealsense2-utils
sudo apt-get install librealsense2-dev
sudo apt-get install librealsense2-dbg
sudo apt-get install qtbase5-dev
sudo apt-get -y install python3-pydot python-pydot-ng graphviz
sudo apt-get -y install python-pydot 

### Compiling and installing previous git projects
cd /opt/opencv
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local ..
make -j8 all
make -j8 install

### Compiling and installing previous git projects
cd /opt/darknet
make -j8 all

### Rebooting computer
echo "Your computer needs to restart to finish the installation. Please save everything you're working on and press enter to reboot."

read -n 1 key
while [[ $key != "" ]]; do
	if [[ $key = "" ]]; then
		shutdown -r now
	else
		echo "You pressed the wrong key ! Please press enter to restart your computer."
	fi
	read -n 1 key
done
