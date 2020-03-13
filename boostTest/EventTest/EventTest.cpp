// EventTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "conio.h"
#include <iostream>
using namespace std;

HANDLE event1;

DWORD WINAPI FUN1(LPVOID P)
{
	while (1)
	{
		WaitForSingleObject(event1, INFINITE);
		cout << "fun1 get event..\n" << endl;
		Sleep(1);
	}
	return 0;
}

DWORD WINAPI FUN3(LPVOID P)
{
	int i = 0;
	while (1)
	{
		i++;
		if (i <= 10)
		{
			cout << i << "::fun3  set event " << endl;
			SetEvent(event1);
			Sleep(1000);
		}
		if(i==10)
		{
			ResetEvent(event1);
			cout <<i<< "::fun3 event reset" << endl;
			Sleep(1000);
		}
		if (i > 10)
		{
			cout << i<<"::fun3 sleep" << endl;
			Sleep(1000);
		}

	}
	return 0;
}

void signalEvent_test()
{
	event1 = CreateEvent(NULL, false, true, NULL);
	HANDLE H1 = CreateThread(NULL, 0, FUN1, NULL, 0, NULL);
	HANDLE H3 = CreateThread(NULL, 0, FUN3, NULL, 0, NULL);
	Sleep(INFINITE);
}

int main()
{
	//signalEvent_test();

	return 0;
}
 