#ifndef ENC_KEP_U_H__
#define ENC_KEP_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */


#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

#ifndef OCALL_LOADIDFVALUES_DEFINED__
#define OCALL_LOADIDFVALUES_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_loadIDFValues, (const char* filename, char** keys, double* weights, size_t count, size_t* actual_count));
#endif
#ifndef OCALL_PRINT_STRING_DEFINED__
#define OCALL_PRINT_STRING_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_string, (const char* str));
#endif
#ifndef OCALL_PRINT_INT_DEFINED__
#define OCALL_PRINT_INT_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_int, (int val));
#endif
#ifndef OCALL_PRINT_SIZE_T_DEFINED__
#define OCALL_PRINT_SIZE_T_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_print_size_t, (size_t val));
#endif
#ifndef OCALL_QUERYPERFORMANCECOUNTER_DEFINED__
#define OCALL_QUERYPERFORMANCECOUNTER_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_QueryPerformanceCounter, (int64_t* start_time));
#endif
#ifndef OCALL_QUERYPERFORMANCECOUNTER_END_DEFINED__
#define OCALL_QUERYPERFORMANCECOUNTER_END_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_QueryPerformanceCounter_End, (int64_t* end_time));
#endif
#ifndef OCALL_LOADDICTIONARY_DEFINED__
#define OCALL_LOADDICTIONARY_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_loadDictionary, (const char* filename, char** keys, int* indices, size_t count, size_t* actual_count));
#endif
#ifndef OCALL_LOADVECTORBOOL_DEFINED__
#define OCALL_LOADVECTORBOOL_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_loadVectorBool, (const char* filename, char* data, size_t size, size_t* actual_size));
#endif
#ifndef OCALL_SAVE_PUBLIC_KEY_DEFINED__
#define OCALL_SAVE_PUBLIC_KEY_DEFINED__
void SGX_UBRIDGE(SGX_NOCONVENTION, ocall_save_public_key, (const uint8_t* modulus, size_t n_size, const uint8_t* exponent, size_t e_size));
#endif
#ifndef SGX_OC_CPUIDEX_DEFINED__
#define SGX_OC_CPUIDEX_DEFINED__
void SGX_UBRIDGE(SGX_CDECL, sgx_oc_cpuidex, (int cpuinfo[4], int leaf, int subleaf));
#endif
#ifndef SGX_THREAD_WAIT_UNTRUSTED_EVENT_OCALL_DEFINED__
#define SGX_THREAD_WAIT_UNTRUSTED_EVENT_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_wait_untrusted_event_ocall, (const void* self));
#endif
#ifndef SGX_THREAD_SET_UNTRUSTED_EVENT_OCALL_DEFINED__
#define SGX_THREAD_SET_UNTRUSTED_EVENT_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_untrusted_event_ocall, (const void* waiter));
#endif
#ifndef SGX_THREAD_SETWAIT_UNTRUSTED_EVENTS_OCALL_DEFINED__
#define SGX_THREAD_SETWAIT_UNTRUSTED_EVENTS_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_setwait_untrusted_events_ocall, (const void* waiter, const void* self));
#endif
#ifndef SGX_THREAD_SET_MULTIPLE_UNTRUSTED_EVENTS_OCALL_DEFINED__
#define SGX_THREAD_SET_MULTIPLE_UNTRUSTED_EVENTS_OCALL_DEFINED__
int SGX_UBRIDGE(SGX_CDECL, sgx_thread_set_multiple_untrusted_events_ocall, (const void** waiters, size_t total));
#endif

sgx_status_t ecall_prepare_query_vector(sgx_enclave_id_t eid, const char** keywords_list, size_t keywords_list_size, int k, double* query_vector, size_t query_vector_size, char* w_time, size_t w_time_len);
sgx_status_t foo_on(sgx_enclave_id_t eid, char* buf, size_t len);
sgx_status_t generate_rsa_key_pair_and_export_public_key(sgx_enclave_id_t eid, sgx_status_t* retval);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
