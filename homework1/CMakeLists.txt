cmake_minimum_required(VERSION 3.10)
project(homework1)

find_package(OpenCV REQUIRED)

set(CMAKE_CXX_STANDARD 17)

include_directories(/usr/local/include)

add_executable(homework1 main.cpp rasterizer.hpp rasterizer.cpp Triangle.hpp Triangle.cpp)
target_link_libraries(homework1 ${OpenCV_LIBRARIES})
