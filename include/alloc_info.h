#ifndef OARU_ALLOC_INFO_
#define OARU_ALLOC_INFO_

#include <cstddef>

typedef struct AllocInfo {
  void* hostPtr;
  void* devPtr;
  size_t size;
} AllocInfo;

#endif // OARU_ALLOC_INFO_