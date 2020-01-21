#include <stdio.h>
#include <iostream>
#include <cstddef>

#include "openacc.h"
#include "device_manager.h"
#include "cuda/cuda_device_manager.h"
#include "oaru.h"

// Global variables
CudaDeviceManager devMgt_;

int acc_get_num_devices(acc_device_t acc_get_device_type) {
  return devMgt_.get_num_devices();
}

acc_device_t acc_get_device_type() { return devMgt_.get_device_type(); }

void acc_init(acc_device_t devicetype) { devMgt_.init(); }

void acc_shutdown(acc_device_t devicetype) { devMgt_.destroy(); }

d_void *acc_create(h_void *hostPtr, size_t size) {
  void *devPtr;
  if (devMgt_.is_present(hostPtr, size) == 0) {
    return devMgt_.allocate(hostPtr, size);
  } else {
    std::cout << "[INFO] Already present on device" << std::endl;
    return devMgt_.get_device_ptr(hostPtr);
  }
  return NULL;
}

void acc_delete(h_void *hostPtr, size_t len) {
  if (devMgt_.is_present(hostPtr, len) == 1) {
    devMgt_.free(hostPtr);
  }
}

void acc_update_device(h_void *hostPtr, size_t len) {
  if (devMgt_.is_present(hostPtr, len) == 1) {
    devMgt_.memcpy(hostPtr, len, HOST_TO_DEVICE);
  }
}

void acc_update_self(h_void *hostPtr, size_t len) {
  if (devMgt_.is_present(hostPtr, len) == 1) {
    devMgt_.memcpy(hostPtr, len, DEVICE_TO_HOST);
  }
}

int acc_is_present(h_void *hostPtr, size_t len) {
  return devMgt_.is_present(hostPtr, len);
}
