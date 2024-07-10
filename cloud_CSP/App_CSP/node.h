#pragma once

#include <vector>
#include <iostream>

#include <cereal/types/memory.hpp>  // 用于智能指针的序列化
#include <cereal/types/vector.hpp>  // 用于容器的序列化
#include <cereal/archives/binary.hpp> // 用于二进制归档
#include <fstream>

using namespace std;

const int KEY_EXTEND = 0;

class RBTreeNode
{
public:
	shared_ptr<RBTreeNode> leftChild;
	shared_ptr<RBTreeNode> rightChild;
	shared_ptr<RBTreeNode> parent;

	vector<double> m_Data;
	//vector<double> enc_m_Data;
	string fileName; // 文件编号改为string类型
	double score;
	bool tag; // internal node: false; leaf node: true

public:
	RBTreeNode();
	~RBTreeNode();

	void InitialLeaf(vector<double> vectorData, string fileNumber); // 文件编号改为string类型
	void InitialInternalNode(shared_ptr<RBTreeNode> left, shared_ptr<RBTreeNode> right);
	void node_scoreCal(const vector<double>& queryVector);

	void enc_dataSplit(vector<bool> arry_S); // 将用户界面部分改为终端输入的形式，不需要参数

	/*template <class Archive>
	void serialize(Archive& ar) {
		ar(CEREAL_NVP(m_Data), CEREAL_NVP(fileName), CEREAL_NVP(score), CEREAL_NVP(tag));
		ar(cereal::make_nvp("leftChild", leftChild), cereal::make_nvp("rightChild", rightChild));
	}*/
	template<class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("leftChild", leftChild),
			cereal::make_nvp("rightChild", rightChild),
			CEREAL_NVP(m_Data), CEREAL_NVP(fileName),
			CEREAL_NVP(score), CEREAL_NVP(tag));
	}
};
