#ifndef ACCRT_H_
#define ACCRT_H_

#define h_void void
#define d_void void

#include <stddef.h>

typedef enum {
  acc_device_none = 0,
  acc_device_default = 1,
  acc_device_host = 2,
  acc_device_not_host = 3
} acc_device_t;

typedef enum {
  acc_property_memory = 0,
  acc_property_free_memory = 1,
  acc_property_shared_memory_support = 2,
  acc_property_name = 3,
  acc_property_vendor = 4,
  acc_property_driver = 5
} acc_device_property_t;

extern int acc_get_num_devices(acc_device_t devicetype); // 3.2.1
extern void acc_set_device_type(acc_device_t devicetype); // 3.2.2
extern acc_device_t acc_get_device_type(void); // 3.2.3
extern void acc_set_device_num(int devicenum, acc_device_t devicetype); // 3.2.4
extern int acc_get_device_num(acc_device_t devicetype); // 3.2.5
extern size_t acc_get_property(int devicenum, acc_device_t devicetype, // 3.2.6
                               acc_device_property_t property);
extern const char* acc_get_property_string(int devicenum,
                                           acc_device_t devicetype,
                                           acc_device_property_t property);
extern void acc_init(acc_device_t devicetype); // 3.2.7
extern void acc_shutdown(acc_device_t devicetype); // 3.2.8
extern int acc_async_test(int arg); // 3.2.9
extern int acc_async_test_device(int arg, int device); // 3.2.10
extern int acc_async_test_all(); // 3.2.11
extern int acc_async_test_all_device(int device); // 3.2.12
extern void acc_wait(int arg); // 3.2.13
extern void acc_wait_device(int arg, int device); // 3.2.14
extern void acc_wait_async(int arg, int async); // 3.2.15
extern void acc_wait_device_async(int arg, int async, int device); // 3.2.16
extern void acc_wait_all(); // 3.2.17
extern void acc_wait_all_device(int device); // 3.2.18
extern void acc_wait_all_async(int async); // 3.2.19
extern void acc_wait_all_device_async(int async, int device); // 3.2.20
extern int acc_get_default_async(void); // 3.2.21
extern void acc_set_default_async(int async); // 3.2.22
extern int acc_on_device(acc_device_t device); // 3.2.23
extern d_void* acc_malloc(size_t len); // 3.2.24
extern void acc_free(d_void* a); // 3.2.25
extern d_void* acc_copyin(h_void* a, size_t len); // 3.2.26
extern void acc_copyin_async(h_void* a, size_t len, int async);
extern d_void* acc_create(h_void* a, size_t len);  // 2.3.27
extern void acc_create_async(h_void* a, size_t len, int async );
extern void acc_copyout(h_void* a, size_t len); // 3.2.28
extern void acc_copyout_async(h_void* a, size_t len, int async );
extern void acc_copyout_finalize(h_void* a, size_t len);
extern void acc_copyout_finalize_async(h_void* a, size_t len, int async);
extern void acc_delete(h_void* a, size_t len); // 3.2.29
extern void acc_delete_async(h_void* a, size_t len, int async);
extern void acc_delete_finalize(h_void* a, size_t len);
extern void acc_delete_finalize_async(h_void* a, size_t len, int async);
extern void acc_update_device(h_void* a, size_t len); // 3.2.30
extern void acc_update_device_async(h_void* a, size_t len, int async);
extern void acc_update_self(h_void* a, size_t len); // 3.2.31
extern void acc_update_self_async(h_void* a, size_t len, int async);
extern void acc_map_data(h_void* a, d_void* b, size_t len); // 3.2.32
extern void acc_unmap_data(h_void* a); // 3.2.33
extern d_void* acc_deviceptr(h_void* a); // 3.2.34
extern h_void* acc_hostptr(d_void* a); // 3.2.35
extern int acc_is_present(h_void* a, size_t len); // 3.2.36
extern void acc_memcpy_to_device(d_void* dest, h_void* src, size_t bytes); // 3.2.37
extern void acc_memcpy_to_device_async(d_void* dest, h_void* src,
       size_t bytes, int async);
extern void acc_memcpy_from_device(h_void* dest, d_void* src, size_t bytes); // 3.2.38
extern void acc_memcpy_from_device_async( h_void* dest, d_void* src,
       size_t bytes, int async);
extern void acc_memcpy_device(d_void* dest, d_void* src, size_t bytes); // 3.2.39
extern void acc_memcpy_device_async( d_void* dest, d_void* src,
        size_t bytes, int async);
extern void acc_attach(h_void** ptr); // 3.2.40
extern void acc_attach_async(h_void** ptr, int async);
extern void acc_detach( h_void** ptr ); // 3.2.41
extern void acc_detach_async(h_void** ptr, int async);
extern void acc_detach_finalize(h_void** ptr);
extern void acc_detach_finalize_async(h_void** ptr, int async);
extern void acc_memcpy_d2d(h_void* dst, h_void* src, // 3.2.42
                           size_t sz, int dstdev, int srcdev);
extern void acc_memcpy_d2d_async(h_void* dst, h_void* src,
                                 size_t sz, int dstdev, int srcdev,
                                 int srcasync);

#endif // ACCRT_H_
