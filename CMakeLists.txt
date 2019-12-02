cmake_minimum_required(VERSION 3.10)

project(accrt LANGUAGES CUDA C CXX)

find_package(CUDA 10.0 REQUIRED)

add_library(accrt
  src/openacc.cpp
)

target_include_directories(
  accrt
  PRIVATE
  include
  ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
)

add_executable(basic
  test/basic.cpp)

target_link_libraries(basic accrt cuda)
target_include_directories(basic
  PRIVATE
  include
)
