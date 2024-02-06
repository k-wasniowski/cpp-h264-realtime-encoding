
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src VIDEO_SRC)

add_library(Video STATIC ${VIDEO_SRC})
target_include_directories(Video
        PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Video
        PRIVATE
    opencv_core
    opencv_videoio
)