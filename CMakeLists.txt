cmake_minimum_required(VERSION 3.14)

project(PointCloudReconstruction LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

set(osg_path /Users/channyhuang/Documents/thirdlib/OpenSceneGraph/include)
set(osg_lib /Users/channyhuang/Documents/thirdlib/OpenSceneGraph/build/lib)
set(COMMON_PATH ./../commonClass)
set(COMMONGEOM_PATH ./../commonClass/commonGeometry)

include_directories(${osg_path})
include_directories("/usr/local/include")
include_directories(${COMMON_PATH})

set(IMGUI_PATH ./../commonClass/imgui)
include_directories("./../commonClass/imgui")

link_directories("/usr/local/lib")

link_directories("/System/Library/Frameworks")
link_directories(${osg_lib})

add_definitions(-DGLFW_STATIC)

file(GLOB ALL_SOURCE ./*.cpp)
file(GLOB ALL_HEAD ./*.h)
file(GLOB IMGUI_HEAD ${IMGUI_PATH}/*.h)
file(GLOB IMGUI_SOURCE ${IMGUI_PATH}/*.cpp)
file(GLOB COMMONGeom_FILES ${COMMONGEOM_PATH}/*.h ${COMMONGEOM_PATH}/*.cpp)

add_executable(PointCloudReconstruction
    ${ALL_SOURCE}
    ${ALL_HEAD}
    ${IMGUI_HEAD}
    ${IMGUI_SOURCE}

    ${COMMONGeom_FILES}
)

target_link_libraries(PointCloudReconstruction #[[Qt${QT_VERSION_MAJOR}::Core]]
    libglfw3.a
    libGLEW.a
    "-framework Cocoa"
    "-framework OpenGL"
    "-framework CoreVideo"
    "-framework IOKit"
    osg
    osgDB
    osgViewer
    osgUtil
    osgGA
    OpenThreads
)

install(TARGETS PointCloudReconstruction
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
