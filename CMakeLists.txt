cmake_minimum_required(VERSION 3.27)

include(FetchContent)

message("-- Setting up VCPKG package manager tool")
FetchContent_Declare(
    vcpkg
    GIT_REPOSITORY https://github.com/microsoft/vcpkg.git
    GIT_TAG        53bef8994c541b6561884a8395ea35715ece75db
)
FetchContent_MakeAvailable(vcpkg)

set(CMAKE_TOOLCHAIN_FILE ${vcpkg_SOURCE_DIR}/scripts/buildsystems/vcpkg.cmake)

project(cpp_h264_realtime_encoding)

set(CMAKE_CXX_STANDARD 20)

find_package(OpenCV CONFIG REQUIRED)
find_package(OpenH264 CONFIG REQUIRED)

add_executable(h264-realtime-encoding
    main.cpp
    Camera.ixx
    VideoFrame.ixx
    Encoder.ixx
)

target_link_libraries(h264-realtime-encoding
        PRIVATE
    opencv_ml
    opencv_dnn
    opencv_core
    opencv_flann
    opencv_videoio
)