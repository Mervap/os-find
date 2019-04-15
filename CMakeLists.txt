cmake_minimum_required(VERSION 3.1)
project(find)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_GLIBCXX_DEBUG -fsanitize=address,undefined")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

add_executable(find
        find.cpp
        linux_dirent64.h
        File_filter.cpp
        File_filter.h
        common_utils.h)