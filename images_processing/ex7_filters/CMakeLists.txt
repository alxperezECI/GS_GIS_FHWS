cmake_minimum_required(VERSION 3.1)
project( Example_7 )
find_package( OpenCV 2.4  REQUIRED )
set (CMAKE_CXX_STANDARD 11)
add_executable( cpuFilters main_one.cpp )
target_link_libraries( cpuFilters ${OpenCV_LIBS} )

add_executable( cpuFilters_all main_all.cpp )
target_link_libraries( cpuFilters_all ${OpenCV_LIBS} )

