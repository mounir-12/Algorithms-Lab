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
CMAKE_SOURCE_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10"

# Include any dependencies generated for this target.
include CMakeFiles/goldeneye.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/goldeneye.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/goldeneye.dir/flags.make

CMakeFiles/goldeneye.dir/goldeneye.cpp.o: CMakeFiles/goldeneye.dir/flags.make
CMakeFiles/goldeneye.dir/goldeneye.cpp.o: goldeneye.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/goldeneye.dir/goldeneye.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/goldeneye.dir/goldeneye.cpp.o -c "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10/goldeneye.cpp"

CMakeFiles/goldeneye.dir/goldeneye.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/goldeneye.dir/goldeneye.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10/goldeneye.cpp" > CMakeFiles/goldeneye.dir/goldeneye.cpp.i

CMakeFiles/goldeneye.dir/goldeneye.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/goldeneye.dir/goldeneye.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10/goldeneye.cpp" -o CMakeFiles/goldeneye.dir/goldeneye.cpp.s

CMakeFiles/goldeneye.dir/goldeneye.cpp.o.requires:

.PHONY : CMakeFiles/goldeneye.dir/goldeneye.cpp.o.requires

CMakeFiles/goldeneye.dir/goldeneye.cpp.o.provides: CMakeFiles/goldeneye.dir/goldeneye.cpp.o.requires
	$(MAKE) -f CMakeFiles/goldeneye.dir/build.make CMakeFiles/goldeneye.dir/goldeneye.cpp.o.provides.build
.PHONY : CMakeFiles/goldeneye.dir/goldeneye.cpp.o.provides

CMakeFiles/goldeneye.dir/goldeneye.cpp.o.provides.build: CMakeFiles/goldeneye.dir/goldeneye.cpp.o


# Object files for target goldeneye
goldeneye_OBJECTS = \
"CMakeFiles/goldeneye.dir/goldeneye.cpp.o"

# External object files for target goldeneye
goldeneye_EXTERNAL_OBJECTS =

goldeneye: CMakeFiles/goldeneye.dir/goldeneye.cpp.o
goldeneye: CMakeFiles/goldeneye.dir/build.make
goldeneye: /usr/lib/x86_64-linux-gnu/libmpfr.so
goldeneye: /usr/lib/x86_64-linux-gnu/libgmp.so
goldeneye: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
goldeneye: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
goldeneye: /usr/lib/x86_64-linux-gnu/libboost_thread.so
goldeneye: /usr/lib/x86_64-linux-gnu/libboost_system.so
goldeneye: /usr/lib/x86_64-linux-gnu/libpthread.so
goldeneye: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
goldeneye: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
goldeneye: /usr/lib/x86_64-linux-gnu/libboost_thread.so
goldeneye: /usr/lib/x86_64-linux-gnu/libboost_system.so
goldeneye: /usr/lib/x86_64-linux-gnu/libpthread.so
goldeneye: CMakeFiles/goldeneye.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable goldeneye"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/goldeneye.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/goldeneye.dir/build: goldeneye

.PHONY : CMakeFiles/goldeneye.dir/build

CMakeFiles/goldeneye.dir/requires: CMakeFiles/goldeneye.dir/goldeneye.cpp.o.requires

.PHONY : CMakeFiles/goldeneye.dir/requires

CMakeFiles/goldeneye.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/goldeneye.dir/cmake_clean.cmake
.PHONY : CMakeFiles/goldeneye.dir/clean

CMakeFiles/goldeneye.dir/depend:
	cd "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week10/CMakeFiles/goldeneye.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/goldeneye.dir/depend

