#pragma once

#include <vector>
#include <string>
#include "node.h"
#include <math.h>



struct cmp
{
	bool operator()(std::shared_ptr<RBTreeNode> a, std::shared_ptr<RBTreeNode> b)
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
	void SaveDictionary(const string& filename);

	void GetAllKeywords(const std::vector<std::string>& strPaths);
	void GetAllFilesInFolder(const std::string& path, std::vector<std::string>& strPaths);
	bool ConstructRBTree(const std::string& path, std::string& keywordNum, std::string& docNum, std::string& contime);
	//void SearchRBTree(RBTreeNode* node);
	//void Updata_Top_K_List(RBTreeNode* node);
	//void startSearch(vector<string> keywords_list, string &time, vector<string> &result_list, int k, string &w_time);

	///////////////////////////////////20130705
	void GenEncryptedQuery(const std::vector<std::vector<double>>& Matrix1_inv, const std::vector<std::vector<double>>& Matrix2_inv);
	//void GenRand(CvMat* arr, int seed);
	//void GenRand(cv::Mat* arr, int seed);
	void GenRand(std::vector<std::vector<double>>& arr, int seed);
	//void MatrixToVector(cv::Mat* M, std::vector<std::vector<double>>& Matrix);
	void MatrixToVector(double* data, int rows, int cols, std::vector<std::vector<double>>& Matrix);
	void GenEncTree_dataSplit(std::shared_ptr<RBTreeNode> node);

	void SaveEncryptedTree(const std::string& filename);

};

