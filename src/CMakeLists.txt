cmake_minimum_required(VERSION 3.5.1)
project(reconnaissance-plans-activites)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_definitions(-DUSE_REALSENSE)
#add_definitions(-DUSE_GPU)
#add_definitions(-DUSE_KITCHEN)
#add_definitions(-DUSE_KITCHEN_DIST)


get_directory_property(MYDEFS COMPILE_DEFINITIONS)
if(MYDEFS MATCHES "^USE_KITCHEN" OR MYDEFS MATCHES ";USE_KITCHEN")
    if(EXISTS "traces")
    else()
        file(MAKE_DIRECTORY "traces")
    endif()

endif()

if(MYDEFS MATCHES "^USE_KITCHEN_DIST" OR MYDEFS MATCHES ";USE_KITCHEN_DIST")
    if(EXISTS "tracesDist")
    else()
        file(MAKE_DIRECTORY "tracesDist")
    endif()

endif()



find_package(PkgConfig REQUIRED)
find_package(OpenCV REQUIRED)
find_package(PythonLibs 3.5.2 REQUIRED)
include_directories(${PYTHON_INCLUDE_DIRS})

#get_filename_component(Caffe_CMAKE_DIR "./caffe/build/cmake" PATH)
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
include_directories("./activities/cnn")
include_directories("./activities/sources")
include_directories("./interface")
include_directories("./PARC")
include_directories("./PARC/include")
include_directories("./PARC/src/solver")
include_directories("./PARC/Plan_Library")

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
file(GLOB_RECURSE Plans_HEADERS "PARC/include/*.h")
file(GLOB_RECURSE Plans_SOURCES "PARC/src/solver/*.cpp")
file(GLOB_RECURSE PLSOURCES  "./PARC/Plan_Library/*.cpp")
list(APPEND Plans_SOURCES ${PLSOURCES} ${Plans_SOURCES})
FILE(GLOB_RECURSE QtHeaders RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "interface/*.h")
FILE(GLOB_RECURSE QtSources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "interface/*.cpp")
FILE(GLOB_RECURSE QtForms RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "interface/*.ui")
FILE(GLOB_RECURSE QtRessources RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "ressources/*.qrc")

list(APPEND Activities_HEADERS ${ActivitiesCNN_HEADERS} ${ActivitiesVideoSources_HEADERS} ${QtHeaders})
list(APPEND Activities_SOURCES ${ActivitiesCNN_SOURCES} ${ActivitiesVideoSources_SOURCES} ${QtSources})
list(APPEND HEADERS ${solver_HEADERS} ${PLHEADERS})
list(APPEND SOURCES ${solver_SOURCES} ${PLSOURCES})

list(REMOVE_ITEM QtHeaders "interface/configuration.h")

FIND_PACKAGE(Qt5LinguistTools)
qt5_add_translation(QM_FILES ressources/languages/lang_en.ts)
add_custom_target(translations DEPENDS ${QM_FILES})
qt5_create_translation(QM_FILES
     interface/fenetreparametres.cpp interface/fenetreparametres.ui interface/fenetreprincipale.cpp interface/fenetreprincipale.ui
     ressources/languages/lang_en.ts
 )

# Using a source directory:
#qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} lang_fr.ts lang_en.ts)

# The qm files are generated in the build tree, but the qrc file is inside the
# source directory and fthe path to resources are relative to the location of
# the qrc file itself. We use cdonfigure_file() to copy the qrc file in the build
# directory such that it can find the qm translations files. The qrc file is
# copied if it doesn't exist in the destination or if it is modified.
#configure_file("ressources/translations.qrc" ${CMAKE_BINARY_DIR} COPYONLY)
FILE(GLOB_RECURSE QTFen RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "interface/*.ui")
list(APPEND QTFen "interface/fenetreparametres.cpp")
list(APPEND QTFen "interface/fenetreprincipale.cpp")


FILE(GLOB TS_FILES /resources/languages/lang_en.ts)
#QT5_create_translation(QM_FILES ${QTFen} ${TS_FILES})


QT5_WRAP_CPP(header_mocs ${QtHeaders})
QT5_WRAP_UI(form_headers ${QtForms})
qt5_add_resources(RCC_SOURCES ${QtRessources})
add_executable(${CMAKE_PROJECT_NAME} main.cpp  ${Activities_HEADERS} ${Activities_SOURCES} ${Plans_HEADERS} ${Plans_SOURCES} ${header_mocs} ${form_headers} ${RCC_SOURCES} ${QM_FILES})

TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${OpenCV_LIBS})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${realsense2_LIBRARIES} -lrealsense2)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${Protobuf_LIBRARIES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} ${PYTHON_LIBRARIES})
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} libdarknet)
TARGET_LINK_LIBRARIES(${CMAKE_PROJECT_NAME} Qt5::Widgets)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE /opt/darknet/include)

