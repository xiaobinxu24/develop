# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.3

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
CMAKE_COMMAND = /media/v/docs/Software/clearsoft/clion-1.2.4/bin/cmake/bin/cmake

# The command to remove a file.
RM = /media/v/docs/Software/clearsoft/clion-1.2.4/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/v/proj/work/kfrouter/kfrouter/googletest

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/v/proj/work/kfrouter/kfrouter/googletest/build

# Include any dependencies generated for this target.
include googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/depend.make

# Include the progress variables for this target.
include googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/progress.make

# Include the compile flags for this target's objects.
include googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/flags.make

# Object files for target kfnetwork-unittest
kfnetwork__unittest_OBJECTS =

# External object files for target kfnetwork-unittest
kfnetwork__unittest_EXTERNAL_OBJECTS =

googlemock/gtest/kfnetwork-unittest: googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/build.make
googlemock/gtest/kfnetwork-unittest: googlemock/gtest/libgtest_main.a
googlemock/gtest/kfnetwork-unittest: googlemock/gtest/libgtest.a
googlemock/gtest/kfnetwork-unittest: googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/v/proj/work/kfrouter/kfrouter/googletest/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Linking CXX executable kfnetwork-unittest"
	cd /home/v/proj/work/kfrouter/kfrouter/googletest/build/googlemock/gtest && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/kfnetwork-unittest.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/build: googlemock/gtest/kfnetwork-unittest

.PHONY : googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/build

googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/requires:

.PHONY : googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/requires

googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/clean:
	cd /home/v/proj/work/kfrouter/kfrouter/googletest/build/googlemock/gtest && $(CMAKE_COMMAND) -P CMakeFiles/kfnetwork-unittest.dir/cmake_clean.cmake
.PHONY : googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/clean

googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/depend:
	cd /home/v/proj/work/kfrouter/kfrouter/googletest/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/v/proj/work/kfrouter/kfrouter/googletest /home/v/proj/work/kfrouter/kfrouter/googletest/googletest /home/v/proj/work/kfrouter/kfrouter/googletest/build /home/v/proj/work/kfrouter/kfrouter/googletest/build/googlemock/gtest /home/v/proj/work/kfrouter/kfrouter/googletest/build/googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : googlemock/gtest/CMakeFiles/kfnetwork-unittest.dir/depend

