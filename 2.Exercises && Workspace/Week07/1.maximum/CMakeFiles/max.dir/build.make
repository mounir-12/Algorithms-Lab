# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_SOURCE_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum"

# Include any dependencies generated for this target.
include CMakeFiles/max.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/max.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/max.dir/flags.make

CMakeFiles/max.dir/max.cpp.o: CMakeFiles/max.dir/flags.make
CMakeFiles/max.dir/max.cpp.o: max.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/max.dir/max.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/max.dir/max.cpp.o -c "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum/max.cpp"

CMakeFiles/max.dir/max.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/max.dir/max.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum/max.cpp" > CMakeFiles/max.dir/max.cpp.i

CMakeFiles/max.dir/max.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/max.dir/max.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum/max.cpp" -o CMakeFiles/max.dir/max.cpp.s

CMakeFiles/max.dir/max.cpp.o.requires:

.PHONY : CMakeFiles/max.dir/max.cpp.o.requires

CMakeFiles/max.dir/max.cpp.o.provides: CMakeFiles/max.dir/max.cpp.o.requires
	$(MAKE) -f CMakeFiles/max.dir/build.make CMakeFiles/max.dir/max.cpp.o.provides.build
.PHONY : CMakeFiles/max.dir/max.cpp.o.provides

CMakeFiles/max.dir/max.cpp.o.provides.build: CMakeFiles/max.dir/max.cpp.o


# Object files for target max
max_OBJECTS = \
"CMakeFiles/max.dir/max.cpp.o"

# External object files for target max
max_EXTERNAL_OBJECTS =

max: CMakeFiles/max.dir/max.cpp.o
max: CMakeFiles/max.dir/build.make
max: /usr/lib/x86_64-linux-gnu/libmpfr.so
max: /usr/lib/x86_64-linux-gnu/libgmp.so
max: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
max: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
max: /usr/lib/x86_64-linux-gnu/libboost_thread.so
max: /usr/lib/x86_64-linux-gnu/libboost_system.so
max: /usr/lib/x86_64-linux-gnu/libpthread.so
max: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
max: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
max: /usr/lib/x86_64-linux-gnu/libboost_thread.so
max: /usr/lib/x86_64-linux-gnu/libboost_system.so
max: /usr/lib/x86_64-linux-gnu/libpthread.so
max: CMakeFiles/max.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable max"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/max.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/max.dir/build: max

.PHONY : CMakeFiles/max.dir/build

CMakeFiles/max.dir/requires: CMakeFiles/max.dir/max.cpp.o.requires

.PHONY : CMakeFiles/max.dir/requires

CMakeFiles/max.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/max.dir/cmake_clean.cmake
.PHONY : CMakeFiles/max.dir/clean

CMakeFiles/max.dir/depend:
	cd "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/1.maximum/CMakeFiles/max.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/max.dir/depend

