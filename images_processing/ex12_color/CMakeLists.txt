cmake_minimum_required(VERSION 3.1)
project( Example_12 )
find_package( OpenCV REQUIRED )
set (CMAKE_CXX_STANDARD 11)
add_executable( cpuColor main_one.cpp )
target_link_libraries( cpuColor ${OpenCV_LIBS} )

