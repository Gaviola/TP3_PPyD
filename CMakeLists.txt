cmake_minimum_required(VERSION 3.28)
project(MPI)

set(CMAKE_CXX_STANDARD 17)
find_package(MPI REQUIRED)

add_executable(MPI main.cpp)

target_link_libraries(MPI PRIVATE MPI::MPI_CXX)
