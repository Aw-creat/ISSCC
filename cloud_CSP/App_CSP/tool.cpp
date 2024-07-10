#include <map>
#include <queue>
#include <random>
#include <vector>
#include <string>
#include "tool.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <iomanip> // 添加这个头文件
#include <stack>
#include <functional>

#include <cereal/types/memory.hpp>  // 用于智能指针的序列化
#include <cereal/types/vector.hpp>  // 用于容器的序列化
#include <cereal/archives/binary.hpp> // 用于二进制归档
#include <cereal/cereal.hpp>

using namespace std;
map<string, int> Dictionary;
map<string, double> IDFVALUE;
shared_ptr<RBTreeNode> RBTreeRoot;
shared_ptr<RBTreeNode> RBTreeRoot_Enc;
double K_TH_SCORE;
int KEYWORD_NUM;
int num_k;    //the value of k
vector<double> queryVector;

priority_queue<shared_ptr<RBTreeNode>, vector<shared_ptr<RBTreeNode>>, cmp> Top_K_Queue;
priority_queue<shared_ptr<RBTreeNode>, vector<shared_ptr<RBTreeNode>>, cmp> Whole_Queue;//queue without cutting
vector<bool> ARRAY_S;

RBTreeSearch::RBTreeSearch()
{

}
RBTreeSearch::~RBTreeSearch()
{

}

// Function to read a vector<bool> from a file
/*void loadVectorBool(vector<bool>& vec, const string& filename) {
	ifstream inFile(filename, ios::binary);
	if (!inFile) {
		cerr << "Failed to open file for reading: " << filename << endl;
		return;
	}

	// Read the size of the vector
	size_t size;
	inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	if (inFile.fail()) {
		cerr << "Error reading the size of the vector." << endl;
		return;
	}

	// Resize the vector to the saved size
	vec.resize(size);

	// Read each boolean value from the file
	for (size_t i = 0; i < size; ++i) {
		char byte;
		inFile.read(&byte, sizeof(byte));
		if (inFile.fail()) {
			cerr << "Error reading data for vector element." << endl;
			vec.resize(i); // Resize to the actual number of elements read
			break;
		}
		vec[i] = static_cast<bool>(byte);
	}

	inFile.close();
}*/

void loadKeywords(vector<string>& keywords_list, int& k) {
	ifstream inFile("E:\\SCIS\\SCIS_eight\\cloud_CSP\\App_CSP\\keywords_data.txt");
	string line;
	keywords_list.clear();

	while (getline(inFile, line)) {
		keywords_list.push_back(line);
	}

	// The last line is k, so extract it and remove from list
	if (!keywords_list.empty()) {
		k = stoi(keywords_list.back());
		keywords_list.pop_back();
	}
}


/*void RBTreeSearch::startSearch(vector<double> queryVector, string &time, vector<string> &result_list) {
	while (!Top_K_Queue.empty()) {
		Top_K_Queue.pop();
	}
	cout << "Size of queryVector before node_scoreCal: " << queryVector.size() << endl;

	// Load the encrypted RBTree before starting the search
	LoadEncryptedTree("E:\\SCIS\\SCIS_seven\\cloud_CSP\\App_CSP\\encrypted_tree.dat");

	K_TH_SCORE = 0;
	////////////////////////////////////////正式开始搜索
	LARGE_INTEGER m_liPerfFreq = { 0 }, m_liPerfStart = { 0 }, liPerfNow = { 0 };
	QueryPerformanceFrequency(&m_liPerfFreq);
	QueryPerformanceCounter(&m_liPerfStart);

	RBTreeRoot->node_scoreCal(queryVector);
	SearchRBTree(RBTreeRoot); // search function

	QueryPerformanceCounter(&liPerfNow);
	double run_time = (double)(liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000 / (double)m_liPerfFreq.QuadPart;
	stringstream ss2;
	ss2 << fixed << setprecision(4) << run_time;
	time = ss2.str();
	///////////////////////////////////////////计时结束

	priority_queue<shared_ptr<RBTreeNode>, vector<shared_ptr<RBTreeNode>>, cmp> temp = Top_K_Queue;
	vector<string> listResult;
	while (!temp.empty()) {
		shared_ptr<RBTreeNode> node = temp.top();
		string filename = node->fileName;
		string score;
		stringstream score_ss;
		score_ss << fixed << setprecision(6) << node->score;
		score = score_ss.str();
		listResult.push_back(filename);
		listResult.push_back(score);
		temp.pop();
	}

	result_list.clear();
	for (int i = listResult.size() - 1; i >= 0; i -= 2) {
		result_list.push_back(listResult[i - 1]);
		result_list.push_back(listResult[i]);
	}
	//vector<string> Result = result_list;
	////////////////////////////////////////

	// 执行搜索结果后续处理
}*/

vector<string> RBTreeSearch::startSearch(vector<double> queryVector, string &time) {
	while (!Top_K_Queue.empty()) {
		Top_K_Queue.pop();
	}
	cout << "Size of queryVector before node_scoreCal: " << queryVector.size() << endl;

	LoadEncryptedTree("E:\\SCIS\\SCIS_eight\\cloud_CSP\\App_CSP\\encrypted_tree.dat");

	K_TH_SCORE = 0;
	////////////////////////////////////////正式开始搜索
	LARGE_INTEGER m_liPerfFreq = { 0 }, m_liPerfStart = { 0 }, liPerfNow = { 0 };
	QueryPerformanceFrequency(&m_liPerfFreq);
	QueryPerformanceCounter(&m_liPerfStart);

	RBTreeRoot->node_scoreCal(queryVector);
	SearchRBTree(RBTreeRoot, queryVector);  // 搜索红黑树

	QueryPerformanceCounter(&liPerfNow);
	double run_time = (double)(liPerfNow.QuadPart - m_liPerfStart.QuadPart) * 1000 / (double)m_liPerfFreq.QuadPart;
	stringstream ss;
	ss << fixed << setprecision(4) << run_time;
	time = ss.str();
	///////////////////////////////////////////计时结束

	priority_queue<shared_ptr<RBTreeNode>, vector<shared_ptr<RBTreeNode>>, cmp> temp = Top_K_Queue;
	vector<string> listResult;
	while (!temp.empty()) {
		shared_ptr<RBTreeNode> node = temp.top();
		string filename = node->fileName;
		string score;
		stringstream score_ss;
		score_ss << fixed << setprecision(6) << node->score;
		score = score_ss.str();
		//result_list.push_back(filename + ": " + score);  // 这里简化了结果的存储方式
		listResult.push_back(filename);
		listResult.push_back(score);
		temp.pop();
	}
	vector<string> result_list;
	result_list.clear();
	for (int i = listResult.size() - 1; i >= 0; i -= 2) {
		result_list.push_back(listResult[i - 1]);
		result_list.push_back(listResult[i]);
	}

	vector<string> Result = result_list;
	return Result;
}


void RBTreeSearch::SearchRBTree(shared_ptr<RBTreeNode> node, const vector<double>& queryVector)
{   string score;
	K_TH_SCORE = 0;
	cout << "Entering function SearchRBTree, queryVector size: " << queryVector.size() << endl;
	unordered_set<RBTreeNode*> visited;  // 记录已访问的节点
	stack<shared_ptr<RBTreeNode>> stack;
	stack.push(node);

	while (!stack.empty()) {
		auto node = stack.top();
		stack.pop();

		// 防止重复处理同一个节点
		if (visited.find(node.get()) != visited.end()) continue;
		visited.insert(node.get());

		if (!node->tag) { // 内部节点
			node->node_scoreCal(queryVector);  // 确保当前节点得分已计算
			cout << "Processing node. Tag: Internal, Score: " << node->score << endl;

			if (node->score > K_TH_SCORE) {
				auto leftNode = node->leftChild;
				auto rightNode = node->rightChild;

				leftNode->node_scoreCal(queryVector);
				rightNode->node_scoreCal(queryVector);

				// 按分数排序将节点压入栈，高分数优先
				if (leftNode->score >= rightNode->score) {
					stack.push(rightNode);
					stack.push(leftNode);
				}
				else {
					stack.push(leftNode);
					stack.push(rightNode);
				}
			}
		}
		else { // 叶子节点
			if (node->score > 0) {
				cout << "Processing leaf node. Score: " << node->score << endl;
				Updata_Top_K_List(node);
			}
		}
	}
	cout << "Exiting function SearchRBTree, queryVector size: " << queryVector.size() << endl;
}



void RBTreeSearch::Updata_Top_K_List(shared_ptr<RBTreeNode> node) {
	if (Top_K_Queue.size() < num_k - 1) {
		Top_K_Queue.push(node);
		K_TH_SCORE = 0;
	}
	else if (Top_K_Queue.size() == num_k - 1) {
		Top_K_Queue.push(node);
		shared_ptr<RBTreeNode> least_node = Top_K_Queue.top();
		K_TH_SCORE = least_node->score;
	}
	else if (Top_K_Queue.size() == num_k) {
		shared_ptr<RBTreeNode> least_node = Top_K_Queue.top();
		if (node->score > least_node->score) {
			Top_K_Queue.pop();
			Top_K_Queue.push(node);
			shared_ptr<RBTreeNode> new_least = Top_K_Queue.top();
			K_TH_SCORE = new_least->score; // Ensure K_TH_SCORE is updated with the new top element's score
		}
	}
}


void RBTreeSearch::GenRand(vector<vector<double>>& arr, int seed)
{
	mt19937 rng(seed);
	uniform_real_distribution<double> dis(0.0, 1.0);

	for (size_t i = 0; i < arr.size(); ++i)
	{
		for (size_t j = 0; j < arr[i].size(); ++j)
		{
			arr[i][j] = dis(rng);
		}
	}
}

void RBTreeSearch::MatrixToVector(double* data, int rows, int cols, vector<vector<double>>& Matrix)
{
	for (int i = 0; i < rows; ++i)
	{
		vector<double> temp;
		for (int j = 0; j < cols; ++j)
		{
			temp.push_back(data[i * cols + j]);
		}
		Matrix.push_back(temp);
	}
}

void RBTreeSearch::LoadEncryptedTree(const string& filename) {
	ifstream is(filename, ios::binary);
	if (!is) {
		cerr << "Failed to open encrypted tree file: " << filename << endl;
		return;
	}

	cereal::BinaryInputArchive archive(is); // 创建二进制输入存档
	try {
		archive(RBTreeRoot);  // 反序列化RBTreeRoot
	}
	catch (const cereal::Exception& e) {
		cerr << "Cereal deserialization error: " << e.what() << endl;
	}
}

vector<double> RBTreeSearch::prepareQueryVector(vector<string> keywords_list, int k, string &w_time) {
	string path = "allWordsIDF.txt";
	num_k = k;

	map<string, double>::iterator dou_it;
	// 调用新的 loadIDFValues 函数
	loadIDFValues(path);

	cout << "检索个数：" << num_k << endl;
	for (const auto& keyword : keywords_list) {
		cout << "keyword in keyword_list:" << keyword << endl;
	}

	///////////////////////////////////////////初始化查询向量和top-k队列
	///////////////////////////////////////////初始化一些变量
	LoadDictionary("dictionary.txt");
	cout << "Dictionary size: " << Dictionary.size() << endl;
	// 打印 Dictionary 的内容
	cout << "Dictionary contents:" << endl;
	for (const auto& entry : Dictionary) {
		cout << "Word: " << entry.first << ", Index: " << entry.second << endl;
	}

	KEYWORD_NUM = Dictionary.size() + KEY_EXTEND;
	string keywordNum = to_string(KEYWORD_NUM);
	cout << "keywordNum = " << keywordNum << endl;

	LARGE_INTEGER m_Freq = { 0 }, m_Start = { 0 }, m_Now = { 0 }; //LARGE_INTEGER 结构体存储高精度时间值
	QueryPerformanceFrequency(&m_Freq);//获得每秒的计时单位数，用于转换性能计数器的值。
	QueryPerformanceCounter(&m_Start);//获取开始时间，用于后续计算总运行时间

	queryVector.clear();
	queryVector.resize(KEYWORD_NUM, 0.0);

	vector<int> loc;
	double sum = 0;
	map<string, int>::iterator int_it;
	for (int n = 0; n < keywords_list.size(); n++) {
		cout << "Checking keyword: " << keywords_list[n] << endl;
		dou_it = IDFVALUE.find(keywords_list[n]);//使用迭代器 dou_it 从 IDFVALUE中查找关键词的IDF值
		int_it = Dictionary.find(keywords_list[n]);//使用int_it 从Dictionary 中查找关键词的索引
		if (dou_it != IDFVALUE.end() && int_it != Dictionary.end()) {
			loc.push_back(Dictionary[keywords_list[n]]);
			sum += double(IDFVALUE[keywords_list[n]] * IDFVALUE[keywords_list[n]]);
			cout << "Size of queryVector before assigning values: " << queryVector.size() << endl;
			if (Dictionary[keywords_list[n]] >= 0 && Dictionary[keywords_list[n]] < queryVector.size()) {
				queryVector[Dictionary[keywords_list[n]]] = IDFVALUE[keywords_list[n]];
			}
			else {
				cout << "Error: Index out of range for keyword " << keywords_list[n] << endl;
			}
			//cout << "Size of queryVector after assigning values: " << queryVector.size() << endl;
			cout << "Keyword " << keywords_list[n] << " is in dictionary!" << endl;
		}
		else {
			cout << "Caution: Keyword " << keywords_list[n] << " is not in dictionary!" << endl;
			continue;
		}
	}
	//cout << "Size of queryVector after assigning values: " << queryVector.size() << endl;
	if (loc.size() == 0) {
		cout << "Caution: No Keyword you have input is in dictionary!" << endl;
		//return;
	}
	for (int c = 0; c < loc.size(); c++) {
		queryVector[loc[c]] /= sqrt(abs(sum));
	}

	// Load the vector from file
	loadVectorBool(ARRAY_S, "array_s.dat");

	vector<double> queryVector_later = queryVector;
	int seed = GetTickCount();
	default_random_engine rng(seed);
	for (int i = 0; i < KEYWORD_NUM; i++) {
		if (!ARRAY_S[i]) {
			uniform_real_distribution<double> dist(0.0, 1.0);
			double ran = dist(rng);
			queryVector[i] = ran;
			queryVector_later[i] -= ran;
		}
	}

	queryVector.resize(KEYWORD_NUM * 2);
	for (int j = 0; j < KEYWORD_NUM; j++) {
		queryVector[j + KEYWORD_NUM] = queryVector_later[j];
	}

	vector<double>().swap(queryVector_later);
	//cout << "Size of queryVector_later after swapping: " << queryVector_later.size() << endl;

	QueryPerformanceCounter(&m_Now);
	double run_time2 = (double)(m_Now.QuadPart - m_Start.QuadPart) * 1000 / (double)m_Freq.QuadPart;
	stringstream ss;
	ss << fixed << setprecision(4) << run_time2;
	w_time = ss.str();

	return queryVector;
}

void RBTreeSearch::loadIDFValues(const string& path) {
	ifstream file(path);
	string line;
	//map<string, double>::iterator dou_it;
	if (file.is_open()) {
		while (getline(file, line)) {
			stringstream ss(line);
			string strleft, strright;
			getline(ss, strleft, ' ');
			getline(ss, strright);
			double weight = stod(strright);
			auto dou_it = IDFVALUE.find(strleft);
			if (dou_it == IDFVALUE.end()) {
				IDFVALUE.insert(pair<string, double>(strleft, weight));
			}
		}
		file.close();
	}
	else {
		cout << "Error: Unable to open the file " << path << endl;
	}
}

void RBTreeSearch::loadVectorBool(std::vector<bool>& vec, const std::string& filename) {
	std::ifstream inFile(filename, std::ios::binary);
	if (!inFile) {
		std::cerr << "Failed to open file for reading: " << filename << std::endl;
		return;
	}

	// Read the size of the vector
	size_t size;
	inFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	if (inFile.fail()) {
		std::cerr << "Error reading the size of the vector." << std::endl;
		return;
	}

	// Resize the vector to the saved size
	vec.resize(size);

	// Read each boolean value from the file
	for (size_t i = 0; i < size; ++i) {
		char byte;
		inFile.read(&byte, sizeof(byte));
		if (inFile.fail()) {
			std::cerr << "Error reading data for vector element." << std::endl;
			vec.resize(i); // Resize to the actual number of elements read
			break;
		}
		vec[i] = static_cast<bool>(byte);
	}

	inFile.close();
}

void RBTreeSearch::LoadDictionary(const string& filename) {
	ifstream inFile(filename);
	if (inFile.is_open()) {
		Dictionary.clear();
		string line;
		while (getline(inFile, line)) {
			istringstream iss(line);
			string word;
			int index;
			if (iss >> word >> index) {
				Dictionary[word] = index;
			}
		}
		inFile.close();
		cout << "Dictionary loaded from file: " << filename << endl;
	}
	else {
		cerr << "Unable to open file: " << filename << endl;
	}
}




