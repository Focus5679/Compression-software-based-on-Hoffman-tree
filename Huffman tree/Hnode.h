#pragma once

class Hnode //哈夫曼树的存储存储结构
{
public:
	unsigned char name; //8位的存储单元来存储字符（256种）
	unsigned int weight;//存储字符的频度
	Hnode *lc;//左孩子
	Hnode *rc;//右孩子
	Hnode()//默认构造函数
	{
		name = 0;
		weight = 0;
		lc = nullptr;
		rc = nullptr;
	}
	Hnode(unsigned char name, unsigned int weight) :name(name), weight(weight)//构造函数
	{
		lc = nullptr;
		rc = nullptr;
	}
private:
	
};
class Cmp//用于在优先队列中比较Hnode*类型
{
public:
	bool operator() (const Hnode * a, const Hnode * b)
	{
		return  a->weight > b->weight;  //weight小的优先级别高    
	}
};