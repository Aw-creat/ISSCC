#include "enc_KEP_u.h"
#include <errno.h>

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

static sgx_status_t SGX_CDECL enc_KEP_ocall_loadIDFValues(void* pms)
{
	ms_ocall_loadIDFValues_t* ms = SGX_CAST(ms_ocall_loadIDFValues_t*, pms);
	ocall_loadIDFValues(ms->ms_filename, ms->ms_keys, ms->ms_weights, ms->ms_count, ms->ms_actual_count);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_ocall_print_string(void* pms)
{
	ms_ocall_print_string_t* ms = SGX_CAST(ms_ocall_print_string_t*, pms);
	ocall_print_string(ms->ms_str);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_ocall_print_int(void* pms)
{
	ms_ocall_print_int_t* ms = SGX_CAST(ms_ocall_print_int_t*, pms);
	ocall_print_int(ms->ms_val);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_ocall_print_size_t(void* pms)
{
	ms_ocall_print_size_t_t* ms = SGX_CAST(ms_ocall_print_size_t_t*, pms);
	ocall_print_size_t(ms->ms_val);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_ocall_QueryPerformanceCounter(void* pms)
{
	ms_ocall_QueryPerformanceCounter_t* ms = SGX_CAST(ms_ocall_QueryPerformanceCounter_t*, pms);
	ocall_QueryPerformanceCounter(ms->ms_start_time);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_ocall_QueryPerformanceCounter_End(void* pms)
{
	ms_ocall_QueryPerformanceCounter_End_t* ms = SGX_CAST(ms_ocall_QueryPerformanceCounter_End_t*, pms);
	ocall_QueryPerformanceCounter_End(ms->ms_end_time);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_ocall_loadDictionary(void* pms)
{
	ms_ocall_loadDictionary_t* ms = SGX_CAST(ms_ocall_loadDictionary_t*, pms);
	ocall_loadDictionary(ms->ms_filename, ms->ms_keys, ms->ms_indices, ms->ms_count, ms->ms_actual_count);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_ocall_loadVectorBool(void* pms)
{
	ms_ocall_loadVectorBool_t* ms = SGX_CAST(ms_ocall_loadVectorBool_t*, pms);
	ocall_loadVectorBool(ms->ms_filename, ms->ms_data, ms->ms_size, ms->ms_actual_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_ocall_save_public_key(void* pms)
{
	ms_ocall_save_public_key_t* ms = SGX_CAST(ms_ocall_save_public_key_t*, pms);
	ocall_save_public_key(ms->ms_modulus, ms->ms_n_size, ms->ms_exponent, ms->ms_e_size);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_sgx_oc_cpuidex(void* pms)
{
	ms_sgx_oc_cpuidex_t* ms = SGX_CAST(ms_sgx_oc_cpuidex_t*, pms);
	sgx_oc_cpuidex(ms->ms_cpuinfo, ms->ms_leaf, ms->ms_subleaf);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_sgx_thread_wait_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_wait_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_wait_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_wait_untrusted_event_ocall(ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_sgx_thread_set_untrusted_event_ocall(void* pms)
{
	ms_sgx_thread_set_untrusted_event_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_untrusted_event_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_untrusted_event_ocall(ms->ms_waiter);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_sgx_thread_setwait_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_setwait_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_setwait_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_setwait_untrusted_events_ocall(ms->ms_waiter, ms->ms_self);

	return SGX_SUCCESS;
}

static sgx_status_t SGX_CDECL enc_KEP_sgx_thread_set_multiple_untrusted_events_ocall(void* pms)
{
	ms_sgx_thread_set_multiple_untrusted_events_ocall_t* ms = SGX_CAST(ms_sgx_thread_set_multiple_untrusted_events_ocall_t*, pms);
	ms->ms_retval = sgx_thread_set_multiple_untrusted_events_ocall(ms->ms_waiters, ms->ms_total);

	return SGX_SUCCESS;
}

static const struct {
	size_t nr_ocall;
	void * func_addr[14];
} ocall_table_enc_KEP = {
	14,
	{
		(void*)(uintptr_t)enc_KEP_ocall_loadIDFValues,
		(void*)(uintptr_t)enc_KEP_ocall_print_string,
		(void*)(uintptr_t)enc_KEP_ocall_print_int,
		(void*)(uintptr_t)enc_KEP_ocall_print_size_t,
		(void*)(uintptr_t)enc_KEP_ocall_QueryPerformanceCounter,
		(void*)(uintptr_t)enc_KEP_ocall_QueryPerformanceCounter_End,
		(void*)(uintptr_t)enc_KEP_ocall_loadDictionary,
		(void*)(uintptr_t)enc_KEP_ocall_loadVectorBool,
		(void*)(uintptr_t)enc_KEP_ocall_save_public_key,
		(void*)(uintptr_t)enc_KEP_sgx_oc_cpuidex,
		(void*)(uintptr_t)enc_KEP_sgx_thread_wait_untrusted_event_ocall,
		(void*)(uintptr_t)enc_KEP_sgx_thread_set_untrusted_event_ocall,
		(void*)(uintptr_t)enc_KEP_sgx_thread_setwait_untrusted_events_ocall,
		(void*)(uintptr_t)enc_KEP_sgx_thread_set_multiple_untrusted_events_ocall,
	}
};

sgx_status_t ecall_prepare_query_vector(sgx_enclave_id_t eid, const char** keywords_list, size_t keywords_list_size, int k, double* query_vector, size_t query_vector_size, char* w_time, size_t w_time_len)
{
	sgx_status_t status;
	ms_ecall_prepare_query_vector_t ms;
	ms.ms_keywords_list = keywords_list;
	ms.ms_keywords_list_size = keywords_list_size;
	ms.ms_k = k;
	ms.ms_query_vector = query_vector;
	ms.ms_query_vector_size = query_vector_size;
	ms.ms_w_time = w_time;
	ms.ms_w_time_len = w_time_len;
	status = sgx_ecall(eid, 0, &ocall_table_enc_KEP, &ms);
	return status;
}

sgx_status_t foo_on(sgx_enclave_id_t eid, char* buf, size_t len)
{
	sgx_status_t status;
	ms_foo_on_t ms;
	ms.ms_buf = buf;
	ms.ms_len = len;
	status = sgx_ecall(eid, 1, &ocall_table_enc_KEP, &ms);
	return status;
}

sgx_status_t generate_rsa_key_pair_and_export_public_key(sgx_enclave_id_t eid, sgx_status_t* retval)
{
	sgx_status_t status;
	ms_generate_rsa_key_pair_and_export_public_key_t ms;
	status = sgx_ecall(eid, 2, &ocall_table_enc_KEP, &ms);
	if (status == SGX_SUCCESS && retval) *retval = ms.ms_retval;
	return status;
}

