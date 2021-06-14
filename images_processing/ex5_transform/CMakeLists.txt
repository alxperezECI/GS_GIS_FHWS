cmake_minimum_required(VERSION 3.1)
project( Example_5 )
find_package( OpenCV REQUIRED )
set (CMAKE_CXX_STANDARD 11)
add_executable( transform main_one.cpp )
target_link_libraries( transform ${OpenCV_LIBS} )

add_executable( transform_all main_all.cpp )
target_link_libraries( transform_all ${OpenCV_LIBS} )
