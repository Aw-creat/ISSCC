#pragma once

#include <vector>
#include <iostream>

#include <cereal/types/vector.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <fstream>

using namespace std;

const int KEY_EXTEND = 0;

class RBTreeNode
{
public:
	std::shared_ptr<RBTreeNode> leftChild;
	std::shared_ptr<RBTreeNode> rightChild;
	std::shared_ptr<RBTreeNode> parent;

	//RBTreeNode* leftChild;
	//RBTreeNode* rightChild;
	//RBTreeNode* parent;
	vector<double> m_Data;
	//vector<double> enc_m_Data;
	string fileName; // 文件编号改为string类型
	double score;
	bool tag; // internal node: false; leaf node: true


public:
	RBTreeNode();
	~RBTreeNode();

	void InitialLeaf(vector<double> vectorData, string fileNumber); 
	void InitialInternalNode(std::shared_ptr<RBTreeNode> left, std::shared_ptr<RBTreeNode> right);
	void node_scoreCal(vector<double> queryVector);
	void enc_dataSplit(vector<bool> arry_S); 

	/*template <class Archive>
	void serialize(Archive& ar) {
		ar(m_Data, fileName, score, tag, leftChild, rightChild, parent);
	}*/

	template<class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("leftChild", leftChild),
			cereal::make_nvp("rightChild", rightChild),
			CEREAL_NVP(m_Data), CEREAL_NVP(fileName),
			CEREAL_NVP(score), CEREAL_NVP(tag));
	}

};
