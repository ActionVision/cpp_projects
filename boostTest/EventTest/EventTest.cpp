// EventTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "conio.h"
#include <iostream>
using namespace std;
//signalEvent_test
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

//MultisignalEvent_test
HANDLE events[3];
DWORD WINAPI work_receive(LPVOID P)
{
	while (1)
	{
		//当只要有一个事件进来就返回，设置为false,
		DWORD ret = WaitForMultipleObjects(3, events, false, INFINITE);
		switch (ret)
		{
		case WAIT_OBJECT_0+0:
			cout << "get event 0\n" << endl;
			ResetEvent(events[0]);
			break;
		case WAIT_OBJECT_0 + 1:
			cout << "get event 1\n" << endl;
			ResetEvent(events[1]);
			break;
		case WAIT_OBJECT_0 + 2:
			cout << "get event 2\n" << endl;
			ResetEvent(events[2]);
			break;
		case WAIT_TIMEOUT:
			break;
		default:
			break;
		}
		Sleep(1);
	}
}
DWORD WINAPI work_send(LPVOID P)
{
	int i = 0;
	while (1)
	{
		i++;
		if (i % 3 == 0)
		{
			SetEvent(events[0]);
			cout << "send event0" << endl;
		}
		if (i % 3 == 1)
		{
			SetEvent(events[1]);
			cout << "send event1" << endl;
		}
		if (i % 3 == 2)
		{
			SetEvent(events[2]);
			cout << "send event2" << endl;
		}
		Sleep(1000);
	}
}
void MultisignalEvent_test()
{
	events[0] = CreateEvent(NULL, false, false, NULL);
	events[1] = CreateEvent(NULL, false, false, NULL);
	events[2] = CreateEvent(NULL, false, false, NULL);
	HANDLE H1 = CreateThread(NULL, 0, work_receive, NULL, 0, NULL);
	HANDLE H2 = CreateThread(NULL, 0, work_send, NULL, 0, NULL);
	Sleep(INFINITE);
}

//wait all event come
DWORD WINAPI work_receive1(LPVOID P)
{
	while (1)
	{
		//要等待所有的事件都达到，设置为true,
		DWORD ret = WaitForMultipleObjects(3, events, true, INFINITE);
		switch (ret)
		{
		case WAIT_OBJECT_0 + 0:
			cout << "get event 012\n" << endl;
			ResetEvent(events[0]);
			ResetEvent(events[1]);
			ResetEvent(events[2]);
		case WAIT_TIMEOUT:
			break;
		default:
			break;
		}
		Sleep(1);
	}
}
DWORD WINAPI work_send1(LPVOID P)
{
	int i = 0;
	while (1)
	{
		i++;
		if (i % 3 == 0)
		{
			SetEvent(events[0]);
			SetEvent(events[1]);
			SetEvent(events[2]);
			cout << "send event012" << endl;
		}
		if (i % 3 == 1)
		{
			SetEvent(events[1]);
			cout << "send event1" << endl;
		}
		if (i % 3 == 2)
		{
			SetEvent(events[2]);
			cout << "send event2" << endl;
		}
		Sleep(1000);
	}
}
void MultisignalEvent_test1()
{
	events[0] = CreateEvent(NULL, false, false, NULL);
	events[1] = CreateEvent(NULL, false, false, NULL);
	events[2] = CreateEvent(NULL, false, false, NULL);
	HANDLE H1 = CreateThread(NULL, 0, work_receive1, NULL, 0, NULL);
	HANDLE H2 = CreateThread(NULL, 0, work_send1, NULL, 0, NULL);
	Sleep(INFINITE);
}
int main()
{
	//signalEvent_test();
	//MultisignalEvent_test();
	MultisignalEvent_test1();
	return 0;
}
 