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
CMAKE_SOURCE_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit"

# Include any dependencies generated for this target.
include CMakeFiles/hit.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/hit.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/hit.dir/flags.make

CMakeFiles/hit.dir/hit.cpp.o: CMakeFiles/hit.dir/flags.make
CMakeFiles/hit.dir/hit.cpp.o: hit.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/hit.dir/hit.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/hit.dir/hit.cpp.o -c "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit/hit.cpp"

CMakeFiles/hit.dir/hit.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/hit.dir/hit.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit/hit.cpp" > CMakeFiles/hit.dir/hit.cpp.i

CMakeFiles/hit.dir/hit.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/hit.dir/hit.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit/hit.cpp" -o CMakeFiles/hit.dir/hit.cpp.s

CMakeFiles/hit.dir/hit.cpp.o.requires:

.PHONY : CMakeFiles/hit.dir/hit.cpp.o.requires

CMakeFiles/hit.dir/hit.cpp.o.provides: CMakeFiles/hit.dir/hit.cpp.o.requires
	$(MAKE) -f CMakeFiles/hit.dir/build.make CMakeFiles/hit.dir/hit.cpp.o.provides.build
.PHONY : CMakeFiles/hit.dir/hit.cpp.o.provides

CMakeFiles/hit.dir/hit.cpp.o.provides.build: CMakeFiles/hit.dir/hit.cpp.o


# Object files for target hit
hit_OBJECTS = \
"CMakeFiles/hit.dir/hit.cpp.o"

# External object files for target hit
hit_EXTERNAL_OBJECTS =

hit: CMakeFiles/hit.dir/hit.cpp.o
hit: CMakeFiles/hit.dir/build.make
hit: /usr/lib/x86_64-linux-gnu/libmpfr.so
hit: /usr/lib/x86_64-linux-gnu/libgmp.so
hit: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
hit: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
hit: /usr/lib/x86_64-linux-gnu/libboost_thread.so
hit: /usr/lib/x86_64-linux-gnu/libboost_system.so
hit: /usr/lib/x86_64-linux-gnu/libpthread.so
hit: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
hit: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
hit: /usr/lib/x86_64-linux-gnu/libboost_thread.so
hit: /usr/lib/x86_64-linux-gnu/libboost_system.so
hit: /usr/lib/x86_64-linux-gnu/libpthread.so
hit: CMakeFiles/hit.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable hit"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/hit.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/hit.dir/build: hit

.PHONY : CMakeFiles/hit.dir/build

CMakeFiles/hit.dir/requires: CMakeFiles/hit.dir/hit.cpp.o.requires

.PHONY : CMakeFiles/hit.dir/requires

CMakeFiles/hit.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/hit.dir/cmake_clean.cmake
.PHONY : CMakeFiles/hit.dir/clean

CMakeFiles/hit.dir/depend:
	cd "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week03/1.hit/CMakeFiles/hit.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/hit.dir/depend

