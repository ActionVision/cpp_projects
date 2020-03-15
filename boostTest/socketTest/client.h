#pragma once

#pragma once
#include<iostream>
#include"windows.h"
#include"time.h"
class CClient
{
public:
	CClient(void);
	CClient(SOCKET s, sockaddr_in addr);
	~CClient(void);
public:
	bool IsConnected();//判断连接是否中断。
	bool DisConnect();//中断与服务器的连接。
	bool calc();//计算当前时间，并复制到发送缓冲区内。
	bool startRunning();//开始运行发送和接收线程。
	static DWORD WINAPI sendThread(void*param);//发送线程入口函数。
	static DWORD WINAPI recvThread(void*param);//接收线程入口函数。
	void connectSocket();
private:
	HANDLE m_hSendThread;//发送线程句柄。
	HANDLE m_hRecvThread;//接受线程句柄。
	HANDLE m_hEvent;//发送线程和接收线程同步事件对象。接收客户端请求后通知发送线程发送当前时间。
	SOCKET m_socket;//与客户端连接套接字。
	sockaddr_in m_addr;//客户端地址。
	bool m_IsConnected;
	char *m_pRecvData;//接收缓冲区。
	char *m_pSendData;//发送缓冲区。
	bool m_IsSendData;//由于只有接收到客户端请求后才需要发送，该变量控制是否发送数据。
};


