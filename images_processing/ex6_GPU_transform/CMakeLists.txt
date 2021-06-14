cmake_minimum_required(VERSION 3.1)
project( Example_6 )
find_package( OpenCV REQUIRED )
set (CMAKE_CXX_STANDARD 11)

add_executable( gpuTransform_one main_one.cpp )
target_link_libraries( gpuTransform_one ${OpenCV_LIBS} )

add_executable( gpuTransform_all main_all.cpp )
target_link_libraries( gpuTransform_all ${OpenCV_LIBS} )
