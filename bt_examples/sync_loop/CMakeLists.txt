cmake_minimum_required(VERSION 3.16.3)

project(sync_loop)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(behaviortree_cpp)

add_executable(sync_loop "sync_loop.cpp")
target_link_libraries(sync_loop BT::behaviortree_cpp)