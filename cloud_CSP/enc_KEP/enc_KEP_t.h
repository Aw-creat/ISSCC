#ifndef ENC_KEP_T_H__
#define ENC_KEP_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */


#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void ecall_prepare_query_vector(const char** keywords_list, size_t keywords_list_size, int k, double* query_vector, size_t query_vector_size, char* w_time, size_t w_time_len);
void foo_on(char* buf, size_t len);
sgx_status_t generate_rsa_key_pair_and_export_public_key(void);

sgx_status_t SGX_CDECL ocall_loadIDFValues(const char* filename, char** keys, double* weights, size_t count, size_t* actual_count);
sgx_status_t SGX_CDECL ocall_print_string(const char* str);
sgx_status_t SGX_CDECL ocall_print_int(int val);
sgx_status_t SGX_CDECL ocall_print_size_t(size_t val);
sgx_status_t SGX_CDECL ocall_QueryPerformanceCounter(int64_t* start_time);
sgx_status_t SGX_CDECL ocall_QueryPerformanceCounter_End(int64_t* end_time);
sgx_status_t SGX_CDECL ocall_loadDictionary(const char* filename, char** keys, int* indices, size_t count, size_t* actual_count);
sgx_status_t SGX_CDECL ocall_loadVectorBool(const char* filename, char* data, size_t size, size_t* actual_size);
sgx_status_t SGX_CDECL ocall_save_public_key(const uint8_t* modulus, size_t n_size, const uint8_t* exponent, size_t e_size);
sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf);
sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter);
sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
