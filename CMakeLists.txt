# Aruco root

cmake_minimum_required(VERSION 3.1..3.14)
project( DetectAruco )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

#set(CMAKE_CXX_FLAGS "-mfpu=neon -mfloat-abi=hard -march=armv7-a -D__NEON__ ${CMAKE_CXX_FLAGS}")

add_subdirectory(src)