#pragma once

#include<iostream>
#include<vector>
#include<string>
#include<queue>
#include<stack>
#include<fstream>
#include"Hnode.h"
#include"HuffmanCode_node.h"

using namespace std;

class ComAndEx //压缩解压类的头文件
{
public:
	void Compress(); //压缩文件的函数
	void Extract();//解压文件的函数
	string ScanCharacter();//扫描源文件种字符的种类及个数
	void BuildHuffmanTree();//建立哈夫曼树
	void CreateHuffmanCode(Hnode *root);//生成哈夫曼编码
	void DestoryTree(Hnode *root);//释放哈夫曼树
	ComAndEx()
	{
		TreeRoot = nullptr;
		Hnodenum = 0;
	}

private:
	int Hnodenum;//存储字符的种类数
	Hnode *TreeRoot;//存储哈夫曼树的根节点
	Hnode temparry[256];
	priority_queue<Hnode *, vector<Hnode*>, Cmp> pque;//运用构建哈夫曼树的优先队列（最小堆）
	vector<HuffmanCode_node> HuffmanCode;//存储哈夫曼编码的数组
	string tempcode;//存储临时哈夫曼编码的字符串
};

