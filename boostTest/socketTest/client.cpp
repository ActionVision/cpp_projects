#include "client.h"


CClient::CClient(void)
{
}

CClient::CClient(SOCKET s, sockaddr_in addr)
{//初始化各成员变量。
	m_socket = s;
	m_addr = addr;
	m_hRecvThread = NULL;
	m_hSendThread = NULL;
	m_IsConnected = true;
	m_IsSendData = true;
	m_hEvent = CreateEvent(NULL, true, false, NULL);
	m_pRecvData = new char[1024];
	m_pSendData = new char[1024];
	memset(m_pSendData, 0, 1024);
	memset(m_pRecvData, 0, 1024);
}


CClient::~CClient(void)
{
	delete[]m_pRecvData;
	delete[]m_pSendData;
}

bool CClient::IsConnected()
{
	return m_IsConnected;
}


bool CClient::calc()
{
	time_t t;
	struct tm *local;
	char T[256];
	memset(T, 0, 256);
	t = time(NULL);
	local = localtime(&t);
	sprintf(T, "%d/%d/%d %d:%d:%d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
	strcpy(m_pSendData, T);
	return true;
}

DWORD WINAPI CClient::sendThread(void*param)//发送线程入口函数。
{
	std::cout << "发送数据线程开始运行！！" << std::endl;
	CClient *pClient = static_cast<CClient*>(param);//获得CClient对象指针。以便操纵成员变量。
	WaitForSingleObject(pClient->m_hEvent, INFINITE);//等待接收数据线程通知。
	while (pClient->m_IsConnected)
	{
		while (pClient->m_IsSendData)//可以发送数据。
		{
			std::cout << "等待接收数据线程通知！！" << std::endl;
			//
			//ResetEvent(pClient->m_hEvent);
			int ret = send(pClient->m_socket, pClient->m_pSendData, 1024, 0);
			if (ret == SOCKET_ERROR)
			{
				int r = WSAGetLastError();
				if (r == WSAEWOULDBLOCK)
				{
					continue;
				}
				else
				{
					return 0;
				}
			}
			else
			{
				std::cout << "结果发送成功！！" << std::endl;
				pClient->m_IsSendData = false;
				break;
			}

		}
		Sleep(1000);//未收到发送通知，睡眠1秒。

	}
}

DWORD WINAPI CClient::recvThread(void*param)//接收数据线程入口函数。
{
	std::cout << "接收数据线程开始运行！！" << std::endl;
	CClient *pClient = static_cast<CClient*>(param);
	while (pClient->m_IsConnected)
	{
		memset(pClient->m_pRecvData, 0, 1024);
		int ret = recv(pClient->m_socket, pClient->m_pRecvData, 1024, 0);
		if (ret == SOCKET_ERROR)
		{
			int r = WSAGetLastError();
			if (r == WSAEWOULDBLOCK)
			{
				//std::cout<<"没有收到来自客户端的数据！！"<<std::endl;
				Sleep(20);
				continue;
			}
			else if (r == WSAENETDOWN)
			{
				std::cout << "接收数据线程出现错误,连接中断！" << std::endl;
				break;
			}
			else
			{
				std::cout << "接收数据线程出现错误！" << std::endl;
				break;
			}
		}
		else
		{
			std::cout << "恭喜，收到来自客户端的数据:" << pClient->m_pRecvData << std::endl;
			pClient->calc();
			std::cout << "通知发送线程发送结果！！" << std::endl;
			SetEvent(pClient->m_hEvent);
			pClient->m_IsSendData = true;
		}
	}
	return 0;
}
bool CClient::startRunning()//开始为连接创建发送和接收线程。
{
	m_hRecvThread = CreateThread(NULL, 0, recvThread, (void*)this, 0, NULL);//由于static成员函数，无法访问类成员。因此传入this指针。
	if (m_hRecvThread == NULL)
	{
		return false;
	}
	m_hSendThread = CreateThread(NULL, 0, sendThread, (void*)this, 0, NULL);
	if (m_hSendThread == NULL)
	{
		return false;
	}
	return true;

}

bool CClient::DisConnect()
{
	m_IsConnected = false;//接收和发送线程退出。资源释放交由资源释放线程。
	return true;
}
void CClient::connectSocket()
{
	connect(m_socket, (sockaddr*)&m_addr, sizeof(m_addr));


}
