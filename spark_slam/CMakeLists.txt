cmake_minimum_required(VERSION 3.0.2)
project(spark_slam)

find_package(catkin REQUIRED COMPONENTS
  rospy
  std_srvs
  spark_navigation
)

include_directories(
  ${catkin_INCLUDE_DIRS}
)