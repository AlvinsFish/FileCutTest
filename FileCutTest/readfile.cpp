#include "pch.h"
#include "readfile.h"
#include <windows.h>
#include <thread>
#include <mutex>
//static string sKey = "09128734";

volatile int thread_num = 0;
mutex mut;

void foo(int i, DWORD dLen, LPVOID pFileBlock, bool en)
{
	mut.lock();
	thread_num++;
	mut.unlock();
	cout << "第" << i << "个线程，共" << thread_num << "个线程。" << endl;
	for (int i = 0; i < dLen; ++i)
	{
		// 简单处理，异或一下，测试加密，这儿的读是不占电脑内存的
		char* a = (char*)pFileBlock + i;

		// 注意这而，写了内存，那么这段内存将会实际化，也就是说电脑占用的内存增加了（一共是512M），dFileBlock最好小一点
		//*a = *a ^ 85;
		if (en)
			*a = MakecodeChar(*a, 1);//加密
		else
			*a = CutcodeChar(*a, 1);//解密
		//*a = 'a';
	}
	// “释放”掉虚拟内存
	UnmapViewOfFile(pFileBlock);

	mut.lock();
	thread_num--;
	mut.unlock();
}

bool FileEncryption(fstring fsSrcPath, fstring fsDesPath)
{
	//存取模式
	DWORD dAccessType = (GENERIC_READ | GENERIC_WRITE);


	//共享模式
	DWORD dShareType = FILE_SHARE_READ | FILE_SHARE_WRITE;


	// 打开文件句柄
	HANDLE hFile = CreateFile(fsSrcPath.c_str()
		, dAccessType				// 访问模式
		, dShareType				// 共享模式
		, NULL
		, OPEN_EXISTING				// 如果存在则打开，否则失败
		, FILE_FLAG_OVERLAPPED		// 。。。
		, NULL);


	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("文件不存在");
		system("pause");
		return 1;
	}
	printf("载入文件成功\n");

	DWORD dFileSize = GetFileSize(hFile, NULL);


	// 创建映射句柄
	HANDLE hFileMap = CreateFileMapping(hFile
		, NULL
		, PAGE_READWRITE		// 注意，和createfile要一致
		, 0
		, dFileSize
		, NULL);


	if (hFileMap == NULL)
	{
		printf("创建映射句柄失败\n");
		CloseHandle(hFile);
		system("pause");
		return 1;
	}
	printf("创建映射句柄成功\n");

	CloseHandle(hFile);


	// 每次取500M 这个最好不要太大500M还是过大，这儿为了说明问题，当让如果是只读的话也没有什么问题
	DWORD dFileBlock = 1024 * 1024 * 512;


	DWORD dBegin = 0;
	DWORD dLen = 0;
	int count = 0;
	while (dBegin < dFileSize)
	{
		if (dBegin + dFileBlock > dFileSize)
		{
			dLen = dFileSize - dBegin;
		}
		else
		{
			dLen = dFileBlock;
		}


		// 取映射句柄的某一部分，并映射到内存中，注意这是虚拟内存，实际内存中并没有文件，也就是说不管多大文件，内存占用很小
		// 当然io（磁盘使用量可能会很大）
		LPVOID pFileBlock = MapViewOfFile(hFileMap
			, FILE_MAP_ALL_ACCESS		// 注意，和createfile要一致
			, 0
			, dBegin					// 开始位置
			, dLen);					// 映射长度


		if (pFileBlock == NULL)
		{
			printf("映射失败\n");
			CloseHandle(hFileMap);
			system("pause");
			return 1;
		}
		cout << "第" << count << "次映射成功." << endl;
		//printf("映射成功\n");

		
		while (true)
		{
			bool end = false;
			mut.lock();
			if (thread_num < 2)
				end = true;
			mut.unlock();
			if (end)
				break;
			Sleep(500);
		}

		// true，加密
		// false，解密
		thread t(foo, count, dLen, pFileBlock, true);
		t.detach();
		count++;

		//int key[] = { 1,2,3,4,5 };//加密字符		
		dBegin += dLen;
	}

	while (true)
	{
		bool end = false;
		mut.lock();
		if (thread_num == 0)
			end = true;
		mut.unlock();
		if (end)
			break;
		Sleep(500);
	}

	CloseHandle(hFileMap);
	printf("\nSuccess!\n");
	//system("pause");
	return 0;
}




//单个字符异或运算
char MakecodeChar(char c, int key) {
	return c = c ^ key;
}
//单个字符解密
char CutcodeChar(char c, int key) {
	return c ^ key;
}




//加密
void Makecode(char *pstr, int *pkey) {
	int len = strlen(pstr);//获取长度
	for (int i = 0;i < len;i++)
		*(pstr + i) = MakecodeChar(*(pstr + i), pkey[i % 5]);
}


//解密
void Cutecode(char *pstr, int *pkey) {
	int len = strlen(pstr);
	for (int i = 0;i < len;i++)
		*(pstr + i) = CutcodeChar(*(pstr + i), pkey[i % 5]);
}