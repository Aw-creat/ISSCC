#include "enc_KEP_t.h"
#include "sgx_trts.h"
#include "sgx_tcrypto.h"
//#include "kepTool.h"
#include <string.h>
#include <vector>

#include <map>
#include <queue>

#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>

#include <iomanip> // 添加这个头文件
#include <stack>
#include <functional>
#include <cmath> 
using namespace std;
map<string, int> Dictionary;
map<string, double> IDFVALUE;
double K_TH_SCORE;
int KEYWORD_NUM;
int num_k;    //the value of k
vector<double> queryVector;
vector<bool> ARRAY_S;
const int KEY_EXTEND = 0;


void foo_on(char *buf, size_t len)
{
	const char *secret = "hello enclave1";
	if (len > strlen(secret))
	{
		memcpy(buf, secret, strlen(secret) + 1);
	}
}

map<string, double> LoadIDFValuesFromHost(const string& filename) {
	const size_t MAX_ENTRIES = 13379;
	char** keys = NULL;
	//double weights[MAX_ENTRIES];
	double* weights = new double[MAX_ENTRIES];
	size_t actual_count = 0;

	// 初始化方法keys
	unsigned int i = 0;
	keys = (char**)calloc(MAX_ENTRIES, sizeof(char*));
	for (i = 0; i < MAX_ENTRIES; ++i)
	{
		keys[i] = (char*)calloc(60, sizeof(char));
	}

	ocall_loadIDFValues(filename.c_str(), keys, weights, MAX_ENTRIES, &actual_count);
	// 创建局部字典

	//map<string, int> Dictionary;
	for (size_t i = 0; i < actual_count; ++i) {
		IDFVALUE[string(keys[i])] = weights[i];
		//delete[] keys[i];  // 释放分配的内存
	}
	delete[] keys;  // 释放 keys 数组的内存
	delete[] weights;  // 释放 weights 数组的内存

	// 输出加载信息
	char buffer[100];
	snprintf(buffer, sizeof(buffer), "IDFValues loaded from host. Total entries: %zu", actual_count);
	ocall_print_string(buffer);
	return IDFVALUE;
}

map<string, int> LoadDictionaryFromHost(const string& filename) {
	const size_t MAX_ENTRIES = 400;
	char** keys=NULL;
	int indices[MAX_ENTRIES];
	size_t actual_count = 0;

	// 初始化方法keys
	unsigned int i = 0;
	keys = (char**)calloc(MAX_ENTRIES, sizeof(char*));
	for (i = 0; i < MAX_ENTRIES; ++i)
	{
		keys[i] = (char*)calloc(60, sizeof(char));
	}
	

	ocall_loadDictionary(filename.c_str(), keys, indices, MAX_ENTRIES, &actual_count);
	// 创建局部字典

	//map<string, int> Dictionary;
	for (size_t i = 0; i < actual_count; ++i) {
		Dictionary[std::string(keys[i])] = indices[i];
		delete[] keys[i];  // 释放分配的内存
	}

	// 输出加载信息
	char buffer[100];
	snprintf(buffer, sizeof(buffer), "Dictionary loaded from host. Total entries: %zu", actual_count);
	ocall_print_string(buffer);
	return Dictionary;
}
//vector<double> prepareQueryVector(vector<string> keywords_list, int k, string &w_time);
vector<bool> loadVectorBoolFromHost(const string& filename) {
	const size_t MAX_SIZE = 1000;  // 假设最大容量
	char buffer[MAX_SIZE];
	size_t actual_size = 0;

	// 调用外部定义的 OCall 以从文件中加载数据
	ocall_loadVectorBool(filename.c_str(), buffer, MAX_SIZE, &actual_size);

	// 创建并填充 vector<bool>
	vector<bool> vec;
	vec.reserve(actual_size);  // 预先分配足够的空间
	for (size_t i = 0; i < actual_size; ++i) {
		vec.push_back(buffer[i] != 0);  // 将读取的字节转换为布尔值并添加到 vector
	}

	return vec;  // 返回填充好的 vector
}



void generate_random_doubles(vector<double>& values, size_t count) {
	values.resize(count);
	for (size_t i = 0; i < count; i++) {
		uint8_t rand_bytes[4]; // 32 bits of randomness
		sgx_status_t status = sgx_read_rand(rand_bytes, sizeof(rand_bytes));
		if (status != SGX_SUCCESS) {
			char error_msg[100];
			snprintf(error_msg, sizeof(error_msg), "Failed to generate random data: %d", status);
			ocall_print_string(error_msg);
			//std::cerr << "Failed to generate random data: " << status << std::endl;
			return;
		}

		uint32_t rand_int = *reinterpret_cast<uint32_t*>(rand_bytes);
		double rand_double = static_cast<double>(rand_int) / (static_cast<double>(UINT32_MAX) + 1);
		values[i] = rand_double;
	}
}

vector<double> prepareQueryVector(const char** keywords_list, size_t keywords_list_size, int k, string &w_time) {
	int num_k = k;

	IDFVALUE = LoadIDFValuesFromHost("E:\\SCIS\\SCIS_eight\\cloud_CSP\\App_CSP\\allWordsIDF.txt");

	char buffer1[100];
	snprintf(buffer1, sizeof(buffer1), "检索个数：%d", num_k);
	ocall_print_string(buffer1);

	Dictionary = LoadDictionaryFromHost("E:\\SCIS\\SCIS_eight\\cloud_CSP\\App_CSP\\dictionary.txt");
	int KEYWORD_NUM = Dictionary.size() + KEY_EXTEND;
	char buffer2[100];
	snprintf(buffer2, sizeof(buffer2), "关键词总数：%d", KEYWORD_NUM);
	ocall_print_string(buffer2);

	int64_t start_time, end_time;
	ocall_QueryPerformanceCounter(&start_time);

	vector<double> queryVector(KEYWORD_NUM, 0.0);
	//ocall_print_string("asfafsasgf");
	//ocall_print_string(keywords_list[0]);
	//ocall_print_int(Dictionary["1969"]);
	//ocall_print_int(keywords_list_size);
	//ocall_print_string("asfafsasgf");
	vector<int> loc;
	double sum = 0;
	for (size_t n = 0; n < keywords_list_size; n++) {
		string keyword(keywords_list[n]); // 将 const char* 转换为 std::string
		auto dou_it = IDFVALUE.find(keyword);
		auto int_it = Dictionary.find(keyword);
		if (dou_it != IDFVALUE.end() && int_it != Dictionary.end()) {
			loc.push_back(Dictionary[keyword]);
			sum += IDFVALUE[keyword] * IDFVALUE[keyword];
			if (Dictionary[keyword] >= 0 && Dictionary[keyword] < queryVector.size()) {
				queryVector[Dictionary[keyword]] = IDFVALUE[keyword];
			}
			else {
				char buffer4[100];
				snprintf(buffer4, sizeof(buffer4), "Error: Index out of range for keyword %s", keyword.c_str());
				ocall_print_string(buffer4);
			}
		}
		else {
			char buffer5[100];
			snprintf(buffer5, sizeof(buffer5), "Caution: Keyword %s is not in dictionary!", keyword.c_str());
			ocall_print_string(buffer5);
		}
	}

	if (loc.empty()) {
		ocall_print_string("Caution: No Keyword you have input is in dictionary!");
	}

	for (int index : loc) {
		queryVector[index] /= sqrt(fabs(sum));
	}

	ARRAY_S = loadVectorBoolFromHost("E:\\SCIS\\SCIS_eight\\cloud_CSP\\App_CSP\\array_s.dat");

	vector<double> randoms(KEYWORD_NUM);
	generate_random_doubles(randoms, KEYWORD_NUM);

	for (int i = 0; i < KEYWORD_NUM; i++) {
		if (!ARRAY_S[i]) {
			double ran = randoms[i];
			queryVector[i] = ran;
		}
	}

	queryVector.resize(KEYWORD_NUM * 2);
	for (int j = 0; j < KEYWORD_NUM; j++) {
		queryVector[j + KEYWORD_NUM] = -queryVector[j];
	}

	ocall_QueryPerformanceCounter_End(&end_time);
	int64_t time = end_time - start_time;
	w_time = to_string(time);
	char buffer3[100];
	snprintf(buffer3, sizeof(buffer3), "陷门生成时间：%s", w_time.c_str());
	ocall_print_string(buffer3);

	return queryVector;
}


extern "C" {
//#include <string.h> // 为了使用 strncpy 和其他字符串操作

	void ecall_prepare_query_vector(const char** keywords_list, size_t keywords_list_size, int k, double* query_vector, size_t query_vector_size, char* w_time, size_t w_time_len) {
		// 打印接收到的关键词列表
		ocall_print_string("Received keywords list:");
		for (size_t i = 0; i < keywords_list_size; ++i) {
			if (keywords_list[i] == nullptr) {
				ocall_print_string("Null pointer found in keywords list");
				return;  // 如果发现 nullptr，输出错误消息并提前返回
			}
			ocall_print_string(keywords_list[i]);
		}

		string w_time_str;
		vector<double> result_vector = prepareQueryVector(keywords_list, keywords_list_size, k, w_time_str);

		// 确保 query_vector 足够大
		for (size_t i = 0; i < query_vector_size && i < result_vector.size(); ++i) {
			query_vector[i] = result_vector[i];
		}

		// 使用 strncpy 并确保安全地添加 null-terminator
		if (w_time_len > 0) {
			strncpy(w_time, w_time_str.c_str(), w_time_len - 1);
			w_time[w_time_len - 1] = '\0';  // 确保总是有 null-terminator
		}
	}
}

sgx_status_t generate_rsa_key_pair_and_export_public_key() {
	sgx_status_t status;
	sgx_rsa3072_key_t rsa_key;
	uint8_t exponent[4] = { 0x01, 0x00, 0x01, 0x00 }; // 公钥指数为 65537
	uint8_t n[SGX_RSA3072_KEY_SIZE];
	uint8_t d[SGX_RSA3072_PRI_EXP_SIZE];
	uint8_t p[SGX_RSA3072_PRI_EXP_SIZE / 2];
	uint8_t q[SGX_RSA3072_PRI_EXP_SIZE / 2];
	uint8_t dmp1[SGX_RSA3072_PRI_EXP_SIZE / 2];
	uint8_t dmq1[SGX_RSA3072_PRI_EXP_SIZE / 2];
	uint8_t iqmp[SGX_RSA3072_PRI_EXP_SIZE / 2];

	// 生成 RSA 密钥对
	status = sgx_create_rsa_key_pair(
		SGX_RSA3072_KEY_SIZE,
		SGX_RSA3072_PUB_EXP_SIZE,
		n, d, exponent,
		p, q, dmp1, dmq1, iqmp
	);

	if (status != SGX_SUCCESS) {
		return status; // 密钥生成失败处理
	}

	// 将公钥通过 OCALL 传递到不受信任的应用
	//ocall_save_public_key(n, SGX_RSA3072_KEY_SIZE);
	// 将公钥通过 OCALL 传递到不受信任的应用
	//uint32_t exp_as_uint32 = (exponent[0] << 24) | (exponent[1] << 16) | (exponent[2] << 8) | exponent[3];
	//ocall_save_public_key(n, SGX_RSA3072_KEY_SIZE, exp_as_uint32);
	ocall_save_public_key(n, sizeof(n), exponent, sizeof(exponent));


	return SGX_SUCCESS;
}












