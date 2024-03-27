cmake_minimum_required(VERSION 3.16.3)

project(bt_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(behaviortree_cpp)

add_executable(bt_demo "bt_demo.cpp")
target_link_libraries(bt_demo BT::behaviortree_cpp)