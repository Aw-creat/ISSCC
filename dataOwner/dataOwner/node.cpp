#include "node.h"
#include <vector>
#include <string>
#include <cstdlib> // For std::rand()
#include <ctime>   // For std::time()
#include <chrono>

#include <cereal/types/memory.hpp>  // ��������ָ������л�
#include <cereal/types/vector.hpp>  // �������������л�
#include <cereal/archives/binary.hpp> // ���ڶ����ƹ鵵
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

	// �������������
	std::srand(std::time(nullptr)); // ʹ�õ�ǰʱ����Ϊ����

	int dimen = arry_S.size();

	// ȷ�� m1_Data �� m2_Data �Ĵ�С����Ϊ dimen
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
			double ran = static_cast<double>(std::rand()) / RAND_MAX; // ����0��1֮��������
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
