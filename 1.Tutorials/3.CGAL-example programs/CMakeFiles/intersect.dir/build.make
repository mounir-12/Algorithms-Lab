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
CMAKE_SOURCE_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs"

# Include any dependencies generated for this target.
include CMakeFiles/intersect.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/intersect.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/intersect.dir/flags.make

CMakeFiles/intersect.dir/intersect.cpp.o: CMakeFiles/intersect.dir/flags.make
CMakeFiles/intersect.dir/intersect.cpp.o: intersect.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/intersect.dir/intersect.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/intersect.dir/intersect.cpp.o -c "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs/intersect.cpp"

CMakeFiles/intersect.dir/intersect.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/intersect.dir/intersect.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs/intersect.cpp" > CMakeFiles/intersect.dir/intersect.cpp.i

CMakeFiles/intersect.dir/intersect.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/intersect.dir/intersect.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs/intersect.cpp" -o CMakeFiles/intersect.dir/intersect.cpp.s

CMakeFiles/intersect.dir/intersect.cpp.o.requires:

.PHONY : CMakeFiles/intersect.dir/intersect.cpp.o.requires

CMakeFiles/intersect.dir/intersect.cpp.o.provides: CMakeFiles/intersect.dir/intersect.cpp.o.requires
	$(MAKE) -f CMakeFiles/intersect.dir/build.make CMakeFiles/intersect.dir/intersect.cpp.o.provides.build
.PHONY : CMakeFiles/intersect.dir/intersect.cpp.o.provides

CMakeFiles/intersect.dir/intersect.cpp.o.provides.build: CMakeFiles/intersect.dir/intersect.cpp.o


# Object files for target intersect
intersect_OBJECTS = \
"CMakeFiles/intersect.dir/intersect.cpp.o"

# External object files for target intersect
intersect_EXTERNAL_OBJECTS =

intersect: CMakeFiles/intersect.dir/intersect.cpp.o
intersect: CMakeFiles/intersect.dir/build.make
intersect: /usr/lib/x86_64-linux-gnu/libmpfr.so
intersect: /usr/lib/x86_64-linux-gnu/libgmp.so
intersect: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
intersect: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
intersect: /usr/lib/x86_64-linux-gnu/libboost_thread.so
intersect: /usr/lib/x86_64-linux-gnu/libboost_system.so
intersect: /usr/lib/x86_64-linux-gnu/libpthread.so
intersect: /usr/lib/x86_64-linux-gnu/libCGAL_Core.so.11.0.1
intersect: /usr/lib/x86_64-linux-gnu/libCGAL.so.11.0.1
intersect: /usr/lib/x86_64-linux-gnu/libboost_thread.so
intersect: /usr/lib/x86_64-linux-gnu/libboost_system.so
intersect: /usr/lib/x86_64-linux-gnu/libpthread.so
intersect: CMakeFiles/intersect.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable intersect"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/intersect.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/intersect.dir/build: intersect

.PHONY : CMakeFiles/intersect.dir/build

CMakeFiles/intersect.dir/requires: CMakeFiles/intersect.dir/intersect.cpp.o.requires

.PHONY : CMakeFiles/intersect.dir/requires

CMakeFiles/intersect.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/intersect.dir/cmake_clean.cmake
.PHONY : CMakeFiles/intersect.dir/clean

CMakeFiles/intersect.dir/depend:
	cd "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs" "/media/ubuntu/Data/1ère année/1st semester/Algorithms Lab/1.Tutorials/3.CGAL-example programs/CMakeFiles/intersect.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/intersect.dir/depend
