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
CMAKE_SOURCE_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet"

# Include any dependencies generated for this target.
include CMakeFiles/diet.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/diet.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/diet.dir/flags.make

CMakeFiles/diet.dir/diet.cpp.o: CMakeFiles/diet.dir/flags.make
CMakeFiles/diet.dir/diet.cpp.o: diet.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/diet.dir/diet.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/diet.dir/diet.cpp.o -c "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet/diet.cpp"

CMakeFiles/diet.dir/diet.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/diet.dir/diet.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet/diet.cpp" > CMakeFiles/diet.dir/diet.cpp.i

CMakeFiles/diet.dir/diet.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/diet.dir/diet.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet/diet.cpp" -o CMakeFiles/diet.dir/diet.cpp.s

CMakeFiles/diet.dir/diet.cpp.o.requires:

.PHONY : CMakeFiles/diet.dir/diet.cpp.o.requires

CMakeFiles/diet.dir/diet.cpp.o.provides: CMakeFiles/diet.dir/diet.cpp.o.requires
	$(MAKE) -f CMakeFiles/diet.dir/build.make CMakeFiles/diet.dir/diet.cpp.o.provides.build
.PHONY : CMakeFiles/diet.dir/diet.cpp.o.provides

CMakeFiles/diet.dir/diet.cpp.o.provides.build: CMakeFiles/diet.dir/diet.cpp.o


# Object files for target diet
diet_OBJECTS = \
"CMakeFiles/diet.dir/diet.cpp.o"

# External object files for target diet
diet_EXTERNAL_OBJECTS =

diet: CMakeFiles/diet.dir/diet.cpp.o
diet: CMakeFiles/diet.dir/build.make
diet: /usr/lib/x86_64-linux-gnu/libmpfr.so
diet: /usr/lib/x86_64-linux-gnu/libgmp.so
diet: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
diet: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
diet: /usr/lib/x86_64-linux-gnu/libboost_thread.so
diet: /usr/lib/x86_64-linux-gnu/libboost_system.so
diet: /usr/lib/x86_64-linux-gnu/libpthread.so
diet: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
diet: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
diet: /usr/lib/x86_64-linux-gnu/libboost_thread.so
diet: /usr/lib/x86_64-linux-gnu/libboost_system.so
diet: /usr/lib/x86_64-linux-gnu/libpthread.so
diet: CMakeFiles/diet.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable diet"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/diet.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/diet.dir/build: diet

.PHONY : CMakeFiles/diet.dir/build

CMakeFiles/diet.dir/requires: CMakeFiles/diet.dir/diet.cpp.o.requires

.PHONY : CMakeFiles/diet.dir/requires

CMakeFiles/diet.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/diet.dir/cmake_clean.cmake
.PHONY : CMakeFiles/diet.dir/clean

CMakeFiles/diet.dir/depend:
	cd "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/2.diet/CMakeFiles/diet.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/diet.dir/depend
