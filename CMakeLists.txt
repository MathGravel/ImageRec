cmake_minimum_required(VERSION 3.5.1)
project(recActPlan)
get_directory_property(MYDEFS COMPILE_DEFINITIONS)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11")
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_definitions(-DUSE_REALSENSE)
add_definitions(-DUSE_GPU) #comment/uncomment if you have a GPU on your pc (improve FPS)


#package and library required
find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Threads REQUIRED)
find_package(realsense2 REQUIRED)

add_library(libdarknet SHARED IMPORTED)
set_target_properties(libdarknet PROPERTIES IMPORTED_LOCATION /home/turtlebot/Documents/test/darknet/libdarknet.so)
link_directories(/home/turtlebot/Documents/test/darknet/include)
link_directories(/home/turtlebot/Documents/test/darknet)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${realsense2_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories("/usr/include/libusb-1.0/")


#Project directories
include_directories("/Imagerec2/controller")
include_directories("/Imagerec2/controller/sources")
include_directories("/Imagerec2/model")
include_directories("/Imagerec2/model/recoImage/cnn")
include_directories("/Imagerec2/model/recoImage")
include_directories("/Imagerec2/model/activities")
include_directories("/Imagerec2/model/plan")
include_directories("/Imagerec2/model/plan/PARC/src")
include_directories("/Imagerec2/model/plan/PARC/include")
include_directories("/Imagerec2/model/plan/PARC/Plan_Library")
include_directories("/Imagerec2/view")
#add here directories with code


#Project files cpp and h
file(GLOB_RECURSE Controller_SOURCES "controller/*.cpp")
file(GLOB_RECURSE Controller_HEADERS "controller/*.h")
file(GLOB_RECURSE Model_SOURCES "model/*.cpp")
file(GLOB_RECURSE Model_HEADERS "model/*.h")
file(GLOB_RECURSE View_SOURCES "view/*.cpp")
file(GLOB_RECURSE View_HEADERS "view/*.h")
#add here link with files

add_executable(${CMAKE_PROJECT_NAME} main.cpp ${Controller_HEADERS} ${Controller_SOURCES} ${Model_HEADERS} ${Model_SOURCES} ${View_HEADERS} ${View_SOURCES}) #Ajouter les SOURCES et HEADERS icitte.

TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${realsense2_LIBRARIES} -lrealsense2)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${Protobuf_LIBRARIES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} libdarknet)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE /home/turtlebot/Documents/test/darknet/include)
