// DllTest.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"


#define DLL_API __declspec(dllexport)

#include "DllTest.h"

int add(int a, int b)
{
	return a + b;
}