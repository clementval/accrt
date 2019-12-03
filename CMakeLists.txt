cmake_minimum_required(VERSION 3.10)

project(accrt LANGUAGES CUDA C CXX)

find_package(CUDA 10.0 REQUIRED)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(openacc
  src/openacc.cpp
  src/cuda/cuda_helper.cpp
)

set(OPENACC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(
  openacc
  PRIVATE
  ${OPENACC_INCLUDE_DIR}
  ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
)

add_subdirectory(test)