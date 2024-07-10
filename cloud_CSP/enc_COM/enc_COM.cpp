#include "enc_COM_t.h"

#include "sgx_trts.h"
#include <algorithm>
#include <numeric>
#include <cmath>

#include <string.h>
void foo_tw(char *buf, size_t len)
{
	const char *secret = "hello enclave2";
	if (len > strlen(secret))
	{
		memcpy(buf, secret, strlen(secret) + 1);
	}
}

void ecall_sum_array(double* arr, size_t len, double* result) {
	double sum = 0.0;
	for (size_t i = 0; i < len; ++i) {
		sum += arr[i];
	}
	*result = sum;
}

void ecall_diff_array(double* arr, size_t len, double* result) {
	double diff = 0.0;
	for (size_t i = 0; i < len; ++i) {
		diff -= arr[i];
	}
	*result = diff;
}

void ecall_mean_array(double* arr, size_t len, double* result) {
	double mean = 0.0;
	for (size_t i = 0; i < len; ++i) {
		mean += arr[i];
	}
	mean = mean / len;
	*result = mean;
}

void ecall_var_array(double* arr, size_t len, double* result) {
	double sum =0.0, mean = 0.0, var = 0.0;
	for (size_t i = 0; i < len; ++i) {
		sum += arr[i];
	}
	mean = sum / len;
	for (size_t i = 0; i < len; ++i) {
		var += (mean - arr[i]) * (mean - arr[i]);
	}
	var = var / len;
	*result = var;
}

// 快速排序的分区函数
size_t partition(double* arr, size_t low, size_t high) {
	double pivot = arr[high];
	size_t i = low;
	for (size_t j = low; j < high; j++) {
		if (arr[j] < pivot) {
			double temp = arr[i];
			arr[i] = arr[j];
			arr[j] = temp;
			i++;
		}
	}
	double temp = arr[i];
	arr[i] = arr[high];
	arr[high] = temp;
	return i;
}

// 快速排序的递归函数
void quick_sort(double* arr, size_t low, size_t high) {
	if (low < high) {
		size_t pi = partition(arr, low, high);
		if (pi > 0) { // 避免 size_t 变为负数
			quick_sort(arr, low, pi - 1);
		}
		quick_sort(arr, pi + 1, high);
	}
}


void ecall_sort_array(double* arr, size_t len) {
	
	// 调用快速排序算法
	if (len > 0) {
		quick_sort(arr, 0, len - 1);
	}
}

void ecall_find_max(double* arr, size_t len, double* max_val) {
	if (len == 0) {
		*max_val = -1; // Or std::numeric_limits<double>::lowest();
		return;
	}

	double max_value = arr[0];
	for (size_t i = 1; i < len; i++) {
		if (arr[i] > max_value) {
			max_value = arr[i];
		}
	}
	*max_val = max_value;
}

void ecall_find_min(double* arr, size_t len, double* min_val) {
	if (len == 0) {
		*min_val = -1; // Or std::numeric_limits<double>::lowest();
		return;
	}

	double min_value = arr[0];
	for (size_t i = 1; i < len; i++) {
		if (arr[i] < min_value) {
			min_value = arr[i];
		}
	}
	*min_val = min_value;
}

void ecall_search_value(double* arr, size_t len, double value, double* found) {
	*found = 0; // 默认设置为未找到
	for (size_t i = 0; i < len; i++) {
		if (arr[i] == value) {
			*found = 1; // 找到值
			break;
		}
	}
}

void ecall_linear_regression(double* x_values, double* y_values, size_t x_len, size_t y_len, double* slope, double* intercept) {
	double x_sum = 0.0, y_sum = 0.0, xy_sum = 0.0, xx_sum = 0.0;
	double x_mean, y_mean;

	if (x_len != y_len || x_len == 0) {
		*slope = 0;
		*intercept = 0;
		return;
	}

	x_sum = std::accumulate(x_values, x_values + x_len, 0.0);
	y_sum = std::accumulate(y_values, y_values + y_len, 0.0);
	xy_sum = std::inner_product(x_values, x_values + x_len, y_values, 0.0);
	xx_sum = std::inner_product(x_values, x_values + x_len, x_values, 0.0);

	x_mean = x_sum / x_len;
	y_mean = y_sum / y_len;

	*slope = (xy_sum - x_len * x_mean * y_mean) / (xx_sum - x_len * x_mean * x_mean);
	*intercept = y_mean - (*slope * x_mean);
}

void ecall_linear_regression_one(double* x_values, double* y_values, size_t x_len, size_t y_len, double* slope, double* intercept) {
	if (x_len != y_len || x_len == 0) {
		*slope = 0;
		*intercept = 0;
		return;
	}

	double x_sum = std::accumulate(x_values, x_values + x_len, 0.0);
	double y_sum = std::accumulate(y_values, y_values + y_len, 0.0);
	double xy_sum = std::inner_product(x_values, x_values + x_len, y_values, 0.0);
	double xx_sum = std::inner_product(x_values, x_values + x_len, x_values, 0.0);
	double n = static_cast<double>(x_len);

	double x_mean = x_sum / n;
	double y_mean = y_sum / n;

	*slope = (xy_sum - n * x_mean * y_mean) / (xx_sum - n * x_mean * x_mean);
	*intercept = y_mean - (*slope * x_mean);
}

static double sigmoid(double x) {
	return 1 / (1 + exp(-x));
}

void ecall_logistic_regression(double* features, double* labels, size_t n_samples, size_t n_features, double* weights, double* bias) {
	double lr = 0.01;  // 学习率
	*bias = 0.0;

	// 初始化权重为0
	for (size_t i = 0; i < n_features; i++) {
		weights[i] = 0.0;
	}

	// 进行100轮迭代来优化权重
	for (int epoch = 0; epoch < 100; epoch++) {
		for (size_t i = 0; i < n_samples; i++) {
			double linear_combination = 0.0;
			for (size_t j = 0; j < n_features; j++) {
				linear_combination += features[i * n_features + j] * weights[j];
			}
			linear_combination += *bias;

			double predicted = sigmoid(linear_combination);
			double error = labels[i] - predicted;

			for (size_t j = 0; j < n_features; j++) {
				weights[j] += lr * error * predicted * (1 - predicted) * features[i * n_features + j];
			}
			*bias += lr * error * predicted * (1 - predicted);
		}
	}
}

void ecall_stats_analysis(double* data_marray, size_t mcount, double* data_farray, size_t fcount) {
	double m_sum = 0, f_sum = 0, m_sq = 0, f_sq = 0;

	ecall_mean_array(data_marray, mcount, &m_sum);
	ecall_mean_array(data_farray, fcount, &f_sum);

	ecall_var_array(data_marray, mcount, &m_sq);
	ecall_var_array(data_farray, fcount, &f_sq);

	double t = (m_sum - f_sum) / (sqrt(fabs(m_sq / mcount + f_sq / fcount)));
	double v = ((sqrt(fabs(m_sq / mcount + f_sq / fcount))) * (sqrt(fabs(m_sq / mcount + f_sq / fcount)))) / ((m_sq / mcount)* (m_sq / mcount) / (mcount - 1) + (f_sq / fcount)*(f_sq / fcount) / (fcount - 1));
	//ocall_print_stats("男均值: ", m_sum);
	//ocall_print_stats("女均值: ", f_sum);
	//ocall_print_stats("男方差: ", m_sq);
	//ocall_print_stats("女方差: ", f_sq);
	//ocall_print_stats("t: ", t);
	//ocall_print_stats("v: ", v);
}
