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
CMAKE_SOURCE_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09"

# Include any dependencies generated for this target.
include CMakeFiles/suez.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/suez.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/suez.dir/flags.make

CMakeFiles/suez.dir/suez.cpp.o: CMakeFiles/suez.dir/flags.make
CMakeFiles/suez.dir/suez.cpp.o: suez.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/suez.dir/suez.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/suez.dir/suez.cpp.o -c "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09/suez.cpp"

CMakeFiles/suez.dir/suez.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/suez.dir/suez.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09/suez.cpp" > CMakeFiles/suez.dir/suez.cpp.i

CMakeFiles/suez.dir/suez.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/suez.dir/suez.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09/suez.cpp" -o CMakeFiles/suez.dir/suez.cpp.s

CMakeFiles/suez.dir/suez.cpp.o.requires:

.PHONY : CMakeFiles/suez.dir/suez.cpp.o.requires

CMakeFiles/suez.dir/suez.cpp.o.provides: CMakeFiles/suez.dir/suez.cpp.o.requires
	$(MAKE) -f CMakeFiles/suez.dir/build.make CMakeFiles/suez.dir/suez.cpp.o.provides.build
.PHONY : CMakeFiles/suez.dir/suez.cpp.o.provides

CMakeFiles/suez.dir/suez.cpp.o.provides.build: CMakeFiles/suez.dir/suez.cpp.o


# Object files for target suez
suez_OBJECTS = \
"CMakeFiles/suez.dir/suez.cpp.o"

# External object files for target suez
suez_EXTERNAL_OBJECTS =

suez: CMakeFiles/suez.dir/suez.cpp.o
suez: CMakeFiles/suez.dir/build.make
suez: /usr/lib/x86_64-linux-gnu/libmpfr.so
suez: /usr/lib/x86_64-linux-gnu/libgmp.so
suez: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
suez: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
suez: /usr/lib/x86_64-linux-gnu/libboost_thread.so
suez: /usr/lib/x86_64-linux-gnu/libboost_system.so
suez: /usr/lib/x86_64-linux-gnu/libpthread.so
suez: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
suez: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
suez: /usr/lib/x86_64-linux-gnu/libboost_thread.so
suez: /usr/lib/x86_64-linux-gnu/libboost_system.so
suez: /usr/lib/x86_64-linux-gnu/libpthread.so
suez: CMakeFiles/suez.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable suez"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/suez.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/suez.dir/build: suez

.PHONY : CMakeFiles/suez.dir/build

CMakeFiles/suez.dir/requires: CMakeFiles/suez.dir/suez.cpp.o.requires

.PHONY : CMakeFiles/suez.dir/requires

CMakeFiles/suez.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/suez.dir/cmake_clean.cmake
.PHONY : CMakeFiles/suez.dir/clean

CMakeFiles/suez.dir/depend:
	cd "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/3.PotW/Week09/CMakeFiles/suez.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/suez.dir/depend
