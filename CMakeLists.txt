cmake_minimum_required(VERSION 3.10)

project(oaru LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(ENABLE_CUDA "Enable CUDA device support" ON)


set(
  OPENACC_SOURCES
  src/openacc.cpp
)

if(ENABLE_CUDA)
  enable_language(CUDA)
  find_package(CUDA 10.0 REQUIRED)
  set(
    OPENACC_SOURCES
    ${OPENACC_SOURCES}
    src/cuda/cuda_device_manager.cpp
  )
endif()

add_library(openacc
  ${OPENACC_SOURCES}
)

set(OPENACC_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(
  openacc
  PRIVATE
  ${OPENACC_INCLUDE_DIR}
)

if(ENABLE_CUDA)
  target_include_directories(
    openacc
    PRIVATE
    ${CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES}
  )
endif()

enable_testing()
add_subdirectory(test)