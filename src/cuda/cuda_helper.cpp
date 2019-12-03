#include "cuda/cuda_helper.h"

#include <iostream>

void print_cuda_error(CUresult cuResult) {
  switch(cuResult) {
    case CUDA_SUCCESS: 
      std::cerr << "CUDA_SUCCESS" << std::endl; 
      break;
    case CUDA_ERROR_INVALID_VALUE:
      std::cerr << "CUDA_ERROR_INVALID_VALUE" << std::endl;
      break;
    case CUDA_ERROR_OUT_OF_MEMORY:
      std::cerr << "CUDA_ERROR_OUT_OF_MEMORY" << std::endl;
      break;
    case CUDA_ERROR_NOT_INITIALIZED:
      std::cerr << "CUDA_ERROR_NOT_INITIALIZED" << std::endl;
      break;
    case CUDA_ERROR_DEINITIALIZED:
      std::cerr << "CUDA_ERROR_DEINITIALIZED" << std::endl;
      break;
    case CUDA_ERROR_NO_DEVICE:
      std::cerr << "CUDA_ERROR_NO_DEVICE" << std::endl;
      break;
    case CUDA_ERROR_INVALID_DEVICE:
      std::cerr << "CUDA_ERROR_INVALID_DEVICE" << std::endl;
      break;
    case CUDA_ERROR_INVALID_IMAGE:
      std::cerr << "CUDA_ERROR_INVALID_IMAGE" << std::endl;
      break;
    case CUDA_ERROR_INVALID_CONTEXT:
      std::cerr << "CUDA_ERROR_INVALID_CONTEXT" << std::endl;
      break;
    case CUDA_ERROR_CONTEXT_ALREADY_CURRENT:
      std::cerr << "CUDA_ERROR_CONTEXT_ALREADY_CURRENT" << std::endl;
      break;
    case CUDA_ERROR_MAP_FAILED:
      std::cerr << "CUDA_ERROR_MAP_FAILED" << std::endl;
      break;
    case CUDA_ERROR_UNMAP_FAILED:
      std::cerr << "CUDA_ERROR_UNMAP_FAILED" << std::endl;
      break;
    case CUDA_ERROR_ARRAY_IS_MAPPED:
      std::cerr << "CUDA_ERROR_ARRAY_IS_MAPPED" << std::endl;
      break;
    case CUDA_ERROR_ALREADY_MAPPED:
      std::cerr << "CUDA_ERROR_ALREADY_MAPPED" << std::endl;
      break;
    case CUDA_ERROR_NO_BINARY_FOR_GPU:
      std::cerr << "CUDA_ERROR_NO_BINARY_FOR_GPU" << std::endl;
      break;
    case CUDA_ERROR_ALREADY_ACQUIRED:
      std::cerr << "CUDA_ERROR_ALREADY_ACQUIRED" << std::endl;
      break;
    case CUDA_ERROR_NOT_MAPPED:
      std::cerr << "CUDA_ERROR_NOT_MAPPED" << std::endl;
      break;
    case CUDA_ERROR_INVALID_SOURCE:
      std::cerr << "CUDA_ERROR_INVALID_SOURCE" << std::endl;
      break;
    case CUDA_ERROR_FILE_NOT_FOUND:
      std::cerr << "CUDA_ERROR_FILE_NOT_FOUND" << std::endl;
      break;
    case CUDA_ERROR_INVALID_HANDLE:
      std::cerr << "CUDA_ERROR_INVALID_HANDLE" << std::endl;
      break;
    case CUDA_ERROR_NOT_FOUND:
      std::cerr << "CUDA_ERROR_NOT_FOUND" << std::endl;
      break;
    case CUDA_ERROR_NOT_READY:
      std::cerr << "CUDA_ERROR_NOT_READY" << std::endl;
      break;
    case CUDA_ERROR_LAUNCH_FAILED:
      std::cerr << "CUDA_ERROR_LAUNCH_FAILED" << std::endl;
      break;
    case CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES:
      std::cerr << "CUDA_ERROR_LAUNCH_OUT_OF_RESOURCES" << std::endl;
      break;
    case CUDA_ERROR_LAUNCH_TIMEOUT:
      std::cerr << "CUDA_ERROR_LAUNCH_TIMEOUT" << std::endl;
      break;
    case CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING:
      std::cerr << "CUDA_ERROR_LAUNCH_INCOMPATIBLE_TEXTURING" << std::endl;
      break;
    case CUDA_ERROR_UNKNOWN:
      std::cerr << "CUDA_ERROR_UNKNOWN" << std::endl;
      break;
    default:
      std::cerr << "Unknown error code" << std::endl;
      break;
  }
}