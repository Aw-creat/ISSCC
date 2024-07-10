#include <stdio.h>
#include <tchar.h>
#include "sgx_urts.h" //本文件中使用的sgx函数的头
#include "enc_KEP_u.h" //不受信任的头文件
#include "enc_COM_u.h"
#include "enc_VER_u.h"
#include <sgx_tcrypto.h>
#include <stdlib.h>
#include "tool.h"
#include <time.h>
//#include "appTool.h"
//#include <winsock2.h>

#include <rsa.h>
#include <files.h>
#include <osrng.h>
#include <filters.h>
#include <base64.h>
#include <sha.h>
#include <streambuf>
#include <queue.h>
#include <hex.h>
#include <iomanip> 
#include <cryptlib.h>
#include <numeric>
#undef min  // Add this line
#include <algorithm>
#include <cmath>

using namespace std;
using namespace CryptoPP;


#define ENCLAVE1_FILE _T("enc_KEP.signed.dll")
#define ENCLAVE2_FILE _T("enc_COM.signed.dll")
#define ENCLAVE3_FILE _T("enc_VER.signed.dll")
//#define ENCLAVE_FILE _T("../Debug/Enclave.signed.dll")
#define MAX_BUF_LEN 100
#define ARRAY_SIZE 1000

// 从文件读取全部内容为字符串
string ReadFile(const string& filename) {
	ifstream file(filename);
	if (!file) {
		throw runtime_error("Cannot open file.");
	}
	stringstream buffer;
	buffer << file.rdbuf();
	string contents = buffer.str();

	// Output raw contents to debug the actual read data
	cout << "Raw read contents (hex): ";
	for (unsigned char c : contents) {
		cout << hex << (int)c << " ";
	}
	cout << endl;

	return contents;
}

bool LoadPublicKeyFromPEM(const string& filename, RSA::PublicKey& publicKey) {
	string fileContents = ReadFile(filename);
	cout << "Read file contents:" << endl << fileContents << endl;  // Debug print
	size_t start = fileContents.find("-----BEGIN PUBLIC KEY-----");
	size_t end = fileContents.find("-----END PUBLIC KEY-----");

	if (start == string::npos || end == string::npos) {
		cerr << "Invalid PEM file.\n";
		return false;
	}

	start += strlen("-----BEGIN PUBLIC KEY-----");
	string base64Content = fileContents.substr(start, end - start);
	base64Content.erase(remove(base64Content.begin(), base64Content.end(), '\n'), base64Content.end()); // Remove newlines

	string derContent;
	StringSource ss(base64Content, true, new Base64Decoder(new StringSink(derContent)));

	ByteQueue queue;
	queue.Put((const byte*)derContent.data(), derContent.size());
	queue.MessageEnd();
	publicKey.Load(queue);

	return true;
}

size_t MaxDataSizeForRSA_OAEP_SHA1(const RSA::PublicKey& key) {
	size_t hashDigestSize = SHA1::DIGESTSIZE;  // SHA-1 散列长度
	size_t modulusSize = key.GetModulus().ByteCount();  // 模数的长度，单位是字节

	// 计算最大明文长度
	return modulusSize - 2 * hashDigestSize - 2;
}

// 使用公钥加密数据
/*string EncryptWithPublicKey(const RSA::PublicKey& key, const string& data) {
	AutoSeededRandomPool rng;
	string cipherText;
	try {
		rng.IncorporateEntropy((byte*)"additional entropy", 20);
	}
	catch (const CryptoPP::Exception& e) {
		cerr << "Random Pool Failed: " << e.what() << endl;
		return false; // 或者其他错误处理逻辑
	}
	byte block[32];
	try {
		rng.GenerateBlock(block, sizeof(block));
		cout << "Random Block: ";
		for (int i = 0; i < sizeof(block); ++i)
			printf("%02X ", block[i]);
		cout << endl;
	}
	catch (const CryptoPP::Exception& e) {
		cerr << "Failed to generate random block: " << e.what() << endl;
		return false; // 或者其他错误处理逻辑
	}

	// 验证公钥是否有效
	if (!key.Validate(rng, 3)) {
		cerr << "Invalid public key." << endl;
		throw runtime_error("Invalid public key.");
	}

// 计算最大可加密数据长度
	size_t maxDataSize = MaxDataSizeForRSA_OAEP_SHA1(key);
	if (data.size() > maxDataSize) {
		cerr << "Data too long for RSA encryption. Max length: " << maxDataSize << " bytes." << endl;
		throw runtime_error("Data too long for RSA encryption.");
	}

	RSAES_OAEP_SHA_Encryptor encryptor(key);

	// 输出待加密的原始数据（为了安全考虑，可能需要在生产环境中删除或修改此行）
	cout << "Encrypting data: " << data << endl;

	StringSource ss(data, true, new PK_EncryptorFilter(rng, encryptor, new StringSink(cipherText))); 

	// 输出加密后的数据的十六进制表示（为了安全考虑，可能需要在生产环境中删除或修改此行）
	cout << "Encrypted data (hex): ";
	StringSource(cipherText, true, new HexEncoder(new FileSink(std::cout)));
	cout << endl;

	return cipherText;
}*/


void ocall_save_public_key(const uint8_t* modulus, size_t modulus_size, const uint8_t* exponent, size_t exponent_size) {
	
	/*// Base64 encode the modulus and exponent
	string encodedModulus, encodedExponent;
	StringSource(modulus, modulus_size, true, new Base64Encoder(new StringSink(encodedModulus), false));
	StringSource(exponent, exponent_size, true, new Base64Encoder(new StringSink(encodedExponent), false));

	// Construct PEM format
	ofstream out("E:\\SCIS\\SCIS_eight\\cloud_CSP\\App_CSP\\public_key.pem");
	out << "-----BEGIN PUBLIC KEY-----\n";
	out << encodedModulus << "\n";
	out << encodedExponent << "\n";
	out << "-----END PUBLIC KEY-----\n";
	out.close();

	cout << "Public key successfully saved to 'public_key.pem'.\n";*/

	// 将模数和指数转换为 Crypto++ Integer 类型
	Integer n(modulus, modulus_size);
	Integer e(exponent, exponent_size);

	// 创建 RSA 公钥对象
	RSA::PublicKey publicKey;
	publicKey.Initialize(n, e);

	// 使用 Crypto++ 的 DER 编码功能
	std::string derEncoded;
	StringSink derSink(derEncoded);
	publicKey.DEREncodePublicKey(derSink);

	// 对 DER 编码的数据进行 Base64 编码
	std::string pemEncoded;
	Base64Encoder encoder(new StringSink(pemEncoded), true /* 行分割 */);
	encoder.Put((const byte*)derEncoded.data(), derEncoded.size());
	encoder.MessageEnd();

	// 构建 PEM 格式的字符串
	std::ostringstream oss;
	oss << "-----BEGIN PUBLIC KEY-----\n";
	oss << pemEncoded;
	oss << "-----END PUBLIC KEY-----\n";

	// 写入文件
	std::ofstream outFile("E:\\SCIS\\SCIS_eight\\cloud_CSP\\App_CSP\\public_key.pem");
	outFile << oss.str();
	outFile.close();

	std::cout << "Public key successfully saved to 'public_key.pem'." << std::endl;
}

bool LoadPublicKey(const string& filename, RSA::PublicKey& publicKey) {
	try {
		// Open file and read all contents into a string
		ifstream file(filename);
		if (!file.is_open()) {
			throw runtime_error("Cannot open file.");
		}

		string fileContents((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

		// Locate the begin and end of the base64 encoded section
		string header = "-----BEGIN PUBLIC KEY-----";
		string footer = "-----END PUBLIC KEY-----";
		size_t startPos = fileContents.find(header);
		size_t endPos = fileContents.find(footer, startPos);
		if (startPos == string::npos || endPos == string::npos) {
			throw runtime_error("Public key header or footer not found.");
		}

		startPos += header.length();
		endPos = fileContents.find(footer); // Re-find the footer to ensure no extra characters.

		string base64Encoded = fileContents.substr(startPos, endPos - startPos);
		base64Encoded.erase(remove(base64Encoded.begin(), base64Encoded.end(), '\n'), base64Encoded.end());
		base64Encoded.erase(remove(base64Encoded.begin(), base64Encoded.end(), '\r'), base64Encoded.end());

		// Decode from base64 to DER
		ByteQueue queue;
		StringSource ss(base64Encoded, true, new Base64Decoder(new Redirector(queue)));

		// Load public key from DER format
		publicKey.BERDecode(queue);
		return true;
	}
	catch (const Exception& e) {
		cerr << "Error loading public key: " << e.what() << endl;
		return false;
	}
	catch (const runtime_error& e) {
		cerr << e.what() << endl;
		return false;
	}
}

// 使用公钥加密字符串
string EncryptWithPublicKey(const RSA::PublicKey& publicKey, const string& plain) {
	AutoSeededRandomPool rng;
	std::string cipher;

	RSAES_OAEP_SHA_Encryptor encryptor(publicKey);
	StringSource ss(plain, true,
		new PK_EncryptorFilter(rng, encryptor,
			new StringSink(cipher)
		) // PK_EncryptorFilter
	); // StringSource

	return cipher;
}

RSA::PublicKey LoadPublicKeyFromBase64(const string& base64Modulus, const string& base64Exponent) {
	using namespace CryptoPP;
	RSA::PublicKey publicKey;

	// Base64 解码模数和指数
	string decodedModulus, decodedExponent;
	StringSource ssM(base64Modulus, true, new Base64Decoder(new StringSink(decodedModulus)));
	StringSource ssE(base64Exponent, true, new Base64Decoder(new StringSink(decodedExponent)));

	// 打印解码后的模数和指数（十六进制）
	std::cout << "Decoded Modulus (hex): ";
	for (unsigned char c : decodedModulus) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c;
	std::cout << std::endl;

	std::cout << "Decoded Exponent (hex): ";
	for (unsigned char c : decodedExponent) std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)c;
	std::cout << std::endl;

	Integer n((const byte*)decodedModulus.data(), decodedModulus.size());
	Integer e((const byte*)decodedExponent.data(), decodedExponent.size());
	std::cout << "Integer e value: " << std::hex << e << std::endl;

	publicKey.Initialize(n, e);

	// 调试输出公钥初始化状态
	std::cout << "Public Key Initialized: n = " << n << ", e = " << e << std::endl;

	return publicKey;
}


vector<string> EncryptKeywords(const CryptoPP::RSA::PublicKey& publicKey, const vector<string>& keywords) {
	vector<string> encryptedKeywords;
	for (const auto& keyword : keywords) {
		cout << "Encrypting keyword: " << keyword << endl;
		string encrypted = EncryptWithPublicKey(publicKey, keyword);
		encryptedKeywords.push_back(encrypted);
		cout << "Encrypted keyword: " << encrypted << endl;
	}
	return encryptedKeywords;
}

string EncryptInteger(const CryptoPP::RSA::PublicKey& publicKey, int k) {
	stringstream ss;
	ss << k;
	return EncryptWithPublicKey(publicKey, ss.str());
}



extern "C" void ocall_loadIDFValues(const char* filename, char** keys, double* weights, size_t count, size_t* actual_count) {
	cout << "Loading IDF values from: " << filename << endl;
	ifstream inFile(filename);
	vector<string> words;
	vector<double> indexes;
	if (inFile.is_open()) {
		string line;
		while (getline(inFile, line)) {
			istringstream iss(line);
			string word;
			double index;
			if (iss >> word >> index) {
				words.push_back(word);
				indexes.push_back(index);
				if (words.size() == count) break;  // 避免超出数组容量
			}
		}
		inFile.close();
		// 将数据复制到输出数组
		*actual_count = words.size();
		for (size_t i = 0; i < words.size(); ++i) {
			// 假设每个 keys[i] 都足够大，至少可以容纳 100 个字符
			// 这里假定 keys 的每个元素至少有 100 个字符的空间
			errno_t err = strncpy_s(keys[i], 100, words[i].c_str(), 99);  // 将最大字符数限制为 99，保留一个位置给 '\0'
			if (err != 0) {
				// 处理错误，例如通过日志记录或设置错误状态
				cerr << "Failed to copy string: " << words[i] << " to keys[" << i << "]" << endl;
			}
			weights[i] = indexes[i];
		}
		// Output the keys and weights
		/*cout << "Loaded keys and weights:" << endl;
		for (size_t i = 0; i < *actual_count; ++i) {
			cout << "Key: " << keys[i] << ", Weight: " << weights[i] << endl;
		}*/

	}
	else {
		cerr << "Unable to open file: " << filename << endl;
		*actual_count = 0;
	}
}


void ocall_print_string(const char* str) {
	cout << str << endl; // 确保没有被重定向
	flush(cout); // 强制刷新输出
}

void ocall_print_size_t(size_t val) {
	cout << val << endl; // 确保没有被重定向
	flush(cout); // 强制刷新输出
}

void ocall_print_int(int val) {
	cout << val << endl; // 确保没有被重定向
	flush(cout); // 强制刷新输出
}

void ocall_QueryPerformanceCounter(int64_t* start_time) {
	LARGE_INTEGER li;
	if (QueryPerformanceCounter(&li)) {
		*start_time = li.QuadPart;
	}
	else {
		*start_time = 0;
	}
}

void ocall_QueryPerformanceCounter_End(int64_t* end_time) {
	LARGE_INTEGER li;
	if (QueryPerformanceCounter(&li)) {
		*end_time = li.QuadPart;
	}
	else {
		*end_time = 0;
	}
}

void ocall_loadDictionary(const char* filename, char** keys, int* indices, size_t count, size_t* actual_count) {

	ifstream inFile(filename);
	vector<string> words;
	vector<int> indexes;
	if (inFile.is_open()) {
		string line;
		while (getline(inFile, line)) {
			istringstream iss(line);
			string word;
			int index;
			if (iss >> word >> index) {
				words.push_back(word);
				indexes.push_back(index);
				if (words.size() == count) break;  // 避免超出数组容量
			}
		}
		inFile.close();
		// 将数据复制到输出数组
		*actual_count = words.size();
		for (size_t i = 0; i < words.size(); ++i) {
			// 假设每个 keys[i] 都足够大，至少可以容纳 100 个字符
			// 这里假定 keys 的每个元素至少有 100 个字符的空间
			errno_t err = strncpy_s(keys[i], 20, words[i].c_str(), 20);  // 将最大字符数限制为 99，保留一个位置给 '\0'
			if (err != 0) {
				// 处理错误，例如通过日志记录或设置错误状态
				cerr << "Failed to copy string: " << words[i] << " to keys[" << i << "]" << endl;
			}
			indices[i] = indexes[i];
		}

	}
	else {
		cerr << "Unable to open file: " << filename << endl;
		*actual_count = 0;
	}
}

void ocall_loadVectorBool(const char* filename, char* data, size_t size, size_t* actual_size) {
	ifstream inFile(filename, ios::binary);
	if (!inFile) {
		cerr << "Failed to open file for reading: " << filename << endl;
		*actual_size = 0;
		return;
	}

	size_t file_size;
	inFile.read(reinterpret_cast<char*>(&file_size), sizeof(file_size));
	if (inFile.fail()) {
		cerr << "Error reading the size of the vector." << endl;
		*actual_size = 0;
		return;
	}

	// Read boolean values as bytes
	inFile.read(data, file_size);
	if (inFile.fail()) {
		cerr << "Error reading data for vector." << endl;
		*actual_size = 0;
	}
	else {
		*actual_size = file_size;
	}

	inFile.close();
}

vector<double> call_ecall_prepare_query_vector(sgx_enclave_id_t eid, const vector<string>& keywords_list, int k) {
	vector<double> query_vector(610);  // 使用std::vector存储查询向量
	char w_time[100];  // 用于接收执行时间字符串
	vector<const char*> c_keywords_list;

	// 将std::vector<std::string>转换为std::vector<const char*>
	for (const auto& keyword : keywords_list) {
		c_keywords_list.push_back(keyword.c_str());
	}

	// ECALL调用
	sgx_status_t status = ecall_prepare_query_vector(eid, c_keywords_list.data(), c_keywords_list.size(), k, query_vector.data(), query_vector.size(), w_time, sizeof(w_time));

	if (status != SGX_SUCCESS) {
		cout << "ECALL failed with error code: " << status << endl;
		return vector<double>();  // 返回一个空的vector以示错误
	}

	// 打印时间
	cout << "陷门生成时间: " << w_time << endl;

	return query_vector;  // 返回填充好的查询向量
}


////////////////////////////////////////////////////////////////////////////////////计算函数
void sum_array(double* arr, size_t len, double* result) {
	double sum = 0.0;
	for (size_t i = 0; i < len; ++i) {
		sum += arr[i];
	}
	*result = sum;
}

void diff_array(double* arr, size_t len, double* result) {
	double diff = 0.0;
	for (size_t i = 0; i < len; ++i) {
		diff -= arr[i];
	}
	*result = diff;
}

void mean_array(double* arr, size_t len, double* result) {
	double mean = 0.0;
	for (size_t i = 0; i < len; ++i) {
		mean += arr[i];
	}
	mean = mean / len;
	*result = mean;
}

void var_array(double* arr, size_t len, double* result) {
	double sum = 0.0, mean = 0.0, var1 = 0.0, var =0;
	for (size_t i = 0; i < len; ++i) {
		sum += arr[i];
	}
	mean = sum / len;
	for (size_t i = 0; i < len; ++i) {
		var1 += (mean - arr[i]) * (mean - arr[i]);
	}
	var = var1 / len;
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

void sort_array(double* arr, size_t len) {
	
	// 调用快速排序算法
	if (len > 0) {
		quick_sort(arr, 0, len - 1);
	}
}

void find_max(double* arr, size_t len, double* max_val) {
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

void find_min(double* arr, size_t len, double* min_val) {
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

void search1_value(double* arr, size_t len, double value, double* found) {
	*found = 0; // 默认设置为未找到
	for (size_t i = 0; i < len; i++) {
		if (arr[i] == value) {
			*found = 1; // 找到值
			break;
		}
	}
}

void linear_regression(double* x_values, double* y_values, size_t x_len, size_t y_len, double* slope, double* intercept) {
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

void linear_regression_one(double* x_values, double* y_values, size_t x_len, size_t y_len, double* slope, double* intercept) {
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

void logistic_regression(double* features, double* labels, size_t n_samples, size_t n_features, double* weights, double* bias) {
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
void removeBOM(std::string& data) {
	if (data.size() >= 3) {
		// 检查前三个字节是否为UTF-8 BOM
		if (static_cast<unsigned char>(data[0]) == 0xEF &&
			static_cast<unsigned char>(data[1]) == 0xBB &&
			static_cast<unsigned char>(data[2]) == 0xBF) {
			// 移除前三个字节
			data = data.substr(3);
		}
	}
}

void readCSV(const char* filename, double* data_array, int* count) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "无法打开文件: " << filename << std::endl;
		*count = 0;
		return;
	}

	std::string line;
	*count = 0;
	if (std::getline(file, line)) {
		removeBOM(line); // 去除可能的BOM
		std::stringstream ss(line);
		std::string token;
		while (std::getline(ss, token, ',')) {
			try {
				data_array[*count] = std::stod(token);
				(*count)++;
			}
			catch (const std::invalid_argument& e) {
				std::cerr << "非法输入，无法转换为数字: " << token << std::endl;
				data_array[*count] = 0; // 可选: 设置默认值
				(*count)++;
			}
		}
	}

	file.close();
}

void ocall_print_stats(const char* msg, double val) {
	printf("%s %f\n", msg, val);
}


/////////////////////////////////////////////////////////////////////AppDemo是创建和调用enclave的应用程序
int main() {

	RBTreeSearch rBTreeSearch;

	vector<string> keywords_list;
	string keywords, time, w_time;
	vector<string> result_list;
	int k; 

	/*cout << "Enter keywords separated by spaces: ";
	getline(cin, keywords);

	cout << "Enter the number of results to retrieve: ";
	cin >> k;
	cin.ignore(); // 清除输入缓冲区中的换行符

	size_t pos = 0; //声明一个 size_t 类型的变量 pos，用于存储字符串中空格的位置
	while ((pos = keywords.find(' ')) != string::npos) { 
		keywords_list.push_back(keywords.substr(0, pos)); //substr 方法截取从字符串开头到空格位置的子字符串
		keywords.erase(0, pos + 1);
	}
	if (!keywords.empty()) {
		keywords_list.push_back(keywords);
	}*/

	
	sgx_enclave_id_t eid1;
	sgx_enclave_id_t eid2;
	sgx_enclave_id_t eid3;
	//sgx_enclave_id_t eid4;
	sgx_status_t ret1 = SGX_SUCCESS;
	sgx_status_t ret2 = SGX_SUCCESS;
	sgx_status_t ret3 = SGX_SUCCESS;
	sgx_status_t ret4 = SGX_SUCCESS;
	sgx_launch_token_t token1 = { 0 };
	sgx_launch_token_t token2 = { 0 };
	sgx_launch_token_t token3 = { 0 };
	int updated1 = 0;
	int updated2 = 0;
	int updated3 = 0;
	char buffer1[MAX_BUF_LEN] = "Hello World!";
	char buffer2[MAX_BUF_LEN] = "Hello World!";
	char buffer3[MAX_BUF_LEN] = "Hello World!";
	sgx_status_t ecall_status, status;


	// Create the Enclave1 with above launch token.
	ret1 = sgx_create_enclave(ENCLAVE1_FILE, SGX_DEBUG_FLAG, &token1, &updated1, &eid1, NULL);
	if (ret1 != SGX_SUCCESS) {
		printf("App: error %#x, failed to create enclave1.\n", ret1);
		return -1;
	}
	// Create the Enclave2 with above launch token.
	ret2 = sgx_create_enclave(ENCLAVE2_FILE, SGX_DEBUG_FLAG, &token2, &updated2, &eid2, NULL);
	if (ret2 != SGX_SUCCESS) {
		printf("App: error %#x, failed to create enclave2.\n", ret2);
		return -1;
	}

	// Create the Enclave3 with above launch token.
	ret3 = sgx_create_enclave(ENCLAVE3_FILE, SGX_DEBUG_FLAG, &token3, &updated3, &eid3, NULL);
	if (ret3 != SGX_SUCCESS) {
		printf("App: error %#x, failed to create enclave3.\n", ret3);
		return -1;
	}

	// 调用RSA公钥加密
	/*status = generate_rsa_key_pair_and_export_public_key(eid1, &ecall_status);

	if (status != SGX_SUCCESS) {
		printf("ECALL failed: %d\n", status);
	}

	if (ecall_status != SGX_SUCCESS) {
		printf("Enclave operation failed: %d\n", ecall_status);
	}*/

	// Optionally load back the keywords for verification or further processing
	loadKeywords(keywords_list, k);
	cout << "Loaded keywords: ";
	for (const auto& word : keywords_list) {
		cout << word << " ";
	}
	cout << "\nNumber of results to retrieve: " << k << endl;

	/*vector<string> encryptedKeywords;
	RSA::PublicKey publicKey;
	if (!LoadPublicKey("E:\\SCIS\\SCIS_eight\\cloud_CSP\\App_CSP\\public_key.pem", publicKey)) {
		cerr << "Failed to load public key!" << endl;
		return -1;
	}

	// 加密关键词列表
	for (auto& keyword : keywords_list) {
		string encrypted = EncryptWithPublicKey(publicKey, keyword);
		encryptedKeywords.push_back(encrypted);
	}

	// 显示加密后的关键词
	for (auto& encryptedKeyword : encryptedKeywords) {
		cout << "Encrypted: " << encryptedKeyword << endl;
	}


	/*ifstream file("E:\\SCIS\\SCIS_eight\\cloud_CSP\\App_CSP\\public_key.pem");
	string line, base64Modulus, base64Exponent;
	getline(file, line); // Skip BEGIN line
	getline(file, base64Modulus);
	getline(file, base64Exponent);
	file.close();
	RSA::PublicKey publicKey = LoadPublicKeyFromBase64(base64Modulus, base64Exponent);
	vector<string> encryptedKeywords = EncryptKeywords(publicKey, keywords_list);
	string encryptedInt = EncryptInteger(publicKey, k);

	//Output the encrypted data
	cout << "Encrypted Keywords:" << endl;
	for (const auto& encryptedKeyword : encryptedKeywords) {
		cout << encryptedKeyword << endl;
	}
	cout << "Encrypted Integer: " << encryptedInt << endl;*/

	//vector<double> queryVector = rBTreeSearch.prepareQueryVector(keywords_list, k, w_time);
	// 调用 ECALL
    //call_ecall_prepare_query_vector(eid1, keywords_list, k);
	vector<double> queryVector = call_ecall_prepare_query_vector(eid1, keywords_list, k);
	cout << "Query vector: ";
	for (double val : queryVector) {
		cout << val << " ";
	}
	cout << endl;

	cout << "Query vector size: " << queryVector.size() << endl;

	// 调用 startSearch 方法进行搜索
	//rBTreeSearch.startSearch(keywords_list, time, result_list, k, w_time);
	//rBTreeSearch.startSearch(query_vector, time, result_list);
	result_list = rBTreeSearch.startSearch(queryVector, time);

	for (size_t i = 0; i < result_list.size(); i += 2) {
		cout << "Result " << i / 2 + 1 << ":" << endl;
		cout << "File Name: " << result_list[i] << endl;
		cout << "Score: " << result_list[i + 1] << endl;
	}
	cout << "搜索时间: " << time << endl;
	//cout << "陷门生成时间: " << w_time << endl;


	// An Enclave call (ECALL) will happen here.该函数在enclave1.edl中定义
	foo_on(eid1, buffer1, MAX_BUF_LEN);
	printf("%s\n", buffer1);
	foo_tw(eid2, buffer2, MAX_BUF_LEN);
	printf("%s\n", buffer2);
	foo_th(eid3, buffer3, MAX_BUF_LEN);
	printf("%s\n", buffer3);


		// Destroy the enclave when all Enclave calls finished.
	if (SGX_SUCCESS != sgx_destroy_enclave(eid1))
		return -1;
///////////////////////////////////////////////////////计算任务
	double result = 0.0;
	double data[ARRAY_SIZE];
	double search_value = 5.0; // 假设查找值
	int found = 0;

	// 初始化随机数种子并生成随机数
	std::srand(static_cast<unsigned int>(std::time(nullptr)));
	for (int i = 0; i < ARRAY_SIZE; ++i) {
		data[i] = static_cast<double>(std::rand()) / RAND_MAX * 1000.0; // 生成范围在0到1000之间的随机double数
	}
	///////////////////////////////////////////////////////////求和
	cout << "和—————————————————————————" << endl;
	auto start_sum = std::chrono::high_resolution_clock::now();
	ret2 = ecall_sum_array(eid2, data, ARRAY_SIZE, &result);
	auto end_sum = std::chrono::high_resolution_clock::now();
	
	if (ret2 != SGX_SUCCESS) {
		printf("ECALL failed\n");
	}
	else {
		printf("Sum: %lf\n", result);
	}
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> sum_time = end_sum - start_sum;
	printf("Time sum_trust: %lf ms\n", sum_time.count());


	auto start_sum1 = std::chrono::high_resolution_clock::now();
	sum_array(data, ARRAY_SIZE, &result);
	auto end_sum1 = std::chrono::high_resolution_clock::now();

	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> sum_time1 = end_sum1 - start_sum1;
	printf("Sum: %lf\n", result);
	printf("Time sum: %lf ms\n", sum_time1.count());

	/////////////////////////////////////////////////////求差
	cout << "差—————————————————————————" << endl;
	auto start_diff = std::chrono::high_resolution_clock::now();
	ret2 = ecall_diff_array(eid2, data, ARRAY_SIZE, &result);
	auto end_diff = std::chrono::high_resolution_clock::now();

	if (ret2 != SGX_SUCCESS) {
		printf("ECALL failed\n");
	}
	else {
		printf("Diff: %lf\n", result);
	}
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> diff_time = end_diff - start_diff;
	printf("Time diff_trust: %lf ms\n", diff_time.count());


	auto start_diff1 = std::chrono::high_resolution_clock::now();
	diff_array(data, ARRAY_SIZE, &result);
	auto end_diff1 = std::chrono::high_resolution_clock::now();

	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> diff_time1 = end_diff1 - start_diff1;
	printf("Diff: %lf\n", result);
	printf("Time diff: %lf ms\n", diff_time1.count());


	/////////////////////////////////////////////////////求均值
	cout << "均值—————————————————————————" << endl;
	auto start_mean = std::chrono::high_resolution_clock::now();
	ret2 = ecall_mean_array(eid2, data, ARRAY_SIZE, &result);
	auto end_mean = std::chrono::high_resolution_clock::now();

	if (ret2 != SGX_SUCCESS) {
		printf("ECALL failed\n");
	}
	else {
		printf("Mean: %lf\n", result);
	}
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> mean_time = end_mean - start_mean;
	printf("Time mean_trust: %lf ms\n", mean_time.count());


	auto start_mean1 = std::chrono::high_resolution_clock::now();
	mean_array(data, ARRAY_SIZE, &result);
	auto end_mean1 = std::chrono::high_resolution_clock::now();

	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> mean_time1 = end_mean1 - start_mean1;
	printf("Mean: %lf\n", result);
	printf("Time mean: %lf ms\n", mean_time1.count());

	/////////////////////////////////////////////////////求方差
	cout << "方差—————————————————————————" << endl;
	auto start_var = std::chrono::high_resolution_clock::now();
	ret2 = ecall_var_array(eid2, data, ARRAY_SIZE, &result);
	auto end_var = std::chrono::high_resolution_clock::now();

	if (ret2 != SGX_SUCCESS) {
		printf("ECALL failed\n");
	}
	else {
		printf("Var: %lf\n", result);
	}
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> var_time = end_var - start_var;
	printf("Time var_trust: %lf ms\n", var_time.count());


	auto start_var1 = std::chrono::high_resolution_clock::now();
	var_array(data, ARRAY_SIZE, &result);
	auto end_var1 = std::chrono::high_resolution_clock::now();

	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> var_time1 = end_var1 - start_var1;
	printf("Var: %lf\n", result);
	printf("Time var: %lf ms\n", var_time1.count());


	////////////////////////////////////////////////////////////////快速排序
	cout << "快速排序—————————————————————————" << endl;
	auto start_sort = std::chrono::high_resolution_clock::now();
	ret2 = ecall_sort_array(eid2, data, ARRAY_SIZE);
	auto end_sort = std::chrono::high_resolution_clock::now();

	if (ret2 != SGX_SUCCESS) {
		printf("ECALL failed\n");
	}
	else {
		printf("Sorted array:");
		/*for (size_t i = 0; i < ARRAY_SIZE; ++i) {
			std::cout << data[i] << " ";
		}
		std::cout << std::endl;*/
	}
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> sort_time = end_sort - start_sort;
	printf("Time sort_trust: %lf ms\n", sort_time.count());

	auto start_sort1 = std::chrono::high_resolution_clock::now();
	sort_array(data, ARRAY_SIZE);
	auto end_sort1 = std::chrono::high_resolution_clock::now();

	/*std::cout << "Sorted array: ";
	for (size_t i = 0; i < ARRAY_SIZE; ++i) {
		std::cout << data[i] << " ";
	}
	std::cout << std::endl;*/ 
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> sort_time1 = end_sort1 - start_sort1;
	printf("Time sort: %lf ms\n", sort_time1.count());

	////////////////////////////////////////////////////////////////求最大值
	cout << "最大值—————————————————————————" << endl;
	auto start_max = std::chrono::high_resolution_clock::now();
	ret2 = ecall_find_max(eid2, data, ARRAY_SIZE, &result);
	auto end_max = std::chrono::high_resolution_clock::now();

	if (ret2 != SGX_SUCCESS) {
		printf("ECALL failed\n");
	}
	else {
		printf("Maximum value in the array:%lf\n", result);

	}
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> max_time = end_max - start_max;
	printf("Time max_trust: %lf ms\n", max_time.count());

	auto start_max1 = std::chrono::high_resolution_clock::now();
	find_max(data, ARRAY_SIZE, &result);
	auto end_max1 = std::chrono::high_resolution_clock::now();

	std::cout << "Maximum value in the array:  " << result << std::endl;
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> max_time1 = end_max1 - start_max1;
	printf("Time max: %lf ms\n", max_time1.count());

	////////////////////////////////////////////////////////////////求最小值
	cout << "最小值—————————————————————————" << endl;
	auto start_min = std::chrono::high_resolution_clock::now();
	ret2 = ecall_find_min(eid2, data, ARRAY_SIZE, &result);
	auto end_min = std::chrono::high_resolution_clock::now();

	if (ret2 != SGX_SUCCESS) {
		printf("ECALL failed\n");
	}
	else {
		printf("Maximum value in the array:%lf\n", result);

	}
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> min_time = end_min - start_min;
	printf("Time min_trust: %lf ms\n", min_time.count());

	auto start_min1 = std::chrono::high_resolution_clock::now();
	find_min(data, ARRAY_SIZE, &result);
	auto end_min1 = std::chrono::high_resolution_clock::now();

	std::cout << "Minimum value in the array:  " << result << std::endl;
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> min_time1 = end_min1 - start_min1;
	printf("Time min: %lf ms\n", min_time1.count());

	/////////////////////////////////////////////////////查找
	cout << "查找—————————————————————————" << endl;
	auto start_search = std::chrono::high_resolution_clock::now();
	ret2 = ecall_search_value(eid2, data, ARRAY_SIZE, search_value, &result);
	auto end_search = std::chrono::high_resolution_clock::now();

	if (ret2 != SGX_SUCCESS) {
		printf("ECALL failed\n");
	}
	else {
		printf("Search: %lf\n", result);
	}
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> search_time = end_search - start_search;
	printf("Time search_trust: %lf ms\n", search_time.count());


	auto start_search1 = std::chrono::high_resolution_clock::now();
	search1_value(data, ARRAY_SIZE, search_value, &result);
	auto end_search1 = std::chrono::high_resolution_clock::now();

	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> search_time1 = end_search1 - start_search1;
	printf("Search: %lf\n", result);
	printf("Time search: %lf ms\n", search_time1.count());

	/////////////////////////////////////////////////////线性回归
	cout << "线性回归—————————————————————————" << endl;
	std::vector<double> x_values(1000);
	std::vector<double> y_values(1000);
	// 初始化数据
	for (int i = 0; i < 1000; ++i) {
		x_values[i] = i;
		y_values[i] = 2.0 * i + 10; // 示例线性关系: y = 2x + 10
	}

	double slope = 0.0, intercept = 0.0;
	auto start_linear = std::chrono::high_resolution_clock::now();
	ret2 = ecall_linear_regression(eid2, x_values.data(), y_values.data(), x_values.size(), y_values.size(), &slope, &intercept);
	auto end_linear = std::chrono::high_resolution_clock::now();
	if (ret2 != SGX_SUCCESS) {
		std::cerr << "ECALL failed: " << std::hex << ret2 << std::endl;
		sgx_destroy_enclave(eid2);
		return -1;
	}
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> linear_time = end_linear - start_linear;
	std::cout << "Linear Regression results: Slope = " << slope << ", Intercept = " << intercept << std::endl;
	printf("Time linear_trust: %lf ms\n", linear_time.count());
	

	auto start_linear1 = std::chrono::high_resolution_clock::now();
	linear_regression(x_values.data(), y_values.data(), x_values.size(), y_values.size(), &slope, &intercept);
	auto end_linear1 = std::chrono::high_resolution_clock::now();

	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> linear_time1 = end_linear1 - start_linear1;
	std::cout << "Linear Regression results: Slope = " << slope << ", Intercept = " << intercept << std::endl;
	printf("Time linear: %lf ms\n", linear_time1.count());

	///////////////////////////////////////////////////////////////////////线性回归1000
	cout << "线性回归1000—————————————————————————" << endl;
	std::vector<double> x1_values(1000);
	std::vector<double> y1_values(1000);
	std::srand(static_cast<unsigned int>(std::time(nullptr)));

	double true_slope = 2.0;
	double true_intercept = 5.0;

	// 生成随机数据
	for (int i = 0; i < 1000; ++i) {
		x1_values[i] = i;
		y1_values[i] = true_slope * i + true_intercept + (std::rand() % 100) / 10.0; // 加入随机噪声
	}

	double slope1 = 0.0, intercept1 = 0.0;
	auto start_lin = std::chrono::high_resolution_clock::now();
	ret2 = ecall_linear_regression_one(eid2, x1_values.data(), y1_values.data(), x1_values.size(), y1_values.size(), &slope1, &intercept1);
	auto end_lin = std::chrono::high_resolution_clock::now();
	if (ret2 != SGX_SUCCESS) {
		std::cerr << "ECALL failed: " << std::hex << ret2 << std::endl;
		sgx_destroy_enclave(eid2);
		return -1;
	}
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> lin_time = end_lin - start_lin;
	std::cout << "Linear Regression results: Slope = " << slope1 << ", Intercept = " << intercept1 << std::endl;
	printf("Time lin_trust: %lf ms\n", lin_time.count());

	auto start_lin1 = std::chrono::high_resolution_clock::now();
	linear_regression_one(x1_values.data(), y1_values.data(), x1_values.size(), y1_values.size(), &slope1, &intercept1);
	auto end_lin1 = std::chrono::high_resolution_clock::now();

	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> lin_time1 = end_lin1 - start_lin1;
	std::cout << "Linear Regression results: Slope = " << slope1 << ", Intercept1 = " << intercept << std::endl;
	printf("Time lin: %lf ms\n", lin_time1.count());

	///////////////////////////////////////////////////////////////////////逻辑回归1000
	cout << "逻辑回归1000—————————————————————————" << endl;
	const size_t n_samples = 1000, n_features = 1000;
	std::vector<double> features(n_samples * n_features);
	std::vector<double> labels(n_samples);
	std::vector<double> weights(n_features);
	double bias;

	// 假设为了简化，我们直接生成一些随机数据来模拟特征和标签
	for (size_t i = 0; i < features.size(); i++) {
		features[i] = static_cast<double>(rand()) / RAND_MAX;
	}
	for (size_t i = 0; i < labels.size(); i++) {
		labels[i] = rand() % 2;  // 0或1的标签
	}


	auto start_log = std::chrono::high_resolution_clock::now();
	ret2 = ecall_logistic_regression(eid2, features.data(), labels.data(), n_samples, n_features, weights.data(), &bias);
	auto end_log = std::chrono::high_resolution_clock::now();
	
	if (ret2 != SGX_SUCCESS) {
		std::cerr << "ECALL failed: " << std::hex << ret2 << std::endl;
		sgx_destroy_enclave(eid2);
		return -1;
	}
	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> log_time = end_log - start_log;
	/*std::cout << "Learned weights: ";
	for (double w : weights) {
		std::cout << w << " ";
	}
	std::cout << "\nBias: " << bias << std::endl;*/
	printf("Time log_trust: %lf ms\n", log_time.count());

	auto start_log1 = std::chrono::high_resolution_clock::now();
	logistic_regression(features.data(), labels.data(), n_samples, n_features, weights.data(), &bias);
	auto end_log1 = std::chrono::high_resolution_clock::now();

	// 计算并输出时间（毫秒）
	std::chrono::duration<double, std::milli> log_time1 = end_log1 - start_log1;
	/*std::cout << "Learned weights: ";
	for (double w : weights) {
		std::cout << w << " ";
	}
	std::cout << "\nBias: " << bias << std::endl;*/
	printf("Time log: %lf ms\n", log_time1.count());

	/////////////////////////////////////////////////////////////////////////////////计算结束
	auto start_test = std::chrono::high_resolution_clock::now();
	double data_marray[1024], data_farray[1024]; // 假设数组足够大，可以存放所有读取的数据
	int mcount = 0, fcount=0;

	readCSV("E:\\SCIS\\SCIS_eight\\dataset\\male1.csv", data_marray, &mcount);
	readCSV("E:\\SCIS\\SCIS_eight\\dataset\\female1.csv", data_farray, &fcount);

	/*std::cout << "读取的数值个数: " << mcount << std::endl;
	std::cout << "数值列表: ";
	for (int i = 0; i < mcount; i++) {
		std::cout << data_marray[i] << " ";
	}
	std::cout << std::endl;*/

	double m_sum = 0, f_sum = 0, m_sq=0, f_sq=0;
	mean_array(data_marray, mcount, &m_sum);
	mean_array(data_farray, fcount, &f_sum);
	std::cout << "男均值: " << m_sum << std::endl;
	std::cout << "女均值: " << f_sum << std::endl;

	var_array(data_marray, mcount, &m_sq);
	var_array(data_farray, fcount, &f_sq);

	std::cout << "男方差: " << m_sq << std::endl;
	std::cout << "女方差: " << f_sq << std::endl;

	double t = (m_sum - f_sum)/(sqrt(abs(m_sq/mcount + f_sq/ fcount)));
	double v = ( (sqrt(abs(m_sq / mcount + f_sq / fcount))) * (sqrt(abs(m_sq / mcount + f_sq / fcount))) )/ ((m_sq / mcount)* (m_sq / mcount) / (mcount - 1) + (f_sq / fcount)*(f_sq / fcount)/ (fcount -1));
	std::cout << "t: " << t << std::endl;
	std::cout << "v: " << v << std::endl;
	auto end_test = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> test_time = end_test - start_test;
	printf("Time t-test: %lf ms\n", test_time.count());

	auto start_test1 = std::chrono::high_resolution_clock::now();
	ecall_stats_analysis(eid2, data_marray, mcount, data_farray, fcount);
	auto end_test1 = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> test1_time = end_test1 - start_test1;
	printf("Time t-test_trust: %lf ms\n", test1_time.count());
	// 销毁enclave
	sgx_destroy_enclave(eid2);
	return 0;




	//与数据使用者端建立连接
	//socket_connection(Favg, Mavg, Fcount, Mcount, Fvar_sq, Mvar_sq);


	return 0;
}