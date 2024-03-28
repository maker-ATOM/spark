cmake_minimum_required(VERSION 3.16.3)

project(port_blackboard)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(behaviortree_cpp)

add_executable(port_blackboard "port_blackboard.cpp")
target_link_libraries(port_blackboard BT::behaviortree_cpp)