#pragma once

#include <vector>
#include <string>
#include "node.h"
#include <math.h>
#include <cmath>
#include <iostream>
#include <map>
#include <random>
#include <windows.h> // For QueryPerformanceCounter
#include <sstream>   // For formatting the time outpu



struct cmp
{
	bool operator()(shared_ptr<RBTreeNode> a, shared_ptr<RBTreeNode> b)
	{
		return a->score > b->score; // ∞¥…˝–Ú≈≈–Ú
	}
};

class RBTreeSearch
{
public:
	RBTreeSearch();
	~RBTreeSearch();
	/////////////////////////////////////////

	//void GetAllKeywords(const std::vector<std::string>& strPaths);
	//void GetAllFilesInFolder(const std::string& path, std::vector<std::string>& strPaths);
	//bool ConstructRBTree(const std::string& path, std::string& keywordNum, std::string& docNum, std::string& contime);
	//void SearchRBTree(std::shared_ptr<RBTreeNode> node);
	void SearchRBTree(shared_ptr<RBTreeNode> node, const vector<double>& queryVector);
	void Updata_Top_K_List(shared_ptr<RBTreeNode> node);
	//void startSearch(vector<string> keywords_list, string &time, vector<string> &result_list, int k, string &w_time);
	//void startSearch(vector<double> queryVector, string &time, vector<string> &result_list);
	vector<string> startSearch(vector<double> queryVector, string &time);
	///////////////////////////////////20130705
	void GenEncryptedQuery(const vector<vector<double>>& Matrix1_inv, const vector<vector<double>>& Matrix2_inv);
	//void GenRand(CvMat* arr, int seed);
	//void GenRand(cv::Mat* arr, int seed);
	void GenRand(std::vector<vector<double>>& arr, int seed);
	//void MatrixToVector(cv::Mat* M, std::vector<std::vector<double>>& Matrix);
	void MatrixToVector(double* data, int rows, int cols, vector<vector<double>>& Matrix);
	//void GenEncTree_dataSplit(RBTreeNode* node);

	void LoadEncryptedTree(const string& filename);
	vector<double> prepareQueryVector(vector<string> keywords_list, int k, string &w_time);
	void loadIDFValues(const string& path);
	void loadVectorBool(vector<bool>& vec, const string& filename);
	void LoadDictionary(const string& filename);


};

void loadKeywords(std::vector<std::string>& keywords_list, int& k);
