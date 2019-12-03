
#ifndef OARU_CUDA_DEVICE_MANAGER_H_
#define OARU_CUDA_DEVICE_MANAGER_H_

#include <cuda.h>
#include <map>

void print_cuda_error(CUresult cuResult);

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
private:
    CUdevice device_;
    CUcontext context_;
    PresentTable presentTable_;
    void dump_present_table();

public:
    void init();
    void destroy();
    int get_num_devices();
    void* allocate(void *hostPtr, size_t size);
    void memcpy(void* hostPtr, size_t len, DataMovementDirection direction);
    void free(void* hostPtr);
    int is_present(void* hostPtr, size_t len);
    void* get_device_ptr(void* hostPtr);  
};

#endif // OARU_CUDA_DEVICE_MANAGER_H_