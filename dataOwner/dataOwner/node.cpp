#include "node.h"
#include <vector>
#include <string>
#include <cstdlib> // For std::rand()
#include <ctime>   // For std::time()
#include <chrono>

#include <cereal/types/memory.hpp>  // 用于智能指针的序列化
#include <cereal/types/vector.hpp>  // 用于容器的序列化
#include <cereal/archives/binary.hpp> // 用于二进制归档
#include <cereal/cereal.hpp>

using namespace std;

RBTreeNode::RBTreeNode() {}

RBTreeNode::~RBTreeNode() {}

void RBTreeNode::InitialLeaf(vector<double> vectorData, string fileNumber) {
	m_Data = vectorData;
	int vecSize = vectorData.size();
	srand(static_cast<unsigned int>(time(nullptr))); // Use std::time() for seeding
	int r = 0;
	while (r < KEY_EXTEND / 2) {
		int loc = rand() % KEY_EXTEND + vecSize - KEY_EXTEND;
		if (m_Data[loc] != 1) {
			m_Data[loc] = 1;
			r++;
		}
	}

	leftChild = nullptr;
	rightChild = nullptr;
	parent = nullptr;
	fileName = fileNumber;
	score = 0;
	tag = true;
}

void RBTreeNode::InitialInternalNode(std::shared_ptr<RBTreeNode> left, std::shared_ptr<RBTreeNode> right) {
	m_Data = left->m_Data;
	int vecSize = m_Data.size();
	vector<double> rightWeight = right->m_Data;
	for (int ki = 0; ki < vecSize; ki++) {
		if (m_Data[ki] < rightWeight[ki])
			m_Data[ki] = rightWeight[ki];
	}

	leftChild = left;
	rightChild = right;
	parent = nullptr;

	fileName = "";
	score = 0;
	tag = false;
}

void RBTreeNode::node_scoreCal(vector<double> queryVector) {
	double temp = 0;
	if (!queryVector.empty() && !m_Data.empty()) {
		for (size_t ki = 0; ki < queryVector.size(); ki++) {
			temp += queryVector[ki] * m_Data[ki];
		}
		score = temp;
	}
}

void RBTreeNode::enc_dataSplit(std::vector<bool> arry_S)
{
	std::vector<double> m1_Data = m_Data;
	std::vector<double> m2_Data = m_Data;

	// 设置随机数种子
	std::srand(std::time(nullptr)); // 使用当前时间作为种子

	int dimen = arry_S.size();

	// 确保 m1_Data 和 m2_Data 的大小至少为 dimen
	if (m1_Data.size() < dimen) {
		m1_Data.resize(dimen);
	}
	if (m2_Data.size() < dimen) {
		m2_Data.resize(dimen);
	}

	for (int i = 0; i < dimen; ++i)
	{
		if (arry_S[i])
		{
			double ran = static_cast<double>(std::rand()) / RAND_MAX; // 生成0到1之间的随机数
			m1_Data[i] = ran;
			m2_Data[i] -= ran;
		}
	}

	for (int i = 0; i < dimen; ++i)
	{
		m1_Data.push_back(m2_Data[i]);
	}

	m_Data = m1_Data;
}
