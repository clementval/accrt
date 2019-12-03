
#ifndef OARU_DEVICE_MANAGER_H_
#define OARU_DEVICE_MANAGER_H_

#include <map>
#include "openacc.h"
#include <cuda.h>

typedef struct alloc_info {
  void* hostPtr;
  void* devPtr;
  CUdeviceptr cuPtr;
  size_t size;
} AllocInfo;

typedef std::map<const void *, AllocInfo> PresentTable;

enum DataMovementDirection {
    HOST_TO_DEVICE = 0,
    DEVICE_TO_HOST = 1,
    HOST_TO_HOST = 2,
    DEVICE_TO_DEVICE = 3
};

class DeviceManager {
protected:
    PresentTable presentTable_;
    acc_device_t currentDeviceType_;

    void dump_present_table();

public:
    virtual void init() = 0;
    virtual void destroy() = 0;
    virtual int get_num_devices() = 0;
    virtual void* allocate(void *hostPtr, size_t size) = 0;
    virtual void memcpy(void* hostPtr, size_t len, DataMovementDirection direction) = 0;
    virtual void free(void* hostPtr) = 0;
    virtual int is_present(void* hostPtr, size_t len) = 0;
    virtual void* get_device_ptr(void* hostPtr) = 0;
    virtual acc_device_t get_device_type() = 0;
};

#endif // OARU_DEVICE_MANAGER_H_