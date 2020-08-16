#pragma once

#include<vector>
using namespace std;
class HuffmanCode_node //储存哈夫曼编码的节点
{
public:
	unsigned char name;//字符名称
	unsigned int weight;//该字符的权重
	string code;//该字符的哈夫曼编码
	HuffmanCode_node()
	{
		name = 0;
	}
	HuffmanCode_node(unsigned char name, unsigned int weight, string code):name(name), weight(weight), code(code){}//构造函数
private:

};
