cmake_minimum_required(VERSION 3.1)
project( Example_9 )
find_package( OpenCV REQUIRED )
set (CMAKE_CXX_STANDARD 11)
add_executable( cpuCanny main_one.cpp )
target_link_libraries( cpuCanny ${OpenCV_LIBS} )
