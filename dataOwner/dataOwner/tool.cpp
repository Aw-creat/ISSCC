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
#include <functional>

#include <cereal/types/memory.hpp>  // 用于智能指针的序列化
#include <cereal/types/vector.hpp>  // 用于容器的序列化
#include <cereal/archives/binary.hpp> // 用于二进制归档


using namespace std;
map<string, int> Dictionary;
map<string, double> IDFVALUE;
//RBTreeNode* RBTreeRoot;
std::shared_ptr<RBTreeNode> RBTreeRoot;
//RBTreeNode* RBTreeRoot_Enc;
std::shared_ptr<RBTreeNode> RBTreeRoot_Enc;
double K_TH_SCORE;
int KEYWORD_NUM;
int num_k;    //the value of k
vector<double> queryVector;

priority_queue<std::shared_ptr<RBTreeNode>, vector<std::shared_ptr<RBTreeNode>>, cmp> Top_K_Queue;
priority_queue<std::shared_ptr<RBTreeNode>, vector<std::shared_ptr<RBTreeNode>>, cmp> Whole_Queue;//queue without cutting
vector<bool> ARRAY_S;

RBTreeSearch::RBTreeSearch()
{

}
RBTreeSearch::~RBTreeSearch()
{

}

// Function to save a vector<bool> to a file
/*void saveVectorBool(const vector<bool>& vec, const string& filename) {
	ofstream outFile(filename, ios::binary);
	if (!outFile) {
		cerr << "Failed to open file for writing: " << filename << endl;
		return;
	}

	// Write the size of the vector
	size_t size = vec.size();
	outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

	// Write each boolean value to the file
	for (size_t i = 0; i < size; ++i) {
		char byte = vec[i] ? 1 : 0;
		outFile.write(&byte, sizeof(byte));
	}

	outFile.close();
}*/

void saveVectorBool(const std::vector<bool>& vec, const std::string& filename) {
	std::ofstream outFile(filename, std::ios::binary);
	if (!outFile) {
		std::cerr << "Failed to open file for writing: " << filename << std::endl;
		return;
	}

	// Write the size of the vector
	size_t size = vec.size();
	outFile.write(reinterpret_cast<const char*>(&size), sizeof(size));

	// Write each boolean value to the file
	for (bool value : vec) {
		char byte = static_cast<char>(value);
		outFile.write(&byte, sizeof(byte));
	}

	outFile.close();
	if (outFile.fail()) {
		std::cerr << "Failed to write data to file: " << filename << std::endl;
	}
}


void RBTreeSearch::SaveDictionary(const string& filename) {
	ofstream outFile(filename);
	if (outFile.is_open()) {
		for (const auto& entry : Dictionary) {
			outFile << entry.first << " " << entry.second << endl;
		}
		outFile.close();
		cout << "Dictionary saved to file: " << filename << endl;
	}
	else {
		cerr << "Unable to open file: " << filename << endl;
	}
}

void RBTreeSearch::GetAllKeywords(const vector<string>& strPaths) {
	Dictionary.clear();
	int wordIndex = 0; // 关键词在向量中的位置
	for (const auto& filePath : strPaths) {
		ifstream file(filePath);
		string line;
		while (getline(file, line)) {
			istringstream iss(line);
			string word;
			if (iss >> word) {
				auto it = Dictionary.find(word);
				if (it == Dictionary.end()) {
					Dictionary.insert({ word, wordIndex });
					wordIndex++;
				}
			}
		}
		file.close();
	}
	// 打印 Dictionary 的内容
	cout << "Dictionary contents:" << endl;
	for (const auto& entry : Dictionary) {
		cout << "Word: " << entry.first << ", Index: " << entry.second << endl;
	}
}

void RBTreeSearch::GetAllFilesInFolder(const std::string& path, std::vector<std::string>& strPaths) {
	strPaths.clear();
	WIN32_FIND_DATAA findData;
	HANDLE hFind = FindFirstFileA((path + "\\*").c_str(), &findData);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			std::string fileName = findData.cFileName;
			if (fileName.size() >= 4 && fileName.substr(fileName.size() - 4) == ".txt") {
				strPaths.push_back(path + "\\" + fileName);
			}
		} while (FindNextFileA(hFind, &findData) != 0);
		FindClose(hFind);
	}
	else {
		std::cerr << "Error: Unable to open directory" << std::endl;
	}
}

/*bool RBTreeSearch::ConstructRBTree(const std::string& path, std::string& keywordNum, std::string& docNum, std::string& contime) {
	vector<RBTreeNode*> tempNodes;
	vector<string> strPaths;
	GetAllFilesInFolder(path, strPaths); // 获取文件夹中所有文件
	GetAllKeywords(strPaths); // 统计文件集对应的关键词集

	SaveDictionary("dictionary.txt");

	docNum = to_string(strPaths.size());

	if (Dictionary.size() == 0) {
		cout << "Caution: Dictionary is empty!" << endl;
		return false;
	}

	KEYWORD_NUM = Dictionary.size() + KEY_EXTEND;
	keywordNum = to_string(KEYWORD_NUM);
	cout << "keywordNum = " << keywordNum << endl;
	cout << "docNum = " << docNum << endl;

	// 计时开始

	chrono::steady_clock::time_point start = chrono::steady_clock::now();

	for (int fi = 0; fi < strPaths.size(); fi++) {
		string leafname = strPaths[fi];
		vector<double> vecData(KEYWORD_NUM, 0);

		// 读取文件的关键词及权值
		ifstream file(strPaths[fi]);
		string line;
		while (getline(file, line)) {
			istringstream iss(line);
			string keystr, weightStr;
			iss >> keystr >> weightStr;
			double weight = stod(weightStr);
			auto it = Dictionary.find(keystr);
			if (it != Dictionary.end()) {
				vecData[Dictionary[keystr]] = weight;
				// 输出关键词及权值
				cout << "Keyword: " << keystr << ", Weight: " << weight << endl;
			}
			else {
				cout << "Caution: Keyword Set is not completed!" << endl;
				return false;
			}
		}
		file.close();

		// 创建文件对应的叶子节点
		RBTreeNode *leafNode = new RBTreeNode;
		leafNode->InitialLeaf(vecData, strPaths[fi]);
		tempNodes.push_back(leafNode);
	}


	vector<RBTreeNode*> sec_tempnodes;
	while (tempNodes.size() > 0) {
		cout << "Current size of tempNodes: " << tempNodes.size() << endl;
		if (tempNodes.size() % 2 == 0) {
			for (int i = 0; i < tempNodes.size(); i += 2) {
				RBTreeNode* interNode = new RBTreeNode;
				RBTreeNode* leftNode = tempNodes[i];
				RBTreeNode* rightNode = tempNodes[i + 1];

				interNode->InitialInternalNode(leftNode, rightNode);
				leftNode->parent = interNode;
				rightNode->parent = interNode;
				sec_tempnodes.push_back(interNode);
			}
			cout << "Internal nodes created: " << sec_tempnodes.size() << endl;
			tempNodes.clear();
			tempNodes = sec_tempnodes;
			sec_tempnodes.clear();
			if (tempNodes.size() == 1) {
				RBTreeRoot = tempNodes[0];
				cout << "Root node set." << endl;
				break;
			}
		}
		else {
			cout << "Handling remaining nodes." << endl;
			if (tempNodes.size() > 4) {
				for (int n = 0; n < tempNodes.size() - 1; n += 2) {
					RBTreeNode* interNode = new RBTreeNode;
					RBTreeNode* leftNode = tempNodes[n];
					RBTreeNode* rightNode = tempNodes[n + 1];

					interNode->InitialInternalNode(leftNode, rightNode);
					leftNode->parent = interNode;
					rightNode->parent = interNode;
					sec_tempnodes.push_back(interNode);
				}
				cout << "Intermediate nodes created: " << sec_tempnodes.size() << endl;
				RBTreeNode* sec_lastNode = sec_tempnodes[sec_tempnodes.size() - 1];
				RBTreeNode* lastNode = tempNodes[tempNodes.size() - 1];
				RBTreeNode* last_merge = new RBTreeNode;
				last_merge->InitialInternalNode(sec_lastNode, lastNode);
				sec_lastNode->parent = last_merge;
				lastNode->parent = last_merge;
				sec_tempnodes.pop_back();
				sec_tempnodes.push_back(last_merge);
				tempNodes.clear();
				tempNodes = sec_tempnodes;
				sec_tempnodes.clear();
			}
			else if (tempNodes.size() == 3) {
				cout << "Handling special case with 3 nodes." << endl;
				RBTreeNode* firstNode = tempNodes[0];
				RBTreeNode* secNode = tempNodes[1];
				RBTreeNode* thirdNode = tempNodes[2];
				RBTreeNode* tempNode = new RBTreeNode;
				RBTreeNode* finalNode = new RBTreeNode;
				tempNode->InitialInternalNode(firstNode, secNode);
				firstNode->parent = tempNode;
				secNode->parent = tempNode;
				finalNode->InitialInternalNode(tempNode, thirdNode);
				tempNode->parent = finalNode;
				thirdNode->parent = finalNode;
				RBTreeRoot = finalNode;
				cout << "Root node set." << endl;
				break;
			}
		}
	}

	// 清空临时节点容器
	cout << "Clearing temporary node containers." << endl;
	vector<RBTreeNode*>().swap(tempNodes);
	vector<RBTreeNode*>().swap(sec_tempnodes);

	cout << "Clearing ARRAY_S." << endl;
	ARRAY_S.clear();

	//重新生成 ARRAY_S
	cout << "Regenerating ARRAY_S." << endl;
	default_random_engine rng(chrono::system_clock::now().time_since_epoch().count());
	uniform_int_distribution<int> distribution(0, 1);
	for (int i = 0; i < KEYWORD_NUM; i++) {
		int randomValue = distribution(rng);
		cout << "Generating random value: " << randomValue << endl;
		ARRAY_S.push_back(randomValue);
	}

	// Save the vector to file
	const string filename = "array_s.dat";
	saveVectorBool(ARRAY_S, filename);
	cout << "Vector saved to " << filename << endl;

	// 生成加密树
	cout << "Generating encrypted tree data split." << endl;
	GenEncTree_dataSplit(RBTreeRoot);
	cout << "Encrypted tree data split generated successfully." << endl;


	// 计时结束
	chrono::steady_clock::time_point end = chrono::steady_clock::now();
	contime = to_string(chrono::duration_cast<chrono::milliseconds>(end - start).count());
	//cout << "Execution time: " << contime << " milliseconds." << endl << flush;
	cerr << "Execution time: " << contime << " milliseconds." << endl;

	return true;
}*/

/*bool RBTreeSearch::ConstructRBTree(const std::string& path, std::string& keywordNum, std::string& docNum, std::string& contime) {
	std::vector<std::shared_ptr<RBTreeNode>> tempNodes;
	std::vector<std::string> strPaths;
	GetAllFilesInFolder(path, strPaths);
	GetAllKeywords(strPaths);
	SaveDictionary("dictionary.txt");

	docNum = std::to_string(strPaths.size());

	if (Dictionary.empty()) {
		std::cout << "Caution: Dictionary is empty!" << std::endl;
		return false;
	}

	KEYWORD_NUM = Dictionary.size() + KEY_EXTEND;
	keywordNum = std::to_string(KEYWORD_NUM);

	auto start = std::chrono::steady_clock::now();

	for (const auto& filePath : strPaths) {
		std::vector<double> vecData(KEYWORD_NUM, 0.0);
		std::ifstream file(filePath);
		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string keystr, weightStr;
			iss >> keystr >> weightStr;
			double weight = std::stod(weightStr);
			auto it = Dictionary.find(keystr);
			if (it != Dictionary.end()) {
				vecData[it->second] = weight;
			}
			else {
				std::cout << "Caution: Keyword Set is not completed!" << std::endl;
				return false;
			}
		}
		auto leafNode = std::make_shared<RBTreeNode>();
		leafNode->InitialLeaf(vecData, filePath);
		tempNodes.push_back(leafNode);
	}

	while (tempNodes.size() > 1) {
		std::vector<std::shared_ptr<RBTreeNode>> newTempNodes;
		for (size_t i = 0; i < tempNodes.size(); i += 2) {
			if (i + 1 < tempNodes.size()) {
				auto interNode = std::make_shared<RBTreeNode>();
				interNode->InitialInternalNode(tempNodes[i], tempNodes[i + 1]);
				newTempNodes.push_back(interNode);
			}
			else {
				newTempNodes.push_back(tempNodes[i]);
			}
		}
		tempNodes = newTempNodes;
	}
	RBTreeRoot = tempNodes.front();

	// 生成加密树
	cout << "Generating encrypted tree data split." << endl;
	GenEncTree_dataSplit(RBTreeRoot);
	cout << "Encrypted tree data split generated successfully." << endl;

	auto end = std::chrono::steady_clock::now();
	contime = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());

	return true;
}*/

bool RBTreeSearch::ConstructRBTree(const std::string& path, std::string& keywordNum, std::string& docNum, std::string& contime) {
	std::vector<std::shared_ptr<RBTreeNode>> tempNodes;
	std::vector<std::string> strPaths;
	GetAllFilesInFolder(path, strPaths); // 获取文件夹中所有文件
	GetAllKeywords(strPaths); // 统计文件集对应的关键词集

	SaveDictionary("dictionary.txt");

	docNum = std::to_string(strPaths.size());

	if (Dictionary.size() == 0) {
		std::cout << "Caution: Dictionary is empty!" << std::endl;
		return false;
	}

	KEYWORD_NUM = Dictionary.size() + KEY_EXTEND;
	keywordNum = std::to_string(KEYWORD_NUM);
	std::cout << "keywordNum = " << keywordNum << std::endl;
	std::cout << "docNum = " << docNum << std::endl;

	// 计时开始
	auto start = std::chrono::steady_clock::now();

	for (const auto& leafname : strPaths) {
		std::vector<double> vecData(KEYWORD_NUM, 0);

		// 读取文件的关键词及权值
		std::ifstream file(leafname);
		std::string line;
		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string keystr, weightStr;
			iss >> keystr >> weightStr;
			double weight = std::stod(weightStr);
			auto it = Dictionary.find(keystr);
			if (it != Dictionary.end()) {
				vecData[Dictionary[keystr]] = weight;
				// 输出关键词及权值
				std::cout << "Keyword: " << keystr << ", Weight: " << weight << std::endl;
			}
			else {
				std::cout << "Caution: Keyword Set is not completed!" << std::endl;
				return false;
			}
		}

		// 创建文件对应的叶子节点
		auto leafNode = std::make_shared<RBTreeNode>();
		leafNode->InitialLeaf(vecData, leafname);
		tempNodes.push_back(leafNode);
	}

	std::vector<std::shared_ptr<RBTreeNode>> sec_tempnodes;
	while (!tempNodes.empty()) {
		std::cout << "Current size of tempNodes: " << tempNodes.size() << std::endl;
		if (tempNodes.size() % 2 == 0) {
			for (size_t i = 0; i < tempNodes.size(); i += 2) {
				auto interNode = std::make_shared<RBTreeNode>();
				auto leftNode = tempNodes[i];
				auto rightNode = tempNodes[i + 1];

				interNode->InitialInternalNode(leftNode, rightNode);
				leftNode->parent = interNode;
				rightNode->parent = interNode;
				sec_tempnodes.push_back(interNode);
			}
			std::cout << "Internal nodes created: " << sec_tempnodes.size() << std::endl;
			tempNodes.swap(sec_tempnodes);
			sec_tempnodes.clear();
			if (tempNodes.size() == 1) {
				RBTreeRoot = tempNodes[0];
				std::cout << "Root node set." << std::endl;
				break;
			}
		}
		else {
			std::cout << "Handling remaining nodes." << std::endl;
			if (tempNodes.size() > 4) {
				for (size_t n = 0; n < tempNodes.size() - 1; n += 2) {
					auto interNode = std::make_shared<RBTreeNode>();
					auto leftNode = tempNodes[n];
					auto rightNode = tempNodes[n + 1];

					interNode->InitialInternalNode(leftNode, rightNode);
					leftNode->parent = interNode;
					rightNode->parent = interNode;
					sec_tempnodes.push_back(interNode);
				}
				std::cout << "Intermediate nodes created: " << sec_tempnodes.size() << std::endl;
				auto sec_lastNode = sec_tempnodes.back();
				auto lastNode = tempNodes.back();
				auto last_merge = std::make_shared<RBTreeNode>();
				last_merge->InitialInternalNode(sec_lastNode, lastNode);
				sec_lastNode->parent = last_merge;
				lastNode->parent = last_merge;
				sec_tempnodes.pop_back();
				sec_tempnodes.push_back(last_merge);
				tempNodes.swap(sec_tempnodes);
				sec_tempnodes.clear();
			}
			else if (tempNodes.size() == 3) {
				std::cout << "Handling special case with 3 nodes." << std::endl;
				auto firstNode = tempNodes[0];
				auto secNode = tempNodes[1];
				auto thirdNode = tempNodes[2];
				auto tempNode = std::make_shared<RBTreeNode>();
				auto finalNode = std::make_shared<RBTreeNode>();
				tempNode->InitialInternalNode(firstNode, secNode);
				firstNode->parent = tempNode;
				secNode->parent = tempNode;
				finalNode->InitialInternalNode(tempNode, thirdNode);
				tempNode->parent = finalNode;
				thirdNode->parent = finalNode;
				RBTreeRoot = finalNode;
				std::cout << "Root node set." << std::endl;
				break;
			}
		}
	}

	// 清空临时节点容器
	std::cout << "Clearing temporary node containers." << std::endl;
	tempNodes.clear();
	sec_tempnodes.clear();

	std::cout << "Clearing ARRAY_S." << std::endl;
	ARRAY_S.clear();

	//重新生成 ARRAY_S
	std::cout << "Regenerating ARRAY_S." << std::endl;
	std::default_random_engine rng(std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> distribution(0, 1);
	for (int i = 0; i < KEYWORD_NUM; i++) {
		int randomValue = distribution(rng);
		std::cout << "Generating random value: " << randomValue << std::endl;
		ARRAY_S.push_back(randomValue);
	}

	// Save the vector to file
	const std::string filename = "array_s.dat";
	saveVectorBool(ARRAY_S, filename);
	std::cout << "Vector saved to " << filename << std::endl;

	// 生成加密树
	std::cout << "Generating encrypted tree data split." << std::endl;
	GenEncTree_dataSplit(RBTreeRoot);
	std::cout << "Encrypted tree data split generated successfully." << std::endl;

	// 计时结束
	auto end = std::chrono::steady_clock::now();
	contime = std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
	std::cerr << "Execution time: " << contime << " milliseconds." << std::endl;

	// 保存加密树到文件
	const std::string encryptedTreeFilename = "encrypted_tree.dat";
	SaveEncryptedTree(encryptedTreeFilename);
	std::cout << "Encrypted tree saved to " << encryptedTreeFilename << std::endl;

	return true;
}

void RBTreeSearch::GenEncTree_dataSplit(std::shared_ptr<RBTreeNode> node)
{
	if (node != nullptr)
	{
		cout << "Processing node..." << endl;
		node->enc_dataSplit(ARRAY_S);
		// 递归处理左子树和右子树
		cout << "Processing left child..." << endl;
		GenEncTree_dataSplit(node->leftChild);
		cout << "Processing right child..." << endl;
		GenEncTree_dataSplit(node->rightChild);	//,arry_S	
	}
	else
	{
		cout << "Node is NULL." << endl;
	}
	return;
}

void RBTreeSearch::GenRand(std::vector<std::vector<double>>& arr, int seed)
{
	std::mt19937 rng(seed);
	std::uniform_real_distribution<double> dis(0.0, 1.0);

	for (size_t i = 0; i < arr.size(); ++i)
	{
		for (size_t j = 0; j < arr[i].size(); ++j)
		{
			arr[i][j] = dis(rng);
		}
	}
}

void RBTreeSearch::MatrixToVector(double* data, int rows, int cols, std::vector<std::vector<double>>& Matrix)
{
	for (int i = 0; i < rows; ++i)
	{
		std::vector<double> temp;
		for (int j = 0; j < cols; ++j)
		{
			temp.push_back(data[i * cols + j]);
		}
		Matrix.push_back(temp);
	}
}

/*void RBTreeSearch::SaveEncryptedTree(const std::string& filename) {
	std::ofstream os(filename, std::ios::binary);
	if (!os.is_open()) {
		std::cerr << "Error opening file for writing: " << filename << std::endl;
		return;
	}

	cereal::BinaryOutputArchive archive(os);
	try {
		archive(RBTreeRoot);
	}
	catch (const cereal::Exception& e) {
		std::cerr << "Cereal serialization error: " << e.what() << std::endl;
	}
}*/

void RBTreeSearch::SaveEncryptedTree(const std::string& filename) {
	std::ofstream os(filename, std::ios::binary);
	if (!os.is_open()) {
		std::cerr << "Error opening file for writing: " << filename << std::endl;
		return;
	}

	cereal::BinaryOutputArchive archive(os);
	try {
		archive(RBTreeRoot);  // 序列化整棵树
	}
	catch (const cereal::Exception& e) {
		std::cerr << "Cereal serialization error: " << e.what() << std::endl;
	}
}



