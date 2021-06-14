cmake_minimum_required(VERSION 3.1)
project( Example_8 )
find_package( OpenCV REQUIRED )
set (CMAKE_CXX_STANDARD 11)
add_executable( cpuBorders main_one.cpp )
target_link_libraries( cpuBorders ${OpenCV_LIBS} )

add_executable( cpuBorders_all main_one.cpp )
target_link_libraries( cpuBorders_all ${OpenCV_LIBS} )

