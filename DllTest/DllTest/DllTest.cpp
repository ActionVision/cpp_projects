// DllTest.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"


#define DLL_API __declspec(dllexport)

#include "DllTest.h"

int add(int a, int b)
{
	return a + b;
}