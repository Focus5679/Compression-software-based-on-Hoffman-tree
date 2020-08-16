#include"ComAndEx.h"
//压缩函数
void ComAndEx::Compress()
{
	string filename = ScanCharacter();//扫描源文件种字符的种类及个数，返回源文件的文件名并用string类型变量保存
	if (filename == "NULL")return;
	BuildHuffmanTree();//构造哈夫曼树
	CreateHuffmanCode(TreeRoot); //生成哈夫曼编码
	cout << "请输入压缩后的文件名：" << endl;
	string outname;
	cin >> outname;
	ofstream out("./rescourse/"+outname, ios::binary);
	if (!out)
	{
		cout << "操作失败" << endl;
		return;
	}
	out.write((char*)&Hnodenum, sizeof(HuffmanCode.size()));
	for (auto i : temparry)//依次输入字符及字符的频度
	{
		if (i.weight > 0)
		{
			out.write((char*)&i.name, sizeof(unsigned char));
			out.write((char*)&i.weight, sizeof(unsigned int));
		}
	}
	ifstream in("./rescourse/" + filename, ios::binary);//根据源文件生成压缩文件
	if (!in)
	{
		cout << "打开源文件失败" << endl;
		return;
	}
	unsigned char temp_char = 0;//用8位的无符号变量作为读写字符的单元
	string unit;//存储哈夫曼编码，编码的个数大于等于八位就和char_temp进行|操作
	while (true)
	{
		temp_char = in.get();//在原文件中读取一个字符
		if (in.eof())break;//判断是否到达文件的末尾
		for (int i = 0; i < Hnodenum; i++)//在存取哈夫曼编码的数组中找到该字符对应的编码并将编码存储到unit中
		{
			if (temp_char == HuffmanCode[i].name)
			{
				unit += HuffmanCode[i].code;
			}
		}
		while (unit.size() >= 8)//如果unit中存储的哈夫曼编码的位数大于等于8，就将前八位通过位操作写入char_temp中
		{
			temp_char = 0;
			for (int i = 0; i < 8; i++)
			{
				temp_char = temp_char << 1;
				if (unit[i] == '1')
					temp_char = temp_char | 1;
			}
			auto p = unit.begin();
			unit.erase(p, p+8);//清空unit中的前八位编码
			out.write((char*)&temp_char, sizeof(unsigned char));//将载有哈夫曼编码信息的字符写入到压缩文件中去
		}
	}
	if (unit.length() > 0)//处理最后不足八位的哈夫曼编码
	{
		temp_char = 0;
		for (decltype(unit.length()) i = 0; i < unit.length(); ++i)
		{
			temp_char = temp_char << 1;
			if (unit[i] == '1')
				temp_char = temp_char | 1;
		}
		for (decltype(unit.length()) i = 0; i < (8 - unit.length()); ++i)//将编码移到到高位
			temp_char = temp_char << 1;
		out.write((char*)&temp_char, sizeof(unsigned char));
	}
	in.close();
	out.close();
	//文件压缩完成，重新初始化自身的成员变量
	for (int i = 0; i < 256; i++)
	{
		temparry[i].name = 0;
		temparry[i].weight = 0;
	}
	DestoryTree(TreeRoot);
	HuffmanCode.resize(0);
	Hnodenum = 0;
	tempcode = "\0";
}
//解压缩函数
void ComAndEx::Extract()
{
	cout << "请输入您要解压的文件的名字:" << endl;
	string filename;
	cin >> filename;
	ifstream in("./rescourse/" + filename, ios::binary);
	if (!in)
	{
		cout << "文件打开失败.." << endl;
		return;
	}
	unsigned long long length{ 0 };//统计字符总数
	in.read((char*)&Hnodenum, sizeof(unsigned int));//读取字符种类数
	for (int i = 0; i < Hnodenum; i++)//读取字符的种类及频度
	{
		Hnode *temp = new Hnode();
		in.read((char*)&(temp->name), sizeof(char));
		in.read((char*)&(temp->weight), sizeof(int));
		length += temp->weight;
		pque.push(temp);
	}
	BuildHuffmanTree();//建立哈夫曼树
	cout << "请输入解压缩后的文件名称：" << endl;
	string exname;
	cin >> exname;
	ofstream out("./rescourse/" + exname, ios::binary);
	if (!out)
	{
		cout << "操作失败" << endl;
		return;
	}
	unsigned char temp_char = 0;//用于读取哈夫曼编码
	Hnode *ptr = TreeRoot;//用于寻找叶子结点
	while (length)
	{
		temp_char = in.get();
		for (int i = 0; i < 8; i++)
		{
			if (temp_char & 128)//判断最高位是否是1，以此决定向左或向右走。
				ptr = ptr->rc;
			else
				ptr = ptr->lc;
			if (ptr->lc == nullptr && ptr->rc == nullptr)//如果到达叶子结点
			{
				out << ptr->name;//输出字符
				length--;//减少需要输出的字符总数
				if (!length)break;//需要输出的字符总数为0则退出
				ptr = TreeRoot;//回到根节点
			}
			temp_char = temp_char << 1;
		}
	}
	in.close();
	out.close();
	//文件压缩完成，重新初始化自身的成员变量
	DestoryTree(TreeRoot);
	HuffmanCode.resize(0);
	Hnodenum = 0;
	tempcode = "\0";
}
//统计字符个数
string ComAndEx::ScanCharacter()
{
	cout << "请输入文件名（确保文件在当前目录下的rescourse文件夹中）：" << endl;
	string filename;
	cin >> filename;
	ifstream in("./rescourse/" + filename, ios::binary);//以二进制的格式打开源文件
	if (!in)
	{
		cout << "未发现该文件" << endl;
		return NULL;
	}
	unsigned char temp = 0;
	
	while (true)//统计文件中字符种类及个数
	{
		temp = in.get();
		if (in.eof())break;
		temparry[temp].name = temp;
		temparry[temp].weight++;
	}
	in.close();
	for (auto i = 0; i < 256; i++)//将非零的字符及其个数压入优先队列
	{
		if (temparry[i].weight > 0)
		{
			Hnode *temp = new Hnode(temparry[i].name, temparry[i].weight);
			pque.push(temp);
			Hnodenum++;
		}
	}
	if (Hnodenum == 0)
	{
		cout << "该文件为空文件" << endl;
		return NULL;
	}
	return filename;
}
//建立哈夫曼树
void ComAndEx::BuildHuffmanTree()
{
	while (pque.size() > 1)
	{
		Hnode *min = pque.top(); pque.pop();//取出权重最小结点
		Hnode *secmin = pque.top(); pque.pop();//取出权重次小结点
		Hnode *newnode = new Hnode(0, min->weight+secmin->weight);//构建新的结点
		newnode->lc = min;
		newnode->rc = secmin;
		pque.push(newnode);//插回优先队列
	}
	TreeRoot = pque.top();pque.pop();
	if (TreeRoot->lc == nullptr && TreeRoot->rc == nullptr)//特判字符种类仅有1个的情况
	{
		Hnode *newnode = new Hnode;
		newnode->lc = TreeRoot;
		TreeRoot = newnode;
	}
}
//生成哈夫曼编码
void ComAndEx::CreateHuffmanCode(Hnode *root)
{
	if (root->lc == nullptr, root->rc == nullptr)//到达叶子结点，生成哈夫曼编码
	{
		HuffmanCode.push_back(HuffmanCode_node(root->name, root->weight, tempcode));
	}
	if (root->lc != nullptr)//遍历左树
	{
		tempcode.push_back('0');
		CreateHuffmanCode(root->lc);
		tempcode.pop_back();
	}
	if (root->rc != nullptr)//遍历右树
	{
		tempcode.push_back('1');
		CreateHuffmanCode(root->rc);
		tempcode.pop_back();
	}
}
//清空哈夫曼树
void ComAndEx::DestoryTree(Hnode *root)
{
	if (root->lc)//清空左树
	{
		DestoryTree(root->lc);
	}
	if (root->rc)//清空右树
	{
		DestoryTree(root->rc);
	}
	delete root;//释放内存
	root = nullptr;
}