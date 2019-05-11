# Aruco root

cmake_minimum_required(VERSION 3.1..3.14)
project( DetectAruco )
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_subdirectory(src)