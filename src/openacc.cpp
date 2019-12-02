#include <stdio.h>
#include <cstddef>
#include <map>
#include "cuda.h"

#include "openacc.h"



// Global variables
CUdevice device;
CUcontext context;
typedef std::map<const void*, std::pair<void*, std::size_t> > PresentTable;
typedef std::pair<const void*, std::size_t> PresentPair;

PresentTable present_;

void acc_init(acc_device_t devicetype) {
  int deviceCount = 0;
  CUresult err = cuInit(0);

  int major = 0, minor = 0;
  if (err != CUDA_SUCCESS) {
    fprintf(stderr, "Error: cannot call cuInit\n");
  }

  err = cuDeviceGetCount(&deviceCount);
  if (deviceCount == 0) {
    fprintf(stderr, "Error: no devices supporting CUDA\n");
    exit(1);
  }
  fprintf(stdout, "Found %d CUDA device(s)\n");

  err = cuDeviceGet(&device, 0);
  if (err != CUDA_SUCCESS) {
    fprintf(stderr, "Error: cannot get device %d\n", err);
    exit(1);
  }

  char name[100];
  cuDeviceGetName(name, 100, device);
  printf("> Using device 0: %s\n", name);

  err = cuDeviceGetAttribute(
        &major, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MAJOR, device);
  if (err != CUDA_SUCCESS) {
    fprintf(stderr, "Error: cannot get device capabilities\n");
    exit(1);
  }

  err = cuDeviceGetAttribute(
        &minor, CU_DEVICE_ATTRIBUTE_COMPUTE_CAPABILITY_MINOR, device);
  if (err != CUDA_SUCCESS) {
    fprintf(stderr, "Error: cannot get device capabilities\n");
    exit(1);
  }
  printf("> GPU Device has SM %d.%d compute capability\n", major, minor);

  err = cuCtxCreate(&context, 0, device);
  if (err != CUDA_SUCCESS) {
    fprintf(stderr, "* Error initializing the CUDA context.\n");
    cuCtxDestroy(context);
    exit(1);
  }
}

void acc_shutdown(acc_device_t devicetype) { cuCtxDestroy(context); }

void *acc_create(h_void *hostPtr, size_t size) {
  void *devPtr;

  if (present_.find(hostPtr) == present_.end()) {
    CUresult cuResult = CUDA_SUCCESS;
    cuResult = cuMemAlloc((CUdeviceptr *)devPtr, size);
    if (cuResult == CUDA_SUCCESS) {
      present_[hostPtr] = std::make_pair(devPtr, size);
      return devPtr;
    }
  } else {
    return present_[hostPtr].first; 
  }
  

  return NULL;
}

void acc_delete(h_void *hostPtr, size_t len) {
  if (present_.find(hostPtr) != present_.end()) {
    CUresult cuResult = CUDA_SUCCESS;
    cuResult = cuMemFree((CUdeviceptr)present_[hostPtr].first);
    if(cuResult != CUDA_SUCCESS) {
      fprintf(stderr, "Error freeing CUDA memory");
      exit(1);
    } 
  }
}

void acc_update_device(h_void* hostPtr, size_t len) {
  if (present_.find(hostPtr) != present_.end()) {
    CUresult cuResult = CUDA_SUCCESS;
    cuResult = cuMemcpyHtoD((CUdeviceptr)present_[hostPtr].first, hostPtr, len);
    if(cuResult != CUDA_SUCCESS) {
      fprintf(stderr, "Error updating device memory");
      exit(1);     
    }
  } 
}

void acc_update_self(h_void* hostPtr, size_t len) {
  if (present_.find(hostPtr) != present_.end()) {
    CUresult cuResult = CUDA_SUCCESS;
    cuResult = cuMemcpyDtoH(hostPtr, (CUdeviceptr)present_[hostPtr].first, len); 
    if(cuResult != CUDA_SUCCESS) {
      fprintf(stderr, "Error updating host memory");
      exit(1);
    }
  }
}
