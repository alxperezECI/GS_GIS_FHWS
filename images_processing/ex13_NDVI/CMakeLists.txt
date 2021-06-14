cmake_minimum_required(VERSION 3.1)
project( Example_13 )
find_package( OpenCV 2.4  REQUIRED )
set (CMAKE_CXX_STANDARD 11)
add_executable( gpuNDVI main_one.cpp )
target_link_libraries( gpuNDVI ${OpenCV_LIBS} )

