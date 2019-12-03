#include <stdio.h>
#include <iostream>
#include <cstddef>
#include <map>
#include <cuda.h>

#include "openacc.h"
#include "cuda/cuda_helper.h"
#include "oaru.h"

// Global variables
CUdevice device;
CUcontext context;


typedef struct alloc_info {
  h_void* hostPtr;
  d_void* devPtr;
  CUdeviceptr cuPtr;
  size_t len;
} AllocInfo;

typedef std::map<const void *, AllocInfo> PresentTable;

PresentTable present_;

std::ostream& print_alloc_info(std::ostream& os, const AllocInfo* info) {
  return os << " h:" << info->hostPtr << " d:" << info->devPtr << 
      " size:" << info->len;
}

/**
 */
void dump_present_table() {
  std::cout << "Present table dump: (" << present_.size() << ")" << std::endl;
  for (auto const &e : present_) {
    std::cout << print_alloc_info(std::cout, &e.second) << std::endl;
  }
}

void acc_init(acc_device_t devicetype) {
  int deviceCount = 0;
  CUresult cuResult = cuInit(0);

  int major = 0, minor = 0;
  if (cuResult != CUDA_SUCCESS) {
    std::cerr << "[ERROR] Cannot call cuInit" << std::endl;
    exit(1);
  }

  cuResult = cuDeviceGetCount(&deviceCount);
  if (deviceCount == 0) {
    std::cerr << "[ERROR] no devices supporting CUDA" << std::endl;
    exit(1);
  }
  std::cout << "[INFO] Found " << deviceCount << " CUDA device(s)" << std::endl;

  cuResult = cuDeviceGet(&device, 0);
  if (cuResult != CUDA_SUCCESS) {
    std::cerr << "Error: cannot get device 0" << std::endl;
    exit(1);
  }

  char name[100];
  cuResult = cuDeviceGetName(name, 100, device);
  std::cout << "> Using device 0: " << name << std::endl;

  cuResult = cuDeviceGetAttribute(
      &major, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, device);
  if (cuResult != CUDA_SUCCESS) {
    std::cerr << "Error: cannot get device capabilities" << std::endl;
    exit(1);
  }

  cuResult = cuDeviceGetAttribute(
      &minor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, device);
  if (cuResult != CUDA_SUCCESS) {
    std::cerr << "[ERROR] Cannot get device capabilities" << std::endl;
    exit(1);
  }
  std::cout << "> GPU Device has SM " << major << "." << minor << 
      " compute capability" << std::endl;

  cuResult = cuCtxCreate(&context, 0, device);
  if (cuResult != CUDA_SUCCESS) {
    fprintf(stderr, "* Error initializing the CUDA context.\n");
    cuCtxDestroy(context);
    exit(1);
  }
}

void acc_shutdown(acc_device_t devicetype) { cuCtxDestroy(context); }

d_void *acc_create(h_void *hostPtr, size_t size) {
  void *devPtr;
  if (present_.find(hostPtr) == present_.end()) {
    CUresult cuResult = CUDA_SUCCESS;
    CUdeviceptr cuPtr;
    cuResult = cuMemAlloc(&cuPtr, size);
    if (cuResult == CUDA_SUCCESS) {
      devPtr = (void*) (uintptr_t) cuPtr;
      AllocInfo crt = { hostPtr, devPtr, cuPtr, size };
      present_[hostPtr] = crt;
      std::cout << "[INFO] allocate memory on device" << 
          print_alloc_info(std::cout, &crt) << std::endl;
      return devPtr;
    } else {
      std::cerr << "[ERROR] Cannot allocate on device" << std::endl;
      print_cuda_error(cuResult);
      dump_present_table();
      exit(1);
    }
  } else {
    std::cout << "[INFO] Already present on device" << std::endl;
    return present_[hostPtr].devPtr;
  }
  return NULL;
}

void acc_delete(h_void *hostPtr, size_t len) {
  if (present_.find(hostPtr) != present_.end()) {
    CUresult cuResult = CUDA_SUCCESS;
    cuResult = cuMemFree(present_[hostPtr].cuPtr);
    if (cuResult != CUDA_SUCCESS) {
      std::cerr << "[ERROR] Cannot free CUDA memory" << std::endl;
      print_cuda_error(cuResult);
      dump_present_table();
      exit(1);
    }
    present_.erase(hostPtr);
  }
}

void acc_update_device(h_void *hostPtr, size_t len) {
  if (present_.find(hostPtr) != present_.end()) {
    CUresult cuResult = CUDA_SUCCESS;
    cuResult = cuMemcpyHtoD(present_[hostPtr].cuPtr, hostPtr, len);
    if (cuResult != CUDA_SUCCESS) {
      std::cerr << "[ERROR] updating device memory - " << 
          print_alloc_info(std::cerr, &present_[hostPtr]) << std::endl;
      print_cuda_error(cuResult);
      exit(1);
    }
  }
}

void acc_update_self(h_void *hostPtr, size_t len) {
  if (present_.find(hostPtr) != present_.end()) {
    CUresult cuResult = CUDA_SUCCESS;
    cuResult = cuMemcpyDtoH(hostPtr, present_[hostPtr].cuPtr, len);
    if (cuResult != CUDA_SUCCESS) {
      std::cerr << "[ERROR] updating host memory" << std::endl;
      print_cuda_error(cuResult);
      exit(1);
    }
  }
}

int acc_is_present(h_void *hostPtr, size_t len) {
  if (present_.find(hostPtr) != present_.end()) {
    return 1; // TODO check for len
  }
  return 0;
}
