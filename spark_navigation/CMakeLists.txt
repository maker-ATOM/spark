cmake_minimum_required(VERSION 3.0.2)
project(spark_navigation)

find_package(catkin REQUIRED COMPONENTS
  rospy
  std_srvs
  tf2_ros
  # spark_navigation
  message_generation
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)

add_service_files(
  FILES
  Checkpoint.srv
)

generate_messages(
  DEPENDENCIES
)

catkin_package(
  CATKIN_DEPENDS message_runtime
)

add_executable(transform src/transform.cpp)
target_link_libraries(transform ${catkin_LIBRARIES})