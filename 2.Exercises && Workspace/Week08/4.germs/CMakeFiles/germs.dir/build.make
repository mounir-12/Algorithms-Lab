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
CMAKE_SOURCE_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs"

# Include any dependencies generated for this target.
include CMakeFiles/germs.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/germs.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/germs.dir/flags.make

CMakeFiles/germs.dir/germs.cpp.o: CMakeFiles/germs.dir/flags.make
CMakeFiles/germs.dir/germs.cpp.o: germs.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/germs.dir/germs.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/germs.dir/germs.cpp.o -c "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs/germs.cpp"

CMakeFiles/germs.dir/germs.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/germs.dir/germs.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs/germs.cpp" > CMakeFiles/germs.dir/germs.cpp.i

CMakeFiles/germs.dir/germs.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/germs.dir/germs.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs/germs.cpp" -o CMakeFiles/germs.dir/germs.cpp.s

CMakeFiles/germs.dir/germs.cpp.o.requires:

.PHONY : CMakeFiles/germs.dir/germs.cpp.o.requires

CMakeFiles/germs.dir/germs.cpp.o.provides: CMakeFiles/germs.dir/germs.cpp.o.requires
	$(MAKE) -f CMakeFiles/germs.dir/build.make CMakeFiles/germs.dir/germs.cpp.o.provides.build
.PHONY : CMakeFiles/germs.dir/germs.cpp.o.provides

CMakeFiles/germs.dir/germs.cpp.o.provides.build: CMakeFiles/germs.dir/germs.cpp.o


# Object files for target germs
germs_OBJECTS = \
"CMakeFiles/germs.dir/germs.cpp.o"

# External object files for target germs
germs_EXTERNAL_OBJECTS =

germs: CMakeFiles/germs.dir/germs.cpp.o
germs: CMakeFiles/germs.dir/build.make
germs: /usr/lib/x86_64-linux-gnu/libmpfr.so
germs: /usr/lib/x86_64-linux-gnu/libgmp.so
germs: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
germs: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
germs: /usr/lib/x86_64-linux-gnu/libboost_thread.so
germs: /usr/lib/x86_64-linux-gnu/libboost_system.so
germs: /usr/lib/x86_64-linux-gnu/libpthread.so
germs: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
germs: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
germs: /usr/lib/x86_64-linux-gnu/libboost_thread.so
germs: /usr/lib/x86_64-linux-gnu/libboost_system.so
germs: /usr/lib/x86_64-linux-gnu/libpthread.so
germs: CMakeFiles/germs.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable germs"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/germs.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/germs.dir/build: germs

.PHONY : CMakeFiles/germs.dir/build

CMakeFiles/germs.dir/requires: CMakeFiles/germs.dir/germs.cpp.o.requires

.PHONY : CMakeFiles/germs.dir/requires

CMakeFiles/germs.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/germs.dir/cmake_clean.cmake
.PHONY : CMakeFiles/germs.dir/clean

CMakeFiles/germs.dir/depend:
	cd "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week08/4.germs/CMakeFiles/germs.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/germs.dir/depend

