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
	cout << "��" << i << "���̣߳���" << thread_num << "���̡߳�" << endl;
	for (int i = 0; i < dLen; ++i)
	{
		// �򵥴������һ�£����Լ��ܣ�����Ķ��ǲ�ռ�����ڴ��
		char* a = (char*)pFileBlock + i;

		// ע�������д���ڴ棬��ô����ڴ潫��ʵ�ʻ���Ҳ����˵����ռ�õ��ڴ������ˣ�һ����512M����dFileBlock���Сһ��
		//*a = *a ^ 85;
		if (en)
			*a = MakecodeChar(*a, 1);//����
		else
			*a = CutcodeChar(*a, 1);//����
		//*a = 'a';
	}
	// ���ͷš��������ڴ�
	UnmapViewOfFile(pFileBlock);

	mut.lock();
	thread_num--;
	mut.unlock();
}

bool FileEncryption(fstring fsSrcPath, fstring fsDesPath)
{
	//��ȡģʽ
	DWORD dAccessType = (GENERIC_READ | GENERIC_WRITE);


	//����ģʽ
	DWORD dShareType = FILE_SHARE_READ | FILE_SHARE_WRITE;


	// ���ļ����
	HANDLE hFile = CreateFile(fsSrcPath.c_str()
		, dAccessType				// ����ģʽ
		, dShareType				// ����ģʽ
		, NULL
		, OPEN_EXISTING				// ���������򿪣�����ʧ��
		, FILE_FLAG_OVERLAPPED		// ������
		, NULL);


	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("�ļ�������");
		system("pause");
		return 1;
	}
	printf("�����ļ��ɹ�\n");

	DWORD dFileSize = GetFileSize(hFile, NULL);


	// ����ӳ����
	HANDLE hFileMap = CreateFileMapping(hFile
		, NULL
		, PAGE_READWRITE		// ע�⣬��createfileҪһ��
		, 0
		, dFileSize
		, NULL);


	if (hFileMap == NULL)
	{
		printf("����ӳ����ʧ��\n");
		CloseHandle(hFile);
		system("pause");
		return 1;
	}
	printf("����ӳ�����ɹ�\n");

	CloseHandle(hFile);


	// ÿ��ȡ500M �����ò�Ҫ̫��500M���ǹ������Ϊ��˵�����⣬���������ֻ���Ļ�Ҳû��ʲô����
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


		// ȡӳ������ĳһ���֣���ӳ�䵽�ڴ��У�ע�����������ڴ棬ʵ���ڴ��в�û���ļ���Ҳ����˵���ܶ���ļ����ڴ�ռ�ú�С
		// ��Ȼio������ʹ�������ܻ�ܴ�
		LPVOID pFileBlock = MapViewOfFile(hFileMap
			, FILE_MAP_ALL_ACCESS		// ע�⣬��createfileҪһ��
			, 0
			, dBegin					// ��ʼλ��
			, dLen);					// ӳ�䳤��


		if (pFileBlock == NULL)
		{
			printf("ӳ��ʧ��\n");
			CloseHandle(hFileMap);
			system("pause");
			return 1;
		}
		cout << "��" << count << "��ӳ��ɹ�." << endl;
		//printf("ӳ��ɹ�\n");

		
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

		// true������
		// false������
		thread t(foo, count, dLen, pFileBlock, true);
		t.detach();
		count++;

		//int key[] = { 1,2,3,4,5 };//�����ַ�		
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




//�����ַ��������
char MakecodeChar(char c, int key) {
	return c = c ^ key;
}
//�����ַ�����
char CutcodeChar(char c, int key) {
	return c ^ key;
}




//����
void Makecode(char *pstr, int *pkey) {
	int len = strlen(pstr);//��ȡ����
	for (int i = 0;i < len;i++)
		*(pstr + i) = MakecodeChar(*(pstr + i), pkey[i % 5]);
}


//����
void Cutecode(char *pstr, int *pkey) {
	int len = strlen(pstr);
	for (int i = 0;i < len;i++)
		*(pstr + i) = CutcodeChar(*(pstr + i), pkey[i % 5]);
}