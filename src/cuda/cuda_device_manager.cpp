#include "cuda/cuda_device_manager.h"

#include <cuda.h>
#include <iostream>

std::ostream& print_alloc_info(std::ostream& os, const AllocInfo* info) {
  return os << " h:" << info->hostPtr << " d:" << info->devPtr << 
      " size:" << info->size;
}

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



/**
 * 
 */
int CudaDeviceManager::get_num_devices() {
  int deviceCount = 0;
  CUresult cuResult = cuDeviceGetCount(&deviceCount);
  if(cuResult != CUDA_SUCCESS) {
    std::cerr << "[ERROR] Cannot read number of devices" << std::endl;
  }
  return deviceCount;
}

/**
 * 
 */
void CudaDeviceManager::init() {
  CUresult cuResult = cuInit(0);

  int major = 0, minor = 0;
  if (cuResult != CUDA_SUCCESS) {
    std::cerr << "[ERROR] Cannot call cuInit" << std::endl;
    exit(1);
  }

  cuResult = cuDeviceGet(&device_, 0);
  if (cuResult != CUDA_SUCCESS) {
    std::cerr << "Error: cannot get device 0" << std::endl;
    exit(1);
  }

  // char name[100];
  // cuResult = cuDeviceGetName(name, 100, device);
  // std::cout << "> Using device 0: " << name << std::endl;

  // cuResult = cuDeviceGetAttribute(
  //     &major, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, device);
  // if (cuResult != CUDA_SUCCESS) {
  //   std::cerr << "Error: cannot get device capabilities" << std::endl;
  //   exit(1);
  // }

  // cuResult = cuDeviceGetAttribute(
  //     &minor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, device);
  // if (cuResult != CUDA_SUCCESS) {
  //   std::cerr << "[ERROR] Cannot get device capabilities" << std::endl;
  //   exit(1);
  // }
  // std::cout << "> GPU Device has SM " << major << "." << minor << 
  //     " compute capability" << std::endl;

  cuResult = cuCtxCreate(&context_, 0, device_);
  if (cuResult != CUDA_SUCCESS) {
    fprintf(stderr, "* Error initializing the CUDA context.\n");
    cuCtxDestroy(context_);
    exit(1);
  }

  presentTable_.clear();
}

/**
 * 
 */
void CudaDeviceManager::destroy() {
  cuCtxDestroy(context_);
}

/**
 *
 */
void* CudaDeviceManager::allocate(void* hostPtr, size_t size) {
  void* devPtr;
  CUresult cuResult = CUDA_SUCCESS;
  CUdeviceptr cuPtr;
  cuResult = cuMemAlloc(&cuPtr, size);
  if (cuResult == CUDA_SUCCESS) {
    devPtr = (void*) (uintptr_t) cuPtr;
    AllocInfo crt = { hostPtr, devPtr, cuPtr, size };
    presentTable_[hostPtr] = crt;
    return devPtr;
  } else {
    std::cerr << "[ERROR] Cannot allocate on device" << std::endl;
    print_cuda_error(cuResult);
    exit(1);
  }
  return NULL;
}

/**
 *
 */
void CudaDeviceManager::free(void* hostPtr) {
  CUresult cuResult = CUDA_SUCCESS;
  cuResult = cuMemFree(presentTable_[hostPtr].cuPtr);
  if (cuResult != CUDA_SUCCESS) {
    std::cerr << "[ERROR] Cannot free CUDA memory" << std::endl;
    print_cuda_error(cuResult);
    exit(1);
  }
  presentTable_.erase(hostPtr);
}

/**
 *
 */
void CudaDeviceManager::memcpy(void* hostPtr, size_t len, DataMovementDirection direction) {
  CUresult cuResult = CUDA_SUCCESS;
  
  if(direction == HOST_TO_DEVICE) {
    cuResult = cuMemcpyHtoD(presentTable_[hostPtr].cuPtr, hostPtr, len);
  } else if (direction == DEVICE_TO_HOST) {
    cuResult = cuMemcpyDtoH(hostPtr, presentTable_[hostPtr].cuPtr, len);
  }
  
  if (cuResult != CUDA_SUCCESS) {
    std::cerr << "[ERROR] updating device memory - " << 
          print_alloc_info(std::cerr, &present_[hostPtr]) << std::endl;
    print_cuda_error(cuResult);
    exit(1);
  }
}

/**
 *
 */
int CudaDeviceManager::is_present(void* hostPtr, size_t len) {
  if (presentTable_.size() > 0 && presentTable_.find(hostPtr) != presentTable_.end()) {
    return 1; // TODO check for len
  }
  return 0;
}

/**
 *
 */
void CudaDeviceManager::dump_present_table() {
  std::cout << "Present table dump: (" << presentTable_.size() << ")" << std::endl;
  for (auto const &e : presentTable_) {
    std::cout << print_alloc_info(std::cout, &e.second) << std::endl;
  }
}

/**
 *
 */
void* CudaDeviceManager::get_device_ptr(void* hostPtr) {
  if (presentTable_.find(hostPtr) != presentTable_.end()) {
    return presentTable_[hostPtr].devPtr;
  }
  return NULL;
}