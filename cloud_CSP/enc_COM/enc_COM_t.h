#ifndef ENC_COM_T_H__
#define ENC_COM_T_H__

#include <stdint.h>
#include <wchar.h>
#include <stddef.h>
#include "sgx_edger8r.h" /* for sgx_ocall etc. */


#define SGX_CAST(type, item) ((type)(item))

#ifdef __cplusplus
extern "C" {
#endif

void foo_tw(char* buf, size_t len);
void ecall_sum_array(double* arr, size_t len, double* result);
void ecall_diff_array(double* arr, size_t len, double* result);
void ecall_mean_array(double* arr, size_t len, double* result);
void ecall_var_array(double* arr, size_t len, double* result);
void ecall_sort_array(double* arr, size_t len);
void ecall_find_max(double* arr, size_t len, double* max_val);
void ecall_find_min(double* arr, size_t len, double* min_val);
void ecall_search_value(double* arr, size_t len, double value, double* found);
void ecall_linear_regression(double* x_values, double* y_values, size_t x_len, size_t y_len, double* slope, double* intercept);
void ecall_linear_regression_one(double* x_values, double* y_values, size_t x_len, size_t y_len, double* slope, double* intercept);
void ecall_logistic_regression(double* features, double* labels, size_t n_samples, size_t n_features, double* weights, double* bias);
void ecall_stats_analysis(double* data_marray, size_t mcount, double* data_farray, size_t fcount);

sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf);
sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter);
sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self);
sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif
