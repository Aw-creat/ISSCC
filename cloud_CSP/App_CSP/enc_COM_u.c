#include "enc_COM_u.h"
#include <errno.h>

typedef struct ms_foo_tw_t {
	char* ms_buf;
	size_t ms_len;
} ms_foo_tw_t;

typedef struct ms_ecall_sum_array_t {
	double* ms_arr;
	size_t ms_len;
	double* ms_result;
} ms_ecall_sum_array_t;

typedef struct ms_ecall_diff_array_t {
	double* ms_arr;
	size_t ms_len;
	double* ms_result;
} ms_ecall_diff_array_t;

typedef struct ms_ecall_mean_array_t {
	double* ms_arr;
	size_t ms_len;
	double* ms_result;
} ms_ecall_mean_array_t;

typedef struct ms_ecall_var_array_t {
	double* ms_arr;
	size_t ms_len;
	double* ms_result;
} ms_ecall_var_array_t;

typedef struct ms_ecall_sort_array_t {
	double* ms_arr;
	size_t ms_len;
} ms_ecall_sort_array_t;

typedef struct ms_ecall_find_max_t {
	double* ms_arr;
	size_t ms_len;
	double* ms_max_val;
} ms_ecall_find_max_t;

typedef struct ms_ecall_find_min_t {
	double* ms_arr;
	size_t ms_len;
	double* ms_min_val;
} ms_ecall_find_min_t;

typedef struct ms_ecall_search_value_t {
	double* ms_arr;
	size_t ms_len;
	double ms_value;
	double* ms_found;
} ms_ecall_search_value_t;

typedef struct ms_ecall_linear_regression_t {
	double* ms_x_values;
	double* ms_y_values;
	size_t ms_x_len;
	size_t ms_y_len;
	double* ms_slope;
	double* ms_intercept;
} ms_ecall_linear_regression_t;

typedef struct ms_ecall_linear_regression_one_t {
	double* ms_x_values;
	double* ms_y_values;
	size_t ms_x_len;
	size_t ms_y_len;
	double* ms_slope;
	double* ms_intercept;
} ms_ecall_linear_regression_one_t;

typedef struct ms_ecall_logistic_regression_t {
	double* ms_features;
	double* ms_labels;
	size_t ms_n_samples;
	size_t ms_n_features;
	double* ms_weights;
	double* ms_bias;
} ms_ecall_logistic_regression_t;

typedef struct ms_ecall_stats_analysis_t {
	double* ms_data_marray;
	size_t ms_mcount;
	double* ms_data_farray;
	size_t ms_fcount;
} ms_ecall_stats_analysis_t;

typedef struct ms_sgx_oc_cpuidex_t {
	int* ms_cpuinfo;
	int ms_leaf;
	int ms_subleaf;
} ms_sgx_oc_cpuidex_t;

typedef struct ms_sgx_thread_wait_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_self;
} ms_sgx_thread_wait_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_set_untrusted_event_ocall_t {
	int ms_retval;
	const void* ms_waiter;
} ms_sgx_thread_set_untrusted_event_ocall_t;

typedef struct ms_sgx_thread_setwait_untrusted_events_ocall_t {
	int ms_retval;
	const void* ms_waiter;
	const void* ms_self;
} ms_sgx_thread_setwait_untrusted_events_ocall_t;

typedef struct ms_sgx_thread_set_multiple_untrusted_events_ocall_t {
	int ms_retval;
	const void** ms_waiters;
	size_t ms_total;
} ms_sgx_thread_set_multiple_untrusted_events_ocall_t;

static sgx_status_t SGX_CDECL enc_COM_sgx_oc_cpuidex(void* pms)
{
	ms_sgx_oc_cpuidex_t* ms = SGX_CAST(ms_sgx_oc_cpuidex_t*, pms);
	sgx_oc_cpuidex(ms->ms_cpuinfo, ms->ms_leaf, ms->ms_subleaf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_COM_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall(ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_COM_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall(ms->ms_waiter);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_COM_sgx_thread_setwait_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_setwait_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_setwait_untrusted_events_ocall(ms->ms_waiter, ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_COM_sgx_thread_set_multiple_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_multiple_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_multiple_untrusted_events_ocall(ms->ms_waiters, ms->ms_total);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * func_addr[5];
} ocall_table_enc_COM = {
	5,
	{
		(void*)(uintptr_t)enc_COM_sgx_oc_cpuidex,
		(void*)(uintptr_t)enc_COM_sgx_thread_wait_untrusted_event_ocall,
		(void*)(uintptr_t)enc_COM_sgx_thread_set_untrusted_event_ocall,
		(void*)(uintptr_t)enc_COM_sgx_thread_setwait_untrusted_events_ocall,
		(void*)(uintptr_t)enc_COM_sgx_thread_set_multiple_untrusted_events_ocall,
	}
};

sgx_status_t foo_tw(sgx_enclave_id_t eid, char* buf, size_t len)
{
	sgx_status_t status;
	ms_foo_tw_t ms;
	ms.ms_buf = buf;
	ms.ms_len = len;
	status = sgx_ecall(eid, 0, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_sum_array(sgx_enclave_id_t eid, double* arr, size_t len, double* result)
{
	sgx_status_t status;
	ms_ecall_sum_array_t ms;
	ms.ms_arr = arr;
	ms.ms_len = len;
	ms.ms_result = result;
	status = sgx_ecall(eid, 1, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_diff_array(sgx_enclave_id_t eid, double* arr, size_t len, double* result)
{
	sgx_status_t status;
	ms_ecall_diff_array_t ms;
	ms.ms_arr = arr;
	ms.ms_len = len;
	ms.ms_result = result;
	status = sgx_ecall(eid, 2, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_mean_array(sgx_enclave_id_t eid, double* arr, size_t len, double* result)
{
	sgx_status_t status;
	ms_ecall_mean_array_t ms;
	ms.ms_arr = arr;
	ms.ms_len = len;
	ms.ms_result = result;
	status = sgx_ecall(eid, 3, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_var_array(sgx_enclave_id_t eid, double* arr, size_t len, double* result)
{
	sgx_status_t status;
	ms_ecall_var_array_t ms;
	ms.ms_arr = arr;
	ms.ms_len = len;
	ms.ms_result = result;
	status = sgx_ecall(eid, 4, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_sort_array(sgx_enclave_id_t eid, double* arr, size_t len)
{
	sgx_status_t status;
	ms_ecall_sort_array_t ms;
	ms.ms_arr = arr;
	ms.ms_len = len;
	status = sgx_ecall(eid, 5, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_find_max(sgx_enclave_id_t eid, double* arr, size_t len, double* max_val)
{
	sgx_status_t status;
	ms_ecall_find_max_t ms;
	ms.ms_arr = arr;
	ms.ms_len = len;
	ms.ms_max_val = max_val;
	status = sgx_ecall(eid, 6, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_find_min(sgx_enclave_id_t eid, double* arr, size_t len, double* min_val)
{
	sgx_status_t status;
	ms_ecall_find_min_t ms;
	ms.ms_arr = arr;
	ms.ms_len = len;
	ms.ms_min_val = min_val;
	status = sgx_ecall(eid, 7, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_search_value(sgx_enclave_id_t eid, double* arr, size_t len, double value, double* found)
{
	sgx_status_t status;
	ms_ecall_search_value_t ms;
	ms.ms_arr = arr;
	ms.ms_len = len;
	ms.ms_value = value;
	ms.ms_found = found;
	status = sgx_ecall(eid, 8, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_linear_regression(sgx_enclave_id_t eid, double* x_values, double* y_values, size_t x_len, size_t y_len, double* slope, double* intercept)
{
	sgx_status_t status;
	ms_ecall_linear_regression_t ms;
	ms.ms_x_values = x_values;
	ms.ms_y_values = y_values;
	ms.ms_x_len = x_len;
	ms.ms_y_len = y_len;
	ms.ms_slope = slope;
	ms.ms_intercept = intercept;
	status = sgx_ecall(eid, 9, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_linear_regression_one(sgx_enclave_id_t eid, double* x_values, double* y_values, size_t x_len, size_t y_len, double* slope, double* intercept)
{
	sgx_status_t status;
	ms_ecall_linear_regression_one_t ms;
	ms.ms_x_values = x_values;
	ms.ms_y_values = y_values;
	ms.ms_x_len = x_len;
	ms.ms_y_len = y_len;
	ms.ms_slope = slope;
	ms.ms_intercept = intercept;
	status = sgx_ecall(eid, 10, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_logistic_regression(sgx_enclave_id_t eid, double* features, double* labels, size_t n_samples, size_t n_features, double* weights, double* bias)
{
	sgx_status_t status;
	ms_ecall_logistic_regression_t ms;
	ms.ms_features = features;
	ms.ms_labels = labels;
	ms.ms_n_samples = n_samples;
	ms.ms_n_features = n_features;
	ms.ms_weights = weights;
	ms.ms_bias = bias;
	status = sgx_ecall(eid, 11, &ocall_table_enc_COM, &ms);
	return status;
}

sgx_status_t ecall_stats_analysis(sgx_enclave_id_t eid, double* data_marray, size_t mcount, double* data_farray, size_t fcount)
{
	sgx_status_t status;
	ms_ecall_stats_analysis_t ms;
	ms.ms_data_marray = data_marray;
	ms.ms_mcount = mcount;
	ms.ms_data_farray = data_farray;
	ms.ms_fcount = fcount;
	status = sgx_ecall(eid, 12, &ocall_table_enc_COM, &ms);
	return status;
}

