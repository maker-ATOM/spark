# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/aditya/spark_ws/src/bt_examples/bt_demo

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/aditya/spark_ws/src/bt_examples/bt_demo/build

# Include any dependencies generated for this target.
include CMakeFiles/bt_demo.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bt_demo.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bt_demo.dir/flags.make

CMakeFiles/bt_demo.dir/bt_demo.cpp.o: CMakeFiles/bt_demo.dir/flags.make
CMakeFiles/bt_demo.dir/bt_demo.cpp.o: ../bt_demo.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/aditya/spark_ws/src/bt_examples/bt_demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/bt_demo.dir/bt_demo.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/bt_demo.dir/bt_demo.cpp.o -c /home/aditya/spark_ws/src/bt_examples/bt_demo/bt_demo.cpp

CMakeFiles/bt_demo.dir/bt_demo.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/bt_demo.dir/bt_demo.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/aditya/spark_ws/src/bt_examples/bt_demo/bt_demo.cpp > CMakeFiles/bt_demo.dir/bt_demo.cpp.i

CMakeFiles/bt_demo.dir/bt_demo.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/bt_demo.dir/bt_demo.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/aditya/spark_ws/src/bt_examples/bt_demo/bt_demo.cpp -o CMakeFiles/bt_demo.dir/bt_demo.cpp.s

# Object files for target bt_demo
bt_demo_OBJECTS = \
"CMakeFiles/bt_demo.dir/bt_demo.cpp.o"

# External object files for target bt_demo
bt_demo_EXTERNAL_OBJECTS =

bt_demo: CMakeFiles/bt_demo.dir/bt_demo.cpp.o
bt_demo: CMakeFiles/bt_demo.dir/build.make
bt_demo: /usr/local/lib/libbehaviortree_cpp.so
bt_demo: CMakeFiles/bt_demo.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/aditya/spark_ws/src/bt_examples/bt_demo/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable bt_demo"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bt_demo.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bt_demo.dir/build: bt_demo

.PHONY : CMakeFiles/bt_demo.dir/build

CMakeFiles/bt_demo.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bt_demo.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bt_demo.dir/clean

CMakeFiles/bt_demo.dir/depend:
	cd /home/aditya/spark_ws/src/bt_examples/bt_demo/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/aditya/spark_ws/src/bt_examples/bt_demo /home/aditya/spark_ws/src/bt_examples/bt_demo /home/aditya/spark_ws/src/bt_examples/bt_demo/build /home/aditya/spark_ws/src/bt_examples/bt_demo/build /home/aditya/spark_ws/src/bt_examples/bt_demo/build/CMakeFiles/bt_demo.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bt_demo.dir/depend

