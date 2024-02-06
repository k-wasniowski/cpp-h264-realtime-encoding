
add_executable(App ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(App
    PRIVATE
        Modules
)