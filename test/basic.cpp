#include <cassert>
#include "openacc.h"

#define N 10

int main() {

  float a[N];

  for(int i = 0; i < N; ++i) {
    a[i] = 1.5;
  }

  acc_init(acc_device_default);
  assert(acc_is_present(a, sizeof(float)*N) == 0);
  d_void* a_dptr = acc_create(a, sizeof(float)*N);
  assert(acc_is_present(a, sizeof(float)*N) == 1);
  acc_update_device(a, sizeof(float)*N);
  acc_update_self(a, sizeof(float)*N);

  for(int i = 0; i < N; ++i) {
    assert(a[i] == 1.5);
  }

  acc_delete(a, sizeof(float)*N);  
  assert(acc_is_present(a, sizeof(float)*N) == 0);
  acc_shutdown(acc_device_default);
  return 0;
}
