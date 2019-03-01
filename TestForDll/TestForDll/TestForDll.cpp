// TestForDll.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "DllTest.h"
#include "interface.h"
//#pragma comment(lib,"DllTest.lib")

int _tmain(int argc, _TCHAR* argv[])
{
	int a = 3;
	int b = 2;
	int c=add(a, b);
	c = add1(a, b);
	return 0;
}

