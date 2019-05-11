# Arucu/src 

# add_subdirectory(withrobot)

include_directories(${DetectAruco_SOURCE_DIR}/include)
include_directories(${DetectAruco_SOURCE_DIR}/src/withrobot)

# ============== CAM =================================
add_executable(  DetectAruco   cam.cpp )
set_target_properties( DetectAruco PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_link_libraries(  DetectAruco PRIVATE  ${OpenCV_LIBS} )
target_link_libraries(  DetectAruco PRIVATE pthread udev v4l2)

target_sources( DetectAruco PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/detectAruco.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/withrobot/withrobot_camera.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/withrobot/withrobot_utility.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/udp/udp.cpp
)


# =========== DISPLAY UDP ================================
add_executable(  Udp_display   udp_display.cpp )
set_target_properties( Udp_display PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_link_libraries(  Udp_display PRIVATE  ${OpenCV_LIBS} )
target_link_libraries(  Udp_display PRIVATE pthread udev v4l2)

target_sources( Udp_display PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/udp/udp.cpp
)