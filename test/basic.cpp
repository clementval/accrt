#include "openacc.h"

int main() {

  float a[10];

  acc_init(acc_device_default);
  acc_create(a, sizeof(float)*10);
  acc_update_device(a, sizeof(float)*10); 
  acc_update_self(a, sizeof(float)*10); 
  acc_delete(a, sizeof(float)*10);
  acc_shutdown(acc_device_default);
  return 0;
}
