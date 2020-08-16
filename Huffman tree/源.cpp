#include"ComAndEx.h"


int main()
{
	ComAndEx *comandex = new ComAndEx;
	while (true)
	{
		cout << "===========================" << endl;
		cout << "1.压缩文件" << endl;
		cout << "2.解压文件" << endl;
		cout << "3.退出程序" << endl;
		cout << "===========================" << endl;
		cout << "请输入选择：" << endl;
		int chose{ 0 };
		cin >> chose;
		if (chose == 1)
		{
			comandex->Compress();
		}
		else if (chose == 2)
		{
			comandex->Extract();
		}
		else
		{
			cout << "输入错误,请重新输入" << endl;
		}
	}
	return 0;
}