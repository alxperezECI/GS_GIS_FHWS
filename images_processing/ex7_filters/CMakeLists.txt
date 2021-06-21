cmake_minimum_required(VERSION 3.1)
project( Example_7 )
find_package( OpenCV  REQUIRED )
set (CMAKE_CXX_STANDARD 11)
add_executable( cpuFilters main_one.cpp )
target_link_libraries( cpuFilters ${OpenCV_LIBS} )

add_executable( gpuFilters_all main_all.cpp )
target_link_libraries( gpuFilters_all ${OpenCV_LIBS} )

