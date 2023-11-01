cmake_minimum_required(VERSION 3.0.2)
project(safety_controller_to_ackermann_drive)

find_package(catkin REQUIRED COMPONENTS
  ackermann_msgs
  geometry_msgs
  message_generation
  nav_msgs
  roscpp
  rospy
  std_msgs
  tf
)

catkin_package(
  INCLUDE_DIRS include
#  LIBRARIES local_path
  CATKIN_DEPENDS geometry_msgs nav_msgs roscpp rospy std_msgs tf ackermann_msgs
#  DEPENDS system_lib
)

include_directories(
	include
	${catkin_INCLUDE_DIRS}
)


add_executable(${PROJECT_NAME} 
src/safety_controller_to_ackermann_drive.py
)

target_link_libraries(${PROJECT_NAME} ${catkin_LIBRARIES})