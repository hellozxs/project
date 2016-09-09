#define _CRT_SECURE_NO_WARNINGS 1

#include <iostream>
#include <cstdlib>
#include<windows.h>

#include "FileCompress.h"
using namespace std;

void Test1()
{
	int start = GetTickCount();
	
	FileCompress fc;

	
	//fc.Compress("test/input.BIG");
	int  mid = GetTickCount();
	cout << "压缩时间：" << mid - start << endl;

	fc.UnCompress("test/input.BIG.compress");
	int end = GetTickCount();
	cout << "解压时间：" << end - mid << endl;
	

	
}



int main()
{
	Test1();
	
	system("pause");
	return 0;
}

