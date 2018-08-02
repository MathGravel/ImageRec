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
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets CONFIG REQUIRED)

file(GLOB_RECURSE ImageRec_SOURCES "Source files/*.cpp")
file(GLOB_RECURSE ImageRec_HEADERS "Header files/*.h")
file(GLOB_RECURSE ImageRec1_SOURCES "*.cpp")
list(APPEND ImageRec_SOURCES ${ImageRec1_SOURCES} )

FILE( GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" )
FILE( GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp" )
FILE( GLOB_RECURSE resources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.qrc" )
FILE( GLOB_RECURSE forms RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.ui" )
QT5_WRAP_CPP( header_mocs ${headers} ${ImageRec_HEADERS} )
QT5_WRAP_UI( form_headers ${forms} )
qt5_add_resources (RCC_SOURCES ${resources})

#add_executable (ImageRec ${ImageRec_SOURCES} )




add_executable(ImageRec main.cpp ${ImageRec_SOURCES} ${RCC_SOURCES} ${header_mocs} ${form_headers} ${ImageRec_HEADERS} )




target_link_libraries(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${realsense2_LIBRARIES} -lrealsense2)
target_link_libraries(${CMAKE_PROJECT_NAME} ${Protobuf_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} ${PYTHON_LIBRARIES})
target_link_libraries(${CMAKE_PROJECT_NAME} libdarknet)
target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Widgets)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE /home/uqamportable/Documents/libRobot/darknet/include)

