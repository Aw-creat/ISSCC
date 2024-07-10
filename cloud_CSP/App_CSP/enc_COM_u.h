#ifndef ENC_COM_U_H__
#define ENC_COM_U_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include <string.h>
#include "sgx_edger8r.h" /* for sgx_status_t etc. */


#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
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

sgx_status_t foo_tw(sgx_enclave_id_t eid, char* buf, size_t len);
sgx_status_t ecall_sum_array(sgx_enclave_id_t eid, double* arr, size_t len, double* result);
sgx_status_t ecall_diff_array(sgx_enclave_id_t eid, double* arr, size_t len, double* result);
sgx_status_t ecall_mean_array(sgx_enclave_id_t eid, double* arr, size_t len, double* result);
sgx_status_t ecall_var_array(sgx_enclave_id_t eid, double* arr, size_t len, double* result);
sgx_status_t ecall_sort_array(sgx_enclave_id_t eid, double* arr, size_t len);
sgx_status_t ecall_find_max(sgx_enclave_id_t eid, double* arr, size_t len, double* max_val);
sgx_status_t ecall_find_min(sgx_enclave_id_t eid, double* arr, size_t len, double* min_val);
sgx_status_t ecall_search_value(sgx_enclave_id_t eid, double* arr, size_t len, double value, double* found);
sgx_status_t ecall_linear_regression(sgx_enclave_id_t eid, double* x_values, double* y_values, size_t x_len, size_t y_len, double* slope, double* intercept);
sgx_status_t ecall_linear_regression_one(sgx_enclave_id_t eid, double* x_values, double* y_values, size_t x_len, size_t y_len, double* slope, double* intercept);
sgx_status_t ecall_logistic_regression(sgx_enclave_id_t eid, double* features, double* labels, size_t n_samples, size_t n_features, double* weights, double* bias);
sgx_status_t ecall_stats_analysis(sgx_enclave_id_t eid, double* data_marray, size_t mcount, double* data_farray, size_t fcount);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
