cmake_minimum_required(VERSION 3.5.1)
project(reconnaissance-plans-activites)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions(-DUSE_REALSENSE)

find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PythonLibs 3.5.2 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

#get_filename_component(Caffe_CMAKE_DIR "./caffe/build/cmake" PATH)
#set(Caffe_DEFINITIONS "-DUSE_OPENCV;-DUSE_LMDB;-DUSE_LEVELDB")

add_library(libdarknet SHARED IMPORTED)
set_target_properties(libdarknet PROPERTIES IMPORTED_LOCATION /home/troisiememathieu/Music/YOLO/darknet/libdarknet.so)
link_directories(//home/troisiememathieu/Music/YOLO/darknet/include)
link_directories(/home/troisiememathieu/Music/YOLO/darknet)

find_package(Threads REQUIRED)
find_package(realsense2 REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${realsense2_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("/usr/include/libusb-1.0/")
include_directories("./activities")
include_directories("./activities/cnn")
include_directories("./activities/sources")
include_directories("./interface")
include_directories("./plans")
include_directories("./plans/include")
include_directories("./plans/src")
include_directories("./ressources")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets CONFIG REQUIRED)

file(GLOB_RECURSE Activities_SOURCES "activities/*.cpp")
file(GLOB_RECURSE Activities_HEADERS "activities/*.h")
file(GLOB_RECURSE ActivitiesCNN_SOURCES "activities/cnn/*.cpp")
file(GLOB_RECURSE ActivitiesCNN_HEADERS "activities/cnn/*.h")
file(GLOB_RECURSE ActivitiesVideoSources_SOURCES "activities/sources/*.cpp")
file(GLOB_RECURSE ActivitiesVideoSources_HEADERS "activities/sources/*.h")
file(GLOB_RECURSE Plans_HEADERS "plans/include/*.h")
file(GLOB_RECURSE Plans_SOURCES "plans/src/*.cpp")
FILE(GLOB_RECURSE QtHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "interface/*.h")
FILE(GLOB_RECURSE QtSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "interface/*.cpp")
FILE(GLOB_RECURSE QtForms RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "interface/*.ui")
FILE(GLOB_RECURSE QtRessources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "ressources/*.qrc")

list(APPEND Activities_HEADERS ${ActivitiesCNN_HEADERS} ${ActivitiesVideoSources_HEADERS} ${QtHeaders})
list(APPEND Activities_SOURCES ${ActivitiesCNN_SOURCES} ${ActivitiesVideoSources_SOURCES} ${QtSources})

list(REMOVE_ITEM QtHeaders "interface/configuration.h")

QT5_WRAP_CPP(header_mocs ${QtHeaders})
QT5_WRAP_UI(form_headers ${QtForms})
qt5_add_resources(RCC_SOURCES ${QtRessources})

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${Activities_HEADERS} ${Activities_SOURCES} ${Plans_HEADERS} ${Plans_SOURCES} ${header_mocs} ${form_headers} ${RCC_SOURCES})

TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${realsense2_LIBRARIES} -lrealsense2)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${Protobuf_LIBRARIES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${PYTHON_LIBRARIES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} libdarknet)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} Qt5::Widgets)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE /home/troisiememathieu/Music/YOLO/darknet/include)
