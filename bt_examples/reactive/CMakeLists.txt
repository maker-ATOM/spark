cmake_minimum_required(VERSION 3.16.3)

project(reactive)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(behaviortree_cpp)

add_executable(reactive "reactive.cpp")
target_link_libraries(reactive BT::behaviortree_cpp)