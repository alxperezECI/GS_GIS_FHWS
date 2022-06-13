cmake_minimum_required(VERSION 3.1)
project( Example_5 )

find_package( OpenCV REQUIRED )
set (CMAKE_CXX_STANDARD 11)

#These two lines configure and create the lines to build an application
#that process a file passed by argument from command line.
add_executable( transform main_one.cpp )
target_link_libraries( transform ${OpenCV_LIBS} )

#These lines configure the project to create an application that looking
#for all files inside a folder to process them.
add_executable( transform_all main_all.cpp )
target_link_libraries( transform_all ${OpenCV_LIBS} )
