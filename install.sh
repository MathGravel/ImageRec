#!/bin/bash


sudo apt-get -y install python-dev python-numpy python-setuptools python-pip libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libdc1394-22-dev
sudo apt-get -y install libusb-1.0-0-dev
sudo apt-get -y install beignet-dev
sudo apt-get -y install libva-dev libjpeg-dev
sudo apt-get -y install python3-pip cmake

cd ~
sudo -H pip install --upgrade pip
sudo -H pip3 install --upgrade pip
sudo -H pip install --upgrade virtualenv
sudo -H pip install --upgrade google-cloud-vision

#à revérifier sur le git de realsense

#https://github.com/IntelRealSense/librealsense/blob/master/doc/distribution_linux.md
sudo apt-key adv --keyserver hkp://keys.gnupg.net:80 --recv-key C8B3A55A6F3EFCDE
#sudo apt-key adv --keyserver keys.gnupg.net --recv-key C8B3A55A6F3EFCDE || sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-key C8B3A55A6F3EFCDE
#

sudo add-apt-repository "deb http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo bionic main" -u
sudo apt-get -y update
sudo apt-get -y upgrade
sudo apt-get install librealsense2-dkms
sudo apt-get install librealsense2-utils
sudo apt-get install librealsense2-dev
sudo apt-get install librealsense2-dbg

#Ubuntu 16.04?
sudo apt-get install qttools5-dev #Pour qt5_add_translation dans le CMake
sudo apt-get install qtbase5-dev
sudo apt-get -y install python3-pydot python-pydot-ng graphviz
sudo apt-get -y install python-pydot 
#


#Ajouter check si OpenCv déjà installé
while true; do
    read -p "Do you want to install OpenCv? (y/n)?" yn
    case $yn in
        [Yy]* ) 
		#Assurer de creer les folders /opt/opencv et darknet îs rentrer dedans avec cd
		### Compiling and installing previous git projects
		sudo rm -r /opt/opencv
		sudo mkdir -p /opt
		cd /opt
		sudo git clone "https://github.com/opencv/opencv"
		sudo mkdir -p /opt/opencv/build
		cd opencv/build
		sudo cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local -D ENABLE_PRECOMPILED_HEADERS=OFF ..
		sudo make -j8 all
		sudo make -j8 install;
		break;;
        [Nn]* ) break;;
        * ) echo "Please answer yes or no.";;
    esac
done

### Compiling and installing previous git projects
sudo rm -r /opt/darknet
sudo mkdir -p /opt/darknet
cd /opt
sudo git clone "https://github.com/pjreddie/darknet"
cd darknet

#Installation si GPU est dispo selon un yes no
while true; do
    read -p "Is the GPU available? (y/n)?" yn
    case $yn in
        [Yy]* ) 
	sudo sed -i '/GPU=0/c\GPU=1' /opt/darknet/Makefile;
	break;;
        [Nn]* ) break;;
        * ) echo "Please answer yes or no.";;
    esac
done

#Si GPU, sed Makefile pour changer la premiere ligne de GPU=0 a GPU=1
#creer une var den
sudo make -j8 all

echo "If GPU available, go install :";
echo "https://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html";
echo "https://developer.nvidia.com/cuda-downloads";

### Rebooting computer
echo "Your computer needs to restart to finish the installation. Please save everything you're working on and press enter to reboot."

read -n 1 key

while [[ $key != "" ]]; do
	echo "You pressed the wrong key ! Please press enter to restart your computer."
	read -n 1 key
done
sudo shutdown -r now
