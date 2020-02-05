#include "cuda/cuda_device_manager.h"

#include <cuda.h>
#include <iostream>

#define cudaErrCheck(ans) { cudaAssert((ans), __FILE__, __LINE__); }
inline void cudaAssert(CUresult code, const char *file, int line, 
                       bool abort = true) {
  if (code != CUDA_SUCCESS) {
    const char* errstr;
    cuGetErrorString(code, &errstr);
    fprintf(stderr,"GPU error: %s %s %d\n", errstr, file, line);
    if (abort)
       exit(code);
  }
}

/**
 *
 */
int CudaDeviceManager::get_num_devices() {
  int deviceCount = 0;
  cudaErrCheck(cuDeviceGetCount(&deviceCount));
  return deviceCount;
}

/**
 *
 */
void CudaDeviceManager::init() {
  cudaErrCheck(cuInit(0));
  cudaErrCheck(cuDeviceGet(&device_, 0));
  cudaErrCheck(cuCtxCreate(&context_, 0, device_));
  presentTable_.clear();
}

/**
 *
 */
void CudaDeviceManager::destroy() { cuCtxDestroy(context_); }

/**
 *
 */
void *CudaDeviceManager::allocate(void *hostPtr, size_t size) {
  void *devPtr;
  CUdeviceptr cuPtr;
  cudaErrCheck(cuMemAlloc(&cuPtr, size));
  devPtr = (void *)(uintptr_t)cuPtr;
  CudaAllocInfo crt = { { hostPtr, devPtr, size }, cuPtr };
  presentTable_[hostPtr] = crt;
  return devPtr;
}

/**
 *
 */
void CudaDeviceManager::free(void *hostPtr) {
  cudaErrCheck(cuMemFree(presentTable_[hostPtr].cuPtr));
  presentTable_.erase(hostPtr);
}

/**
 *
 */
void CudaDeviceManager::memcpy(void *hostPtr, size_t len,
                               DataMovementDirection direction) {
  if (direction == HOST_TO_DEVICE) {
    cudaErrCheck(cuMemcpyHtoD(presentTable_[hostPtr].cuPtr, hostPtr, len));
  } else if (direction == DEVICE_TO_HOST) {
    cudaErrCheck(cuMemcpyDtoH(hostPtr, presentTable_[hostPtr].cuPtr, len));
  }
}
/**
 *
 */
int CudaDeviceManager::is_present(void *hostPtr, size_t len) {
  if (presentTable_.size() > 0 &&
      presentTable_.find(hostPtr) != presentTable_.end()) {
    return 1; // TODO check for len
  }
  return 0;
}

/**
 *
 */
void CudaDeviceManager::dump_present_table() {
  std::cout << "Present table dump: (" << presentTable_.size() << ")"
            << std::endl;
  for (auto const &e : presentTable_) {
    std::cout << " h:" << e.second.allocInfo.hostPtr
              << " d:" << e.second.allocInfo.devPtr
              << " size:" << e.second.allocInfo.size << std::endl;
  }
}

/**
 *
 */
void *CudaDeviceManager::get_device_ptr(void *hostPtr) {
  if (presentTable_.find(hostPtr) != presentTable_.end()) {
    return presentTable_[hostPtr].allocInfo.devPtr;
  }
  return NULL;
}

size_t CudaDeviceManager::get_property(int devicenum, acc_device_t devicetype,
                                       acc_device_property_t property) {
  int value = 0;
  if(property == acc_property_memory || property == acc_property_free_memory) {
    size_t free, total;
	  cudaErrCheck(cuMemGetInfo(&free, &total));
    return (property == acc_property_memory) ? total : free;
  } else if (property == acc_property_shared_memory_support) {
    cudaErrCheck(cuDeviceGetAttribute(&value,
        CU_DEVICE_ATTRIBUTE_MAX_SHARED_MEMORY_PER_BLOCK, device_));
  }
  return value;
}

const char * CudaDeviceManager::get_property_string(int devicenum, 
    acc_device_t devicetype, acc_device_property_t property) {
  CUresult cuResult = CUDA_SUCCESS;
  char *buf;
  buf = new char[256];
  if(property == acc_property_driver) {
    int version;
    cudaErrCheck(cuDriverGetVersion(&version));
    snprintf(buf, 256, "%d", version);
    return buf;
  } else if(property == acc_property_name) {
    cudaErrCheck(cuDeviceGetName (buf, 256, device_));
    return buf;
  } else if(property == acc_property_vendor) {
    return "nvidia";
  } 
  return "";
}