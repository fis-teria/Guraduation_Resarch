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
CMAKE_SOURCE_DIR = /mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop/build

# Include any dependencies generated for this target.
include CMakeFiles/match.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/match.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/match.dir/flags.make

CMakeFiles/match.dir/src/match.cpp.o: CMakeFiles/match.dir/flags.make
CMakeFiles/match.dir/src/match.cpp.o: ../src/match.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/match.dir/src/match.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/match.dir/src/match.cpp.o -c /mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop/src/match.cpp

CMakeFiles/match.dir/src/match.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/match.dir/src/match.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop/src/match.cpp > CMakeFiles/match.dir/src/match.cpp.i

CMakeFiles/match.dir/src/match.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/match.dir/src/match.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop/src/match.cpp -o CMakeFiles/match.dir/src/match.cpp.s

# Object files for target match
match_OBJECTS = \
"CMakeFiles/match.dir/src/match.cpp.o"

# External object files for target match
match_EXTERNAL_OBJECTS =

match: CMakeFiles/match.dir/src/match.cpp.o
match: CMakeFiles/match.dir/build.make
match: CMakeFiles/match.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable match"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/match.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/match.dir/build: match

.PHONY : CMakeFiles/match.dir/build

CMakeFiles/match.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/match.dir/cmake_clean.cmake
.PHONY : CMakeFiles/match.dir/clean

CMakeFiles/match.dir/depend:
	cd /mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop /mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop /mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop/build /mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop/build /mnt/d/openCV_WorkSpace/Ohno_cmake_Desktop/build/CMakeFiles/match.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/match.dir/depend

