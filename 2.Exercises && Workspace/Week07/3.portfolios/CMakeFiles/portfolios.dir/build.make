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
CMAKE_SOURCE_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios"

# Include any dependencies generated for this target.
include CMakeFiles/portfolios.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/portfolios.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/portfolios.dir/flags.make

CMakeFiles/portfolios.dir/portfolios.cpp.o: CMakeFiles/portfolios.dir/flags.make
CMakeFiles/portfolios.dir/portfolios.cpp.o: portfolios.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/portfolios.dir/portfolios.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/portfolios.dir/portfolios.cpp.o -c "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios/portfolios.cpp"

CMakeFiles/portfolios.dir/portfolios.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/portfolios.dir/portfolios.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios/portfolios.cpp" > CMakeFiles/portfolios.dir/portfolios.cpp.i

CMakeFiles/portfolios.dir/portfolios.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/portfolios.dir/portfolios.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios/portfolios.cpp" -o CMakeFiles/portfolios.dir/portfolios.cpp.s

CMakeFiles/portfolios.dir/portfolios.cpp.o.requires:

.PHONY : CMakeFiles/portfolios.dir/portfolios.cpp.o.requires

CMakeFiles/portfolios.dir/portfolios.cpp.o.provides: CMakeFiles/portfolios.dir/portfolios.cpp.o.requires
	$(MAKE) -f CMakeFiles/portfolios.dir/build.make CMakeFiles/portfolios.dir/portfolios.cpp.o.provides.build
.PHONY : CMakeFiles/portfolios.dir/portfolios.cpp.o.provides

CMakeFiles/portfolios.dir/portfolios.cpp.o.provides.build: CMakeFiles/portfolios.dir/portfolios.cpp.o


# Object files for target portfolios
portfolios_OBJECTS = \
"CMakeFiles/portfolios.dir/portfolios.cpp.o"

# External object files for target portfolios
portfolios_EXTERNAL_OBJECTS =

portfolios: CMakeFiles/portfolios.dir/portfolios.cpp.o
portfolios: CMakeFiles/portfolios.dir/build.make
portfolios: /usr/lib/x86_64-linux-gnu/libmpfr.so
portfolios: /usr/lib/x86_64-linux-gnu/libgmp.so
portfolios: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
portfolios: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
portfolios: /usr/lib/x86_64-linux-gnu/libboost_thread.so
portfolios: /usr/lib/x86_64-linux-gnu/libboost_system.so
portfolios: /usr/lib/x86_64-linux-gnu/libpthread.so
portfolios: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
portfolios: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
portfolios: /usr/lib/x86_64-linux-gnu/libboost_thread.so
portfolios: /usr/lib/x86_64-linux-gnu/libboost_system.so
portfolios: /usr/lib/x86_64-linux-gnu/libpthread.so
portfolios: CMakeFiles/portfolios.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable portfolios"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/portfolios.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/portfolios.dir/build: portfolios

.PHONY : CMakeFiles/portfolios.dir/build

CMakeFiles/portfolios.dir/requires: CMakeFiles/portfolios.dir/portfolios.cpp.o.requires

.PHONY : CMakeFiles/portfolios.dir/requires

CMakeFiles/portfolios.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/portfolios.dir/cmake_clean.cmake
.PHONY : CMakeFiles/portfolios.dir/clean

CMakeFiles/portfolios.dir/depend:
	cd "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week07/3.portfolios/CMakeFiles/portfolios.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/portfolios.dir/depend

