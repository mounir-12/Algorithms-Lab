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
CMAKE_SOURCE_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues"

# Include any dependencies generated for this target.
include CMakeFiles/clues_master.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/clues_master.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/clues_master.dir/flags.make

CMakeFiles/clues_master.dir/clues_master.cpp.o: CMakeFiles/clues_master.dir/flags.make
CMakeFiles/clues_master.dir/clues_master.cpp.o: clues_master.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/clues_master.dir/clues_master.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/clues_master.dir/clues_master.cpp.o -c "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues/clues_master.cpp"

CMakeFiles/clues_master.dir/clues_master.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/clues_master.dir/clues_master.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues/clues_master.cpp" > CMakeFiles/clues_master.dir/clues_master.cpp.i

CMakeFiles/clues_master.dir/clues_master.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/clues_master.dir/clues_master.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues/clues_master.cpp" -o CMakeFiles/clues_master.dir/clues_master.cpp.s

CMakeFiles/clues_master.dir/clues_master.cpp.o.requires:

.PHONY : CMakeFiles/clues_master.dir/clues_master.cpp.o.requires

CMakeFiles/clues_master.dir/clues_master.cpp.o.provides: CMakeFiles/clues_master.dir/clues_master.cpp.o.requires
	$(MAKE) -f CMakeFiles/clues_master.dir/build.make CMakeFiles/clues_master.dir/clues_master.cpp.o.provides.build
.PHONY : CMakeFiles/clues_master.dir/clues_master.cpp.o.provides

CMakeFiles/clues_master.dir/clues_master.cpp.o.provides.build: CMakeFiles/clues_master.dir/clues_master.cpp.o


# Object files for target clues_master
clues_master_OBJECTS = \
"CMakeFiles/clues_master.dir/clues_master.cpp.o"

# External object files for target clues_master
clues_master_EXTERNAL_OBJECTS =

clues_master: CMakeFiles/clues_master.dir/clues_master.cpp.o
clues_master: CMakeFiles/clues_master.dir/build.make
clues_master: /usr/lib/x86_64-linux-gnu/libmpfr.so
clues_master: /usr/lib/x86_64-linux-gnu/libgmp.so
clues_master: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
clues_master: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
clues_master: /usr/lib/x86_64-linux-gnu/libboost_thread.so
clues_master: /usr/lib/x86_64-linux-gnu/libboost_system.so
clues_master: /usr/lib/x86_64-linux-gnu/libpthread.so
clues_master: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
clues_master: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
clues_master: /usr/lib/x86_64-linux-gnu/libboost_thread.so
clues_master: /usr/lib/x86_64-linux-gnu/libboost_system.so
clues_master: /usr/lib/x86_64-linux-gnu/libpthread.so
clues_master: CMakeFiles/clues_master.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable clues_master"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/clues_master.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/clues_master.dir/build: clues_master

.PHONY : CMakeFiles/clues_master.dir/build

CMakeFiles/clues_master.dir/requires: CMakeFiles/clues_master.dir/clues_master.cpp.o.requires

.PHONY : CMakeFiles/clues_master.dir/requires

CMakeFiles/clues_master.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/clues_master.dir/cmake_clean.cmake
.PHONY : CMakeFiles/clues_master.dir/clean

CMakeFiles/clues_master.dir/depend:
	cd "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/2.Exercises && Workspace/Week13/3.clues/CMakeFiles/clues_master.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/clues_master.dir/depend
