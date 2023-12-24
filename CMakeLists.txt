cmake_minimum_required(VERSION 3.25)

project(sorting_vis)

set(CMAKE_CXX_STANDARD 17)

find_package(fmt)

add_executable(sorting_vis
        src/Geometry.h
        src/GeometryIO.h
        src/Sorting.h

        src/main.cpp
)

target_link_libraries(sorting_vis fmt::fmt)
