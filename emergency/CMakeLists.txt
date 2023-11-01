cmake_minimum_required(VERSION 3.0.2)
project(emergency)

find_package(catkin REQUIRED COMPONENTS
  ackermann_msgs
  actionlib
  cv_bridge
  darknet_ros_msgs
  geometry_msgs
  image_transport
  message_generation
  nav_msgs
  nodelet
  roscpp
  rospy
  std_msgs
  tf
)

catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES local_path
  CATKIN_DEPENDS   ackermann_msgs actionlib cv_bridge darknet_ros_msgs geometry_msgs image_transport nav_msgs nodelet roscpp rospy std_msgs tf
#  DEPENDS system_lib
)

include_directories(
	include
	${catkin_INCLUDE_DIRS}
)


add_executable(${PROJECT_NAME} 
src/emergency.cpp

include/emergency.h
)

target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES} ${OpenCV_LIBS})


