#include "enc_KEP_t.h"

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


typedef struct ms_ecall_prepare_query_vector_t {
	const char** ms_keywords_list;
	size_t ms_keywords_list_size;
	int ms_k;
	double* ms_query_vector;
	size_t ms_query_vector_size;
	char* ms_w_time;
	size_t ms_w_time_len;
} ms_ecall_prepare_query_vector_t;

typedef struct ms_foo_on_t {
	char* ms_buf;
	size_t ms_len;
} ms_foo_on_t;

typedef struct ms_generate_rsa_key_pair_and_export_public_key_t {
	sgx_status_t ms_retval;
} ms_generate_rsa_key_pair_and_export_public_key_t;

typedef struct ms_ocall_loadIDFValues_t {
	const char* ms_filename;
	char** ms_keys;
	double* ms_weights;
	size_t ms_count;
	size_t* ms_actual_count;
} ms_ocall_loadIDFValues_t;

typedef struct ms_ocall_print_string_t {
	const char* ms_str;
} ms_ocall_print_string_t;

typedef struct ms_ocall_print_int_t {
	int ms_val;
} ms_ocall_print_int_t;

typedef struct ms_ocall_print_size_t_t {
	size_t ms_val;
} ms_ocall_print_size_t_t;

typedef struct ms_ocall_QueryPerformanceCounter_t {
	int64_t* ms_start_time;
} ms_ocall_QueryPerformanceCounter_t;

typedef struct ms_ocall_QueryPerformanceCounter_End_t {
	int64_t* ms_end_time;
} ms_ocall_QueryPerformanceCounter_End_t;

typedef struct ms_ocall_loadDictionary_t {
	const char* ms_filename;
	char** ms_keys;
	int* ms_indices;
	size_t ms_count;
	size_t* ms_actual_count;
} ms_ocall_loadDictionary_t;

typedef struct ms_ocall_loadVectorBool_t {
	const char* ms_filename;
	char* ms_data;
	size_t ms_size;
	size_t* ms_actual_size;
} ms_ocall_loadVectorBool_t;

typedef struct ms_ocall_save_public_key_t {
	const uint8_t* ms_modulus;
	size_t ms_n_size;
	const uint8_t* ms_exponent;
	size_t ms_e_size;
} ms_ocall_save_public_key_t;

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

static sgx_status_t SGX_CDECL sgx_ecall_prepare_query_vector(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_ecall_prepare_query_vector_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_ecall_prepare_query_vector_t* ms = SGX_CAST(ms_ecall_prepare_query_vector_t*, pms);
	ms_ecall_prepare_query_vector_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_ecall_prepare_query_vector_t), ms, sizeof(ms_ecall_prepare_query_vector_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	const char** _tmp_keywords_list = __in_ms.ms_keywords_list;
	size_t _tmp_keywords_list_size = __in_ms.ms_keywords_list_size;
	size_t _len_keywords_list = _tmp_keywords_list_size * 100;
	char** _in_keywords_list = NULL;
	double* _tmp_query_vector = __in_ms.ms_query_vector;
	char* _tmp_w_time = __in_ms.ms_w_time;

	if (100 != 0 &&
		(size_t)_tmp_keywords_list_size > (SIZE_MAX / 100)) {
		return SGX_ERROR_INVALID_PARAMETER;
	}

	CHECK_UNIQUE_POINTER(_tmp_keywords_list, _len_keywords_list);

	//
	// fence after pointer checks
	//
	sgx_lfence();

	if (_tmp_keywords_list != NULL && _len_keywords_list != 0) {
		if ( _len_keywords_list % sizeof(*_tmp_keywords_list) != 0)
		{
			status = SGX_ERROR_INVALID_PARAMETER;
			goto err;
		}
		_in_keywords_list = (char**)malloc(_len_keywords_list);
		if (_in_keywords_list == NULL) {
			status = SGX_ERROR_OUT_OF_MEMORY;
			goto err;
		}

		if (memcpy_s(_in_keywords_list, _len_keywords_list, _tmp_keywords_list, _len_keywords_list)) {
			status = SGX_ERROR_UNEXPECTED;
			goto err;
		}

	}
	ecall_prepare_query_vector((const char**)_in_keywords_list, _tmp_keywords_list_size, __in_ms.ms_k, _tmp_query_vector, __in_ms.ms_query_vector_size, _tmp_w_time, __in_ms.ms_w_time_len);

err:
	if (_in_keywords_list) free(_in_keywords_list);
	return status;
}

static sgx_status_t SGX_CDECL sgx_foo_on(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_foo_on_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_foo_on_t* ms = SGX_CAST(ms_foo_on_t*, pms);
	ms_foo_on_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_foo_on_t), ms, sizeof(ms_foo_on_t))) {
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
	foo_on(_in_buf, _tmp_len);
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

static sgx_status_t SGX_CDECL sgx_generate_rsa_key_pair_and_export_public_key(void* pms)
{
	CHECK_REF_POINTER(pms, sizeof(ms_generate_rsa_key_pair_and_export_public_key_t));
	//
	// fence after pointer checks
	//
	sgx_lfence();
	ms_generate_rsa_key_pair_and_export_public_key_t* ms = SGX_CAST(ms_generate_rsa_key_pair_and_export_public_key_t*, pms);
	ms_generate_rsa_key_pair_and_export_public_key_t __in_ms;
	if (memcpy_s(&__in_ms, sizeof(ms_generate_rsa_key_pair_and_export_public_key_t), ms, sizeof(ms_generate_rsa_key_pair_and_export_public_key_t))) {
		return SGX_ERROR_UNEXPECTED;
	}
	sgx_status_t status = SGX_SUCCESS;
	sgx_status_t _in_retval;


	_in_retval = generate_rsa_key_pair_and_export_public_key();
	if (memcpy_verw_s(&ms->ms_retval, sizeof(ms->ms_retval), &_in_retval, sizeof(_in_retval))) {
		status = SGX_ERROR_UNEXPECTED;
		goto err;
	}

err:
	return status;
}

SGX_EXTERNC const struct {
	size_t nr_ecall;
	struct {void* call_addr; uint8_t is_priv; uint8_t is_switchless;} ecall_table[3];
} g_ecall_table = {
	3,
	{
		{(void*)(uintptr_t)sgx_ecall_prepare_query_vector, 0, 0},
		{(void*)(uintptr_t)sgx_foo_on, 0, 0},
		{(void*)(uintptr_t)sgx_generate_rsa_key_pair_and_export_public_key, 0, 0},
	}
};

SGX_EXTERNC const struct {
	size_t nr_ocall;
	uint8_t entry_table[14][3];
} g_dyn_entry_table = {
	14,
	{
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
		{0, 0, 0, },
	}
};


sgx_status_t SGX_CDECL ocall_loadIDFValues(const char* filename, char** keys, double* weights, size_t count, size_t* actual_count)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_filename = filename ? strlen(filename) + 1 : 0;
	size_t _len_weights = count * 8;
	size_t _len_actual_count = sizeof(size_t);

	ms_ocall_loadIDFValues_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_loadIDFValues_t);
	void *__tmp = NULL;

	void *__tmp_weights = NULL;
	void *__tmp_actual_count = NULL;

	CHECK_ENCLAVE_POINTER(filename, _len_filename);
	CHECK_ENCLAVE_POINTER(weights, _len_weights);
	CHECK_ENCLAVE_POINTER(actual_count, _len_actual_count);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (filename != NULL) ? _len_filename : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (weights != NULL) ? _len_weights : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (actual_count != NULL) ? _len_actual_count : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_loadIDFValues_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_loadIDFValues_t));
	ocalloc_size -= sizeof(ms_ocall_loadIDFValues_t);

	if (filename != NULL) {
		if (memcpy_verw_s(&ms->ms_filename, sizeof(const char*), &__tmp, sizeof(const char*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_filename % sizeof(*filename) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, filename, _len_filename)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_filename);
		ocalloc_size -= _len_filename;
	} else {
		ms->ms_filename = NULL;
	}

	if (memcpy_verw_s(&ms->ms_keys, sizeof(ms->ms_keys), &keys, sizeof(keys))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (weights != NULL) {
		if (memcpy_verw_s(&ms->ms_weights, sizeof(double*), &__tmp, sizeof(double*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp_weights = __tmp;
		if (_len_weights % sizeof(*weights) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, weights, _len_weights)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_weights);
		ocalloc_size -= _len_weights;
	} else {
		ms->ms_weights = NULL;
	}

	if (memcpy_verw_s(&ms->ms_count, sizeof(ms->ms_count), &count, sizeof(count))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (actual_count != NULL) {
		if (memcpy_verw_s(&ms->ms_actual_count, sizeof(size_t*), &__tmp, sizeof(size_t*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp_actual_count = __tmp;
		if (_len_actual_count % sizeof(*actual_count) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, actual_count, _len_actual_count)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_actual_count);
		ocalloc_size -= _len_actual_count;
	} else {
		ms->ms_actual_count = NULL;
	}

	status = sgx_ocall(0, ms);

	if (status == SGX_SUCCESS) {
		if (weights) {
			if (memcpy_s((void*)weights, _len_weights, __tmp_weights, _len_weights)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (actual_count) {
			if (memcpy_s((void*)actual_count, _len_actual_count, __tmp_actual_count, _len_actual_count)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_string(const char* str)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_str = str ? strlen(str) + 1 : 0;

	ms_ocall_print_string_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_string_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(str, _len_str);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (str != NULL) ? _len_str : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_string_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_string_t));
	ocalloc_size -= sizeof(ms_ocall_print_string_t);

	if (str != NULL) {
		if (memcpy_verw_s(&ms->ms_str, sizeof(const char*), &__tmp, sizeof(const char*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_str % sizeof(*str) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, str, _len_str)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_str);
		ocalloc_size -= _len_str;
	} else {
		ms->ms_str = NULL;
	}

	status = sgx_ocall(1, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_int(int val)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_print_int_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_int_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_int_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_int_t));
	ocalloc_size -= sizeof(ms_ocall_print_int_t);

	if (memcpy_verw_s(&ms->ms_val, sizeof(ms->ms_val), &val, sizeof(val))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(2, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_print_size_t(size_t val)
{
	sgx_status_t status = SGX_SUCCESS;

	ms_ocall_print_size_t_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_print_size_t_t);
	void *__tmp = NULL;


	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_print_size_t_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_print_size_t_t));
	ocalloc_size -= sizeof(ms_ocall_print_size_t_t);

	if (memcpy_verw_s(&ms->ms_val, sizeof(ms->ms_val), &val, sizeof(val))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(3, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_QueryPerformanceCounter(int64_t* start_time)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_start_time = sizeof(int64_t);

	ms_ocall_QueryPerformanceCounter_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_QueryPerformanceCounter_t);
	void *__tmp = NULL;

	void *__tmp_start_time = NULL;

	CHECK_ENCLAVE_POINTER(start_time, _len_start_time);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (start_time != NULL) ? _len_start_time : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_QueryPerformanceCounter_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_QueryPerformanceCounter_t));
	ocalloc_size -= sizeof(ms_ocall_QueryPerformanceCounter_t);

	if (start_time != NULL) {
		if (memcpy_verw_s(&ms->ms_start_time, sizeof(int64_t*), &__tmp, sizeof(int64_t*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp_start_time = __tmp;
		if (_len_start_time % sizeof(*start_time) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset_verw(__tmp_start_time, 0, _len_start_time);
		__tmp = (void *)((size_t)__tmp + _len_start_time);
		ocalloc_size -= _len_start_time;
	} else {
		ms->ms_start_time = NULL;
	}

	status = sgx_ocall(4, ms);

	if (status == SGX_SUCCESS) {
		if (start_time) {
			if (memcpy_s((void*)start_time, _len_start_time, __tmp_start_time, _len_start_time)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_QueryPerformanceCounter_End(int64_t* end_time)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_end_time = sizeof(int64_t);

	ms_ocall_QueryPerformanceCounter_End_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_QueryPerformanceCounter_End_t);
	void *__tmp = NULL;

	void *__tmp_end_time = NULL;

	CHECK_ENCLAVE_POINTER(end_time, _len_end_time);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (end_time != NULL) ? _len_end_time : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_QueryPerformanceCounter_End_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_QueryPerformanceCounter_End_t));
	ocalloc_size -= sizeof(ms_ocall_QueryPerformanceCounter_End_t);

	if (end_time != NULL) {
		if (memcpy_verw_s(&ms->ms_end_time, sizeof(int64_t*), &__tmp, sizeof(int64_t*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp_end_time = __tmp;
		if (_len_end_time % sizeof(*end_time) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		memset_verw(__tmp_end_time, 0, _len_end_time);
		__tmp = (void *)((size_t)__tmp + _len_end_time);
		ocalloc_size -= _len_end_time;
	} else {
		ms->ms_end_time = NULL;
	}

	status = sgx_ocall(5, ms);

	if (status == SGX_SUCCESS) {
		if (end_time) {
			if (memcpy_s((void*)end_time, _len_end_time, __tmp_end_time, _len_end_time)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_loadDictionary(const char* filename, char** keys, int* indices, size_t count, size_t* actual_count)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_filename = filename ? strlen(filename) + 1 : 0;
	size_t _len_indices = count * 4;
	size_t _len_actual_count = sizeof(size_t);

	ms_ocall_loadDictionary_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_loadDictionary_t);
	void *__tmp = NULL;

	void *__tmp_indices = NULL;
	void *__tmp_actual_count = NULL;

	CHECK_ENCLAVE_POINTER(filename, _len_filename);
	CHECK_ENCLAVE_POINTER(indices, _len_indices);
	CHECK_ENCLAVE_POINTER(actual_count, _len_actual_count);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (filename != NULL) ? _len_filename : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (indices != NULL) ? _len_indices : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (actual_count != NULL) ? _len_actual_count : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_loadDictionary_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_loadDictionary_t));
	ocalloc_size -= sizeof(ms_ocall_loadDictionary_t);

	if (filename != NULL) {
		if (memcpy_verw_s(&ms->ms_filename, sizeof(const char*), &__tmp, sizeof(const char*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_filename % sizeof(*filename) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, filename, _len_filename)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_filename);
		ocalloc_size -= _len_filename;
	} else {
		ms->ms_filename = NULL;
	}

	if (memcpy_verw_s(&ms->ms_keys, sizeof(ms->ms_keys), &keys, sizeof(keys))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (indices != NULL) {
		if (memcpy_verw_s(&ms->ms_indices, sizeof(int*), &__tmp, sizeof(int*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp_indices = __tmp;
		if (_len_indices % sizeof(*indices) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, indices, _len_indices)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_indices);
		ocalloc_size -= _len_indices;
	} else {
		ms->ms_indices = NULL;
	}

	if (memcpy_verw_s(&ms->ms_count, sizeof(ms->ms_count), &count, sizeof(count))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (actual_count != NULL) {
		if (memcpy_verw_s(&ms->ms_actual_count, sizeof(size_t*), &__tmp, sizeof(size_t*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp_actual_count = __tmp;
		if (_len_actual_count % sizeof(*actual_count) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, actual_count, _len_actual_count)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_actual_count);
		ocalloc_size -= _len_actual_count;
	} else {
		ms->ms_actual_count = NULL;
	}

	status = sgx_ocall(6, ms);

	if (status == SGX_SUCCESS) {
		if (indices) {
			if (memcpy_s((void*)indices, _len_indices, __tmp_indices, _len_indices)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
		if (actual_count) {
			if (memcpy_s((void*)actual_count, _len_actual_count, __tmp_actual_count, _len_actual_count)) {
				sgx_ocfree();
				return SGX_ERROR_UNEXPECTED;
			}
		}
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_loadVectorBool(const char* filename, char* data, size_t size, size_t* actual_size)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_filename = filename ? strlen(filename) + 1 : 0;

	ms_ocall_loadVectorBool_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_loadVectorBool_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(filename, _len_filename);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (filename != NULL) ? _len_filename : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_loadVectorBool_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_loadVectorBool_t));
	ocalloc_size -= sizeof(ms_ocall_loadVectorBool_t);

	if (filename != NULL) {
		if (memcpy_verw_s(&ms->ms_filename, sizeof(const char*), &__tmp, sizeof(const char*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_filename % sizeof(*filename) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, filename, _len_filename)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_filename);
		ocalloc_size -= _len_filename;
	} else {
		ms->ms_filename = NULL;
	}

	if (memcpy_verw_s(&ms->ms_data, sizeof(ms->ms_data), &data, sizeof(data))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (memcpy_verw_s(&ms->ms_size, sizeof(ms->ms_size), &size, sizeof(size))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (memcpy_verw_s(&ms->ms_actual_size, sizeof(ms->ms_actual_size), &actual_size, sizeof(actual_size))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(7, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

sgx_status_t SGX_CDECL ocall_save_public_key(const uint8_t* modulus, size_t n_size, const uint8_t* exponent, size_t e_size)
{
	sgx_status_t status = SGX_SUCCESS;
	size_t _len_modulus = n_size;
	size_t _len_exponent = e_size;

	ms_ocall_save_public_key_t* ms = NULL;
	size_t ocalloc_size = sizeof(ms_ocall_save_public_key_t);
	void *__tmp = NULL;


	CHECK_ENCLAVE_POINTER(modulus, _len_modulus);
	CHECK_ENCLAVE_POINTER(exponent, _len_exponent);

	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (modulus != NULL) ? _len_modulus : 0))
		return SGX_ERROR_INVALID_PARAMETER;
	if (ADD_ASSIGN_OVERFLOW(ocalloc_size, (exponent != NULL) ? _len_exponent : 0))
		return SGX_ERROR_INVALID_PARAMETER;

	__tmp = sgx_ocalloc(ocalloc_size);
	if (__tmp == NULL) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}
	ms = (ms_ocall_save_public_key_t*)__tmp;
	__tmp = (void *)((size_t)__tmp + sizeof(ms_ocall_save_public_key_t));
	ocalloc_size -= sizeof(ms_ocall_save_public_key_t);

	if (modulus != NULL) {
		if (memcpy_verw_s(&ms->ms_modulus, sizeof(const uint8_t*), &__tmp, sizeof(const uint8_t*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_modulus % sizeof(*modulus) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, modulus, _len_modulus)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_modulus);
		ocalloc_size -= _len_modulus;
	} else {
		ms->ms_modulus = NULL;
	}

	if (memcpy_verw_s(&ms->ms_n_size, sizeof(ms->ms_n_size), &n_size, sizeof(n_size))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	if (exponent != NULL) {
		if (memcpy_verw_s(&ms->ms_exponent, sizeof(const uint8_t*), &__tmp, sizeof(const uint8_t*))) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		if (_len_exponent % sizeof(*exponent) != 0) {
			sgx_ocfree();
			return SGX_ERROR_INVALID_PARAMETER;
		}
		if (memcpy_verw_s(__tmp, ocalloc_size, exponent, _len_exponent)) {
			sgx_ocfree();
			return SGX_ERROR_UNEXPECTED;
		}
		__tmp = (void *)((size_t)__tmp + _len_exponent);
		ocalloc_size -= _len_exponent;
	} else {
		ms->ms_exponent = NULL;
	}

	if (memcpy_verw_s(&ms->ms_e_size, sizeof(ms->ms_e_size), &e_size, sizeof(e_size))) {
		sgx_ocfree();
		return SGX_ERROR_UNEXPECTED;
	}

	status = sgx_ocall(8, ms);

	if (status == SGX_SUCCESS) {
	}
	sgx_ocfree();
	return status;
}

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

	status = sgx_ocall(9, ms);

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

	status = sgx_ocall(10, ms);

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

	status = sgx_ocall(11, ms);

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

	status = sgx_ocall(12, ms);

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

	status = sgx_ocall(13, ms);

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
