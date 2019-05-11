# Arucu/src 

# add_subdirectory(withrobot)

include_directories(${DetectAruco_SOURCE_DIR}/include)
include_directories(${DetectAruco_SOURCE_DIR}/src/withrobot)

add_executable(  DetectAruco   cam.cpp )
set_target_properties( DetectAruco PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_link_libraries(  DetectAruco PRIVATE  ${OpenCV_LIBS} )
target_link_libraries(  DetectAruco PRIVATE pthread udev v4l2)

target_sources( DetectAruco PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/detectAruco.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/withrobot/withrobot_camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/withrobot/withrobot_utility.cpp
)


