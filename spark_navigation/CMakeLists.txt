cmake_minimum_required(VERSION 3.0.2)
project(spark_navigation)

find_package(catkin REQUIRED COMPONENTS
  rospy
  std_srvs
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