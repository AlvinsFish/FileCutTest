// FileCutTest.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
/*
实现如下功能：
1.读取参数
-s source.dat：指定待分割的文件名称；
-d dest：指定分割后的文件模板名称，其文件将以dest0001.dat,dest0002.dat.....的方式进行命名
-n size:指定分割后的文件大小
2.用法
./split_file -s source.dat -d dest -n 10485760
以10M为单位进行文件分割
3.缺省参数
默认的源文件为source.dat
默认的分割文件模板名称为dest
默认的文件分割大小为1M(1048576Bytes)
./split_file -n 10485760
与.2中的命令执行的结果一直
https://www.cnblogs.com/findeasy/archive/2013/02/25/4053109.html
*/
#include "pch.h"
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <algorithm>
#include <iostream>
#include "readfile.h"

using namespace std;

#define MAX_PATH 260

/**
 * -s:source file
 *  -d:destination file template
 *  -n:size of destination file
 */
int main()
{
	FileEncryption(L"C:\\UserData\\人生努力的方向\\1.项目\\2.文件加密\\FileCutTest\\FileCutTest\\cn_office_professional_plus_2016_x86_x64_dvd_6969182.rar", L"");

	system("pause");
	return 0;
}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件


void test()
{
	const int BUFFER_SIZE = 1024 * 10;
	std::ifstream ifs("e://setup_10.2.0.2001s.exe", ios::binary);
	std::ofstream ofs("e://setup_10.2.0.2001s2.exe", ios::binary);
	if (ifs) {
		ifs.seekg(0, ifs.end);
		int length = (int)ifs.tellg();
		ifs.seekg(0, ifs.beg);
		std::string buffer(BUFFER_SIZE, '\0');
		int offset = 0;
		int readSize = min(BUFFER_SIZE, length - offset);
		while (readSize > 0 && ifs.read(&buffer[0], readSize)) {
			ofs.write(&buffer[0], readSize);
			offset += readSize;
			readSize = min(BUFFER_SIZE, length - offset);
			std::cout << "offset:" << offset << std::endl;
		}
	}
	ifs.close();
	ofs.close();

	
}