cmake_minimum_required(VERSION 3.9)
project(ImageRec)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")


add_definitions(-DUSE_REALSENSE)

find_package(PkgConfig REQUIRED)
find_package( OpenCV REQUIRED )
find_package(PythonLibs 3.5.2 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

#get_filename_component(Caffe_CMAKE_DIR "/home/uqamportable/Documents/RobotLib/caffe/build/cmake" PATH)
message(${PYTHON_LIBRARIES})

set(Caffe_DEFINITIONS "-DUSE_OPENCV;-DUSE_LMDB;-DUSE_LEVELDB")




add_library(libdarknet SHARED IMPORTED)
set_target_properties(libdarknet PROPERTIES IMPORTED_LOCATION /home/uqamportable/Documents/libRobot/darknet/libdarknet.so)
link_directories(/home/uqamportable/Documents/libRobot/darknet/include)
link_directories(/home/uqamportable/Documents/libRobot/darknet)


find_package(Threads REQUIRED)
find_package(realsense2 REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${realsense2_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("/usr/include/libusb-1.0/")
include_directories("./Header files")
include_directories("./Header files/CNN")
include_directories("./Source files/")
include_directories("./Source files/CNN")



include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories("Source files/")
include_directories("Source files/CNN/")

ADD_DEFINITIONS(${GTK3_CFLAGS_OTHER})


file(GLOB_RECURSE ImageRec_SOURCES "Source files/*.cpp")
file(GLOB_RECURSE ImageRec_HEADERS "Header files/*.h")

#file(GLOB_RECURSE ImageRec1_SOURCES "Source files/*.cpp")
#file(GLOB_RECURSE ImageRec1_HEADERS "Header files/*.h")

#file(GLOB_RECURSE ImageRec2_SOURCES "Source files/CNN/*.cpp")
#file(GLOB_RECURSE ImageRec2_HEADERS "Header files/CNN/*.h")

#list(APPEND ImageRec_SOURCES ${ImageRec1_SOURCES} )
#list(APPEND ImageRec_SOURCES ${ImageRec2_SOURCES} )
#list(APPEND ImageRec_HEADERS ${ImageRec1_HEADERS} )
#list(APPEND ImageRec_HEADERS ${ImageRec2_HEADERS} )


#set (ImageRec_INCLUDE_DIRS "")
#foreach (_headerFile ${ImageRec_HEADERS})
#    get_filename_component(_dir ${_headerFile} PATH)
#    list (APPEND ImageRec_INCLUDE_DIRS ${_dir})
#endforeach()
#list(REMOVE_DUPLICATES ImageRec_INCLUDE_DIRS)

#add_executable (ImageRec ${ImageRec_SOURCES} )
add_executable(ImageRec main.cpp ${ImageRec_SOURCES})
#target_include_directories(ImageRec PRIVATE ${ImageRec_INCLUDE_DIRS})

#add_executable(ImageRec OldFiles/VideoArea.cpp OldFiles/MainWindow.cpp OldFiles/main.cpp "OldFiles/ImageSegmentationManual.cpp" "OldFiles/ImageSegmentationManual.h" OldFiles/FileManager.cpp OldFiles/FileManager.h "Source files/VideoSources/RealSense.cpp" OldFiles/Classifier.cpp "Source files/CNN/CaffeCNN.cpp" "Source files/Socket.cpp" "Header files/CNN/ImgSegCNN.h" "Source files/CNN/ImgSegCNN.cpp" "Header files/selectiveSearchDepth.h" "Source files/CNN/MaskRCNN.cpp" "Header files/CNN/MaskRCNN.h" "Header files/DetectedObject.h" "Source files/ObjectAff.cpp" "Header files/ObjectAff.h" "Header files/Affordance.h" "Header files/ActivityRegion.h" "Source files/ActivityRegion.cpp" "Source files/Policy.cpp" "Header files/Policy.h" "Source files/VideoSources/RealSenseVideo.cpp" "Header files/VideoSources/RealSenseVideo.h" "Source files/CNN/YoloGPU.cpp" "Header files/CNN/YoloGPU.h" "Header files/Detector.h" "Source files/RecoManager.cpp" "Header files/RecoManager.h" "Header files/VideoSources/VideoFeedTypes.h" "Header files/Serializable.h" "Source files/ImageTreatment.cpp" "Header files/ImageTreatment.h" "Header files/CNN/Yolo.h" "Source files/CNN/YoloCPU.cpp" "Header files/CNN/YoloCPU.h")



target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${realsense2_LIBRARIES} -lrealsense2)
target_link_libraries(${CMAKE_PROJECT_NAME} ${Protobuf_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${PYTHON_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} libdarknet)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE /home/uqamportable/Documents/libRobot/darknet/include)

