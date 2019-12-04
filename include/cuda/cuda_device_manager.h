
#ifndef OARU_CUDA_DEVICE_MANAGER_H_
#define OARU_CUDA_DEVICE_MANAGER_H_

#include <cuda.h>
#include "device_manager.h"
#include "cuda/cuda_alloc_info.h"

void print_cuda_error(CUresult cuResult);

typedef std::map<const void *, CudaAllocInfo> PresentTable;

class CudaDeviceManager : public DeviceManager {
private:
    PresentTable presentTable_;
    CUdevice device_;
    CUcontext context_;

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
    acc_device_t get_device_type() { return acc_device_nvidia; };
};

#endif // OARU_CUDA_DEVICE_MANAGER_H_