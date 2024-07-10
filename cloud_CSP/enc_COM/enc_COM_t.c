#include "enc_COM_t.h"

#include "sgx_trts.h" /* for sgx_ocalloc, sgx_is_outside_enclave */
#include "sgx_lfence.h" /* for sgx_lfence */

#include <errno.h>
#include <mbusafecrt.h> /* for memcpy_s etc */
#include <stdlib.h> /* for malloc/free etc */

#define CHECK_REF_POINTER(ptr, siz) do {	\
	if (!(ptr) || ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_UNIQUE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_outside_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define CHECK_ENCLAVE_POINTER(ptr, siz) do {	\
	if ((ptr) && ! sgx_is_within_enclave((ptr), (siz)))	\
		return SGX_ERROR_INVALID_PARAMETER;\
} while (0)

#define ADD_ASSIGN_OVERFLOW(a, b) (	\
	((a) += (b)) < (b)	\
)


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

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4127)
#pragma warning(disable: 4200)
#pragma warning(disable: 4090)
#endif

static sgx_status_t SGX_CDECL sgx_foo_tw(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_foo_tw_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_foo_tw_t* ms = SGX_CAST(ms_foo_tw_t*, pms);
	ms_foo_tw_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_foo_tw_t), ms, sizeof(ms_foo_tw_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	char* _tmp_buf = __in_ms.ms_buf;
	size_t _tmp_len = __in_ms.ms_len;
	size_t _len_buf = _tmp_len;
	char* _in_buf = NULL;

	CHECK_UNIQUE_POINTER(_tmp_buf, _len_buf);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_buf != NULL && _len_buf != 0) {
		if ( _len_buf % sizeof(*_tmp_buf) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_buf = (char*)malloc(_len_buf)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_buf, 0, _len_buf);
	}
	foo_tw(_in_buf, _tmp_len);
	if (_in_buf) {
		if (memcpy_verw_s(_tmp_buf, _len_buf, _in_buf, _len_buf)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_buf) free(_in_buf);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_sum_array(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_sum_array_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_sum_array_t* ms = SGX_CAST(ms_ecall_sum_array_t*, pms);
	ms_ecall_sum_array_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_sum_array_t), ms, sizeof(ms_ecall_sum_array_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_arr = __in_ms.ms_arr;
	double* _tmp_result = __in_ms.ms_result;
	size_t _len_result = sizeof(double);
	double* _in_result = NULL;

	CHECK_UNIQUE_POINTER(_tmp_result, _len_result);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_result != NULL && _len_result != 0) {
		if ( _len_result % sizeof(*_tmp_result) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_result = (double*)malloc(_len_result)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_result, 0, _len_result);
	}
	ecall_sum_array(_tmp_arr, __in_ms.ms_len, _in_result);
	if (_in_result) {
		if (memcpy_verw_s(_tmp_result, _len_result, _in_result, _len_result)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_result) free(_in_result);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_diff_array(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_diff_array_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_diff_array_t* ms = SGX_CAST(ms_ecall_diff_array_t*, pms);
	ms_ecall_diff_array_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_diff_array_t), ms, sizeof(ms_ecall_diff_array_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_arr = __in_ms.ms_arr;
	double* _tmp_result = __in_ms.ms_result;
	size_t _len_result = sizeof(double);
	double* _in_result = NULL;

	CHECK_UNIQUE_POINTER(_tmp_result, _len_result);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_result != NULL && _len_result != 0) {
		if ( _len_result % sizeof(*_tmp_result) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_result = (double*)malloc(_len_result)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_result, 0, _len_result);
	}
	ecall_diff_array(_tmp_arr, __in_ms.ms_len, _in_result);
	if (_in_result) {
		if (memcpy_verw_s(_tmp_result, _len_result, _in_result, _len_result)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_result) free(_in_result);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_mean_array(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_mean_array_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_mean_array_t* ms = SGX_CAST(ms_ecall_mean_array_t*, pms);
	ms_ecall_mean_array_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_mean_array_t), ms, sizeof(ms_ecall_mean_array_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_arr = __in_ms.ms_arr;
	double* _tmp_result = __in_ms.ms_result;
	size_t _len_result = sizeof(double);
	double* _in_result = NULL;

	CHECK_UNIQUE_POINTER(_tmp_result, _len_result);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_result != NULL && _len_result != 0) {
		if ( _len_result % sizeof(*_tmp_result) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_result = (double*)malloc(_len_result)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_result, 0, _len_result);
	}
	ecall_mean_array(_tmp_arr, __in_ms.ms_len, _in_result);
	if (_in_result) {
		if (memcpy_verw_s(_tmp_result, _len_result, _in_result, _len_result)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_result) free(_in_result);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_var_array(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_var_array_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_var_array_t* ms = SGX_CAST(ms_ecall_var_array_t*, pms);
	ms_ecall_var_array_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_var_array_t), ms, sizeof(ms_ecall_var_array_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_arr = __in_ms.ms_arr;
	double* _tmp_result = __in_ms.ms_result;
	size_t _len_result = sizeof(double);
	double* _in_result = NULL;

	CHECK_UNIQUE_POINTER(_tmp_result, _len_result);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_result != NULL && _len_result != 0) {
		if ( _len_result % sizeof(*_tmp_result) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_result = (double*)malloc(_len_result)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_result, 0, _len_result);
	}
	ecall_var_array(_tmp_arr, __in_ms.ms_len, _in_result);
	if (_in_result) {
		if (memcpy_verw_s(_tmp_result, _len_result, _in_result, _len_result)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_result) free(_in_result);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_sort_array(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_sort_array_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_sort_array_t* ms = SGX_CAST(ms_ecall_sort_array_t*, pms);
	ms_ecall_sort_array_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_sort_array_t), ms, sizeof(ms_ecall_sort_array_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_arr = __in_ms.ms_arr;


	ecall_sort_array(_tmp_arr, __in_ms.ms_len);


	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_find_max(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_find_max_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_find_max_t* ms = SGX_CAST(ms_ecall_find_max_t*, pms);
	ms_ecall_find_max_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_find_max_t), ms, sizeof(ms_ecall_find_max_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_arr = __in_ms.ms_arr;
	size_t _tmp_len = __in_ms.ms_len;
	size_t _len_arr = _tmp_len;
	double* _in_arr = NULL;
	double* _tmp_max_val = __in_ms.ms_max_val;
	size_t _len_max_val = sizeof(double);
	double* _in_max_val = NULL;

	CHECK_UNIQUE_POINTER(_tmp_arr, _len_arr);
	CHECK_UNIQUE_POINTER(_tmp_max_val, _len_max_val);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_arr != NULL && _len_arr != 0) {
		if ( _len_arr % sizeof(*_tmp_arr) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_arr = (double*)malloc(_len_arr);
		if (_in_arr == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_arr, _len_arr, _tmp_arr, _len_arr)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_max_val != NULL && _len_max_val != 0) {
		if ( _len_max_val % sizeof(*_tmp_max_val) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_max_val = (double*)malloc(_len_max_val)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_max_val, 0, _len_max_val);
	}
	ecall_find_max(_in_arr, _tmp_len, _in_max_val);
	if (_in_max_val) {
		if (memcpy_verw_s(_tmp_max_val, _len_max_val, _in_max_val, _len_max_val)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_arr) free(_in_arr);
	if (_in_max_val) free(_in_max_val);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_find_min(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_find_min_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_find_min_t* ms = SGX_CAST(ms_ecall_find_min_t*, pms);
	ms_ecall_find_min_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_find_min_t), ms, sizeof(ms_ecall_find_min_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_arr = __in_ms.ms_arr;
	size_t _tmp_len = __in_ms.ms_len;
	size_t _len_arr = _tmp_len;
	double* _in_arr = NULL;
	double* _tmp_min_val = __in_ms.ms_min_val;
	size_t _len_min_val = sizeof(double);
	double* _in_min_val = NULL;

	CHECK_UNIQUE_POINTER(_tmp_arr, _len_arr);
	CHECK_UNIQUE_POINTER(_tmp_min_val, _len_min_val);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_arr != NULL && _len_arr != 0) {
		if ( _len_arr % sizeof(*_tmp_arr) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_arr = (double*)malloc(_len_arr);
		if (_in_arr == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_arr, _len_arr, _tmp_arr, _len_arr)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_min_val != NULL && _len_min_val != 0) {
		if ( _len_min_val % sizeof(*_tmp_min_val) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_min_val = (double*)malloc(_len_min_val)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_min_val, 0, _len_min_val);
	}
	ecall_find_min(_in_arr, _tmp_len, _in_min_val);
	if (_in_min_val) {
		if (memcpy_verw_s(_tmp_min_val, _len_min_val, _in_min_val, _len_min_val)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_arr) free(_in_arr);
	if (_in_min_val) free(_in_min_val);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_search_value(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_search_value_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_search_value_t* ms = SGX_CAST(ms_ecall_search_value_t*, pms);
	ms_ecall_search_value_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_search_value_t), ms, sizeof(ms_ecall_search_value_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_arr = __in_ms.ms_arr;
	size_t _tmp_len = __in_ms.ms_len;
	size_t _len_arr = _tmp_len;
	double* _in_arr = NULL;
	double* _tmp_found = __in_ms.ms_found;
	size_t _len_found = sizeof(double);
	double* _in_found = NULL;

	CHECK_UNIQUE_POINTER(_tmp_arr, _len_arr);
	CHECK_UNIQUE_POINTER(_tmp_found, _len_found);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_arr != NULL && _len_arr != 0) {
		if ( _len_arr % sizeof(*_tmp_arr) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_arr = (double*)malloc(_len_arr);
		if (_in_arr == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_arr, _len_arr, _tmp_arr, _len_arr)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_found != NULL && _len_found != 0) {
		if ( _len_found % sizeof(*_tmp_found) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_found = (double*)malloc(_len_found)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_found, 0, _len_found);
	}
	ecall_search_value(_in_arr, _tmp_len, __in_ms.ms_value, _in_found);
	if (_in_found) {
		if (memcpy_verw_s(_tmp_found, _len_found, _in_found, _len_found)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_arr) free(_in_arr);
	if (_in_found) free(_in_found);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_linear_regression(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_linear_regression_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_linear_regression_t* ms = SGX_CAST(ms_ecall_linear_regression_t*, pms);
	ms_ecall_linear_regression_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_linear_regression_t), ms, sizeof(ms_ecall_linear_regression_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_x_values = __in_ms.ms_x_values;
	size_t _tmp_x_len = __in_ms.ms_x_len;
	size_t _len_x_values = _tmp_x_len;
	double* _in_x_values = NULL;
	double* _tmp_y_values = __in_ms.ms_y_values;
	size_t _tmp_y_len = __in_ms.ms_y_len;
	size_t _len_y_values = _tmp_y_len;
	double* _in_y_values = NULL;
	double* _tmp_slope = __in_ms.ms_slope;
	size_t _len_slope = sizeof(double);
	double* _in_slope = NULL;
	double* _tmp_intercept = __in_ms.ms_intercept;
	size_t _len_intercept = sizeof(double);
	double* _in_intercept = NULL;

	CHECK_UNIQUE_POINTER(_tmp_x_values, _len_x_values);
	CHECK_UNIQUE_POINTER(_tmp_y_values, _len_y_values);
	CHECK_UNIQUE_POINTER(_tmp_slope, _len_slope);
	CHECK_UNIQUE_POINTER(_tmp_intercept, _len_intercept);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_x_values != NULL && _len_x_values != 0) {
		if ( _len_x_values % sizeof(*_tmp_x_values) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_x_values = (double*)malloc(_len_x_values);
		if (_in_x_values == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_x_values, _len_x_values, _tmp_x_values, _len_x_values)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_y_values != NULL && _len_y_values != 0) {
		if ( _len_y_values % sizeof(*_tmp_y_values) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_y_values = (double*)malloc(_len_y_values);
		if (_in_y_values == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_y_values, _len_y_values, _tmp_y_values, _len_y_values)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_slope != NULL && _len_slope != 0) {
		if ( _len_slope % sizeof(*_tmp_slope) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_slope = (double*)malloc(_len_slope)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_slope, 0, _len_slope);
	}
	if (_tmp_intercept != NULL && _len_intercept != 0) {
		if ( _len_intercept % sizeof(*_tmp_intercept) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_intercept = (double*)malloc(_len_intercept)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_intercept, 0, _len_intercept);
	}
	ecall_linear_regression(_in_x_values, _in_y_values, _tmp_x_len, _tmp_y_len, _in_slope, _in_intercept);
	if (_in_slope) {
		if (memcpy_verw_s(_tmp_slope, _len_slope, _in_slope, _len_slope)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}
	if (_in_intercept) {
		if (memcpy_verw_s(_tmp_intercept, _len_intercept, _in_intercept, _len_intercept)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_x_values) free(_in_x_values);
	if (_in_y_values) free(_in_y_values);
	if (_in_slope) free(_in_slope);
	if (_in_intercept) free(_in_intercept);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_linear_regression_one(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_linear_regression_one_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_linear_regression_one_t* ms = SGX_CAST(ms_ecall_linear_regression_one_t*, pms);
	ms_ecall_linear_regression_one_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_linear_regression_one_t), ms, sizeof(ms_ecall_linear_regression_one_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_x_values = __in_ms.ms_x_values;
	size_t _tmp_x_len = __in_ms.ms_x_len;
	size_t _len_x_values = _tmp_x_len;
	double* _in_x_values = NULL;
	double* _tmp_y_values = __in_ms.ms_y_values;
	size_t _tmp_y_len = __in_ms.ms_y_len;
	size_t _len_y_values = _tmp_y_len;
	double* _in_y_values = NULL;
	double* _tmp_slope = __in_ms.ms_slope;
	size_t _len_slope = sizeof(double);
	double* _in_slope = NULL;
	double* _tmp_intercept = __in_ms.ms_intercept;
	size_t _len_intercept = sizeof(double);
	double* _in_intercept = NULL;

	CHECK_UNIQUE_POINTER(_tmp_x_values, _len_x_values);
	CHECK_UNIQUE_POINTER(_tmp_y_values, _len_y_values);
	CHECK_UNIQUE_POINTER(_tmp_slope, _len_slope);
	CHECK_UNIQUE_POINTER(_tmp_intercept, _len_intercept);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_x_values != NULL && _len_x_values != 0) {
		if ( _len_x_values % sizeof(*_tmp_x_values) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_x_values = (double*)malloc(_len_x_values);
		if (_in_x_values == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_x_values, _len_x_values, _tmp_x_values, _len_x_values)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_y_values != NULL && _len_y_values != 0) {
		if ( _len_y_values % sizeof(*_tmp_y_values) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_y_values = (double*)malloc(_len_y_values);
		if (_in_y_values == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_y_values, _len_y_values, _tmp_y_values, _len_y_values)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	if (_tmp_slope != NULL && _len_slope != 0) {
		if ( _len_slope % sizeof(*_tmp_slope) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_slope = (double*)malloc(_len_slope)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_slope, 0, _len_slope);
	}
	if (_tmp_intercept != NULL && _len_intercept != 0) {
		if ( _len_intercept % sizeof(*_tmp_intercept) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		if ((_in_intercept = (double*)malloc(_len_intercept)) == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		memset((void*)_in_intercept, 0, _len_intercept);
	}
	ecall_linear_regression_one(_in_x_values, _in_y_values, _tmp_x_len, _tmp_y_len, _in_slope, _in_intercept);
	if (_in_slope) {
		if (memcpy_verw_s(_tmp_slope, _len_slope, _in_slope, _len_slope)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}
	if (_in_intercept) {
		if (memcpy_verw_s(_tmp_intercept, _len_intercept, _in_intercept, _len_intercept)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}
	}

err:
	if (_in_x_values) free(_in_x_values);
	if (_in_y_values) free(_in_y_values);
	if (_in_slope) free(_in_slope);
	if (_in_intercept) free(_in_intercept);
	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_logistic_regression(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_logistic_regression_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_logistic_regression_t* ms = SGX_CAST(ms_ecall_logistic_regression_t*, pms);
	ms_ecall_logistic_regression_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_logistic_regression_t), ms, sizeof(ms_ecall_logistic_regression_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_features = __in_ms.ms_features;
	double* _tmp_labels = __in_ms.ms_labels;
	double* _tmp_weights = __in_ms.ms_weights;
	double* _tmp_bias = __in_ms.ms_bias;


	ecall_logistic_regression(_tmp_features, _tmp_labels, __in_ms.ms_n_samples, __in_ms.ms_n_features, _tmp_weights, _tmp_bias);


	return status;
}

static sgx_status_t SGX_CDECL sgx_ecall_stats_analysis(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_stats_analysis_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_stats_analysis_t* ms = SGX_CAST(ms_ecall_stats_analysis_t*, pms);
	ms_ecall_stats_analysis_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_stats_analysis_t), ms, sizeof(ms_ecall_stats_analysis_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	double* _tmp_data_marray = __in_ms.ms_data_marray;
	double* _tmp_data_farray = __in_ms.ms_data_farray;


	ecall_stats_analysis(_tmp_data_marray, __in_ms.ms_mcount, _tmp_data_farray, __in_ms.ms_fcount);


	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* call_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[13];
} g_ecall_table = {
	13,
	{
		{(void*)(uintptr_t)sgx_foo_tw, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_sum_array, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_diff_array, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_mean_array, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_var_array, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_sort_array, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_find_max, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_find_min, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_search_value, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_linear_regression, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_linear_regression_one, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_logistic_regression, 0, 0},
		{(void*)(uintptr_t)sgx_ecall_stats_analysis, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[5][13];
} g_dyn_entry_table = {
	5,
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL sgx_oc_cpuidex(int cpuinfo[4], int leaf, int subleaf)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_cpuinfo = 4 * sizeof(int);

	ms_sgx_oc_cpuidex_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_oc_cpuidex_t);
	void *__tmp = NULL;

	void *__tmp_cpuinfo = NULL;

	CHECK_ENCLAVE_POINTER(cpuinfo, _len_cpuinfo);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (cpuinfo != NULL) ? _len_cpuinfo : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_oc_cpuidex_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_oc_cpuidex_t));
	ocalloc_size -= sizeof(ms_sgx_oc_cpuidex_t);

	if (cpuinfo != NULL) {
		if (memcpy_verw_s(&ms->ms_cpuinfo, sizeof(int*), &__tmp, sizeof(int*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp_cpuinfo = __tmp;
		if (_len_cpuinfo % sizeof(*cpuinfo) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset_verw(__tmp_cpuinfo, 0, _len_cpuinfo);
		__tmp = (void *)((size_t)__tmp + _len_cpuinfo);
		ocalloc_size -= _len_cpuinfo;
	} else {
		ms->ms_cpuinfo = NULL;
	}

	if (memcpy_verw_s(&ms->ms_leaf, sizeof(ms->ms_leaf), &leaf, sizeof(leaf))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (memcpy_verw_s(&ms->ms_subleaf, sizeof(ms->ms_subleaf), &subleaf, sizeof(subleaf))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
		if (cpuinfo) {
			if (memcpy_s((void*)cpuinfo, _len_cpuinfo, __tmp_cpuinfo, _len_cpuinfo)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_wait_untrusted_event_ocall(int* retval, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_wait_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_wait_untrusted_event_ocall_t);

	if (memcpy_verw_s(&ms->ms_self, sizeof(ms->ms_self), &self, sizeof(self))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(1, ms);

	if (status == SGX_SUCCESS) {
		if (retval) {
			if (memcpy_s((void*)retval, sizeof(*retval), &ms->ms_retval, sizeof(ms->ms_retval))) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_untrusted_event_ocall(int* retval, const void* waiter)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_set_untrusted_event_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_untrusted_event_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_untrusted_event_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_set_untrusted_event_ocall_t);

	if (memcpy_verw_s(&ms->ms_waiter, sizeof(ms->ms_waiter), &waiter, sizeof(waiter))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(2, ms);

	if (status == SGX_SUCCESS) {
		if (retval) {
			if (memcpy_s((void*)retval, sizeof(*retval), &ms->ms_retval, sizeof(ms->ms_retval))) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_setwait_untrusted_events_ocall(int* retval, const void* waiter, const void* self)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_setwait_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_setwait_untrusted_events_ocall_t);

	if (memcpy_verw_s(&ms->ms_waiter, sizeof(ms->ms_waiter), &waiter, sizeof(waiter))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (memcpy_verw_s(&ms->ms_self, sizeof(ms->ms_self), &self, sizeof(self))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(3, ms);

	if (status == SGX_SUCCESS) {
		if (retval) {
			if (memcpy_s((void*)retval, sizeof(*retval), &ms->ms_retval, sizeof(ms->ms_retval))) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL sgx_thread_set_multiple_untrusted_events_ocall(int* retval, const void** waiters, size_t total)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_waiters = total * sizeof(void*);

	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(waiters, _len_waiters);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (waiters != NULL) ? _len_waiters : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_sgx_thread_set_multiple_untrusted_events_ocall_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t));
	ocalloc_size -= sizeof(ms_sgx_thread_set_multiple_untrusted_events_ocall_t);

	if (waiters != NULL) {
		if (memcpy_verw_s(&ms->ms_waiters, sizeof(const void**), &__tmp, sizeof(const void**))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_waiters % sizeof(*waiters) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, waiters, _len_waiters)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_waiters);
		ocalloc_size -= _len_waiters;
	} else {
		ms->ms_waiters = NULL;
	}

	if (memcpy_verw_s(&ms->ms_total, sizeof(ms->ms_total), &total, sizeof(total))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(4, ms);

	if (status == SGX_SUCCESS) {
		if (retval) {
			if (memcpy_s((void*)retval, sizeof(*retval), &ms->ms_retval, sizeof(ms->ms_retval))) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif
