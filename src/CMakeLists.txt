cmake_minimum_required(VERSION 3.9)
project(reconnaissance-plans-activites)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions(-DUSE_REALSENSE)

find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PythonLibs 3.5.2 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

#get_filename_component(Caffe_CMAKE_DIR "/home/uqamportable/Documents/RobotLib/caffe/build/cmake" PATH)
#set(Caffe_DEFINITIONS "-DUSE_OPENCV;-DUSE_LMDB;-DUSE_LEVELDB")

add_library(libdarknet SHARED IMPORTED)
set_target_properties(libdarknet PROPERTIES IMPORTED_LOCATION /opt/darknet/libdarknet.so)
link_directories(/opt/darknet/include)
link_directories(/opt/darknet)

find_package(Threads REQUIRED)
find_package(realsense2 REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${realsense2_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("/usr/include/libusb-1.0/")
include_directories("./activities")
include_directories("./activities/CNN")
include_directories("./activities/VideoSources")
include_directories("./interface")
include_directories("./plans")
include_directories("./ressources")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets CONFIG REQUIRED)

file(GLOB_RECURSE Activities_SOURCES "activities/*.cpp")
file(GLOB_RECURSE Activities_HEADERS "activities/*.h")
file(GLOB_RECURSE ActivitiesCNN_SOURCES "activities/CNN/*.cpp")
file(GLOB_RECURSE ActivitiesCNN_HEADERS "activities/CNN/*.h")
file(GLOB_RECURSE ActivitiesVideoSources_SOURCES "activities/VideoSources/*.cpp")
file(GLOB_RECURSE ActivitiesVideoSources_HEADERS "activities/VideoSources/*.h")
list(APPEND Activities_SOURCES ${ActivitiesCNN_SOURCES} ${ActivitiesVideoSources_SOURCES} )
list(APPEND Activities_HEADERS ${ActivitiesCNN_HEADERS} ${ActivitiesVideoSources_HEADERS} )

FILE(GLOB_RECURSE headers RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "interface/*.h")
FILE(GLOB_RECURSE sources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "interface/*.cpp")
FILE(GLOB_RECURSE resources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "interface/*.qrc")
FILE(GLOB_RECURSE forms RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "interface/*.ui")
QT5_WRAP_CPP(header_mocs ${headers} ${Activities_HEADERS})
QT5_WRAP_UI(form_headers ${forms})
qt5_add_resources (RCC_SOURCES ${resources})

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${Activities_SOURCES} ${RCC_SOURCES} ${header_mocs} ${headers} ${sources}  ${form_headers} ${ImageRec_HEADERS})

TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${realsense2_LIBRARIES} -lrealsense2)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${Protobuf_LIBRARIES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${PYTHON_LIBRARIES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} libdarknet)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} Qt5::Widgets)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE /opt/darknet/include)
