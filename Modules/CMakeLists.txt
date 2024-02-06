
aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/src MODULES_SRC)

add_library(Modules STATIC ${MODULES_SRC})
target_include_directories(Modules
        PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(Modules
        PRIVATE
    opencv_core
    opencv_videoio
)