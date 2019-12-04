#ifndef OARU_CUDA_ALLOC_INFO_
#define OARU_CUDA_ALLOC_INFO_

#include <cuda.h>
#include "alloc_info.h"

typedef struct CudaAllocInfo {
  AllocInfo allocInfo;
  CUdeviceptr cuPtr;
} CudaAllocInfo;

#endif // OARU_CUDA_ALLOC_INFO_