#!/bin/bash

cgal_create_cmake_script && echo 'set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")' >> CMakeLists.txt && cmake . && make
