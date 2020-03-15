// socketTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <WinSock2.h>
#include <stdio.h>
#pragma comment(lib,"WS2_32.lib")
#include <iostream>
#include <string>
using namespace std;
#include "client.h"


void test_fd_set()
{
	FD_SET   ReadSet;
	FD_ZERO(&ReadSet);
	WSADATA   wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);         //初始化
	SOCKET  ListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);  //定义一个监听套接字    
																										//bind等操作这里省略....   //.....    
	FD_SET(ListenSocket, &ReadSet);      //将套接字加入ReadSet集合中    
	int isset = FD_ISSET(ListenSocket, &ReadSet);         //这里并没有通过select对fd_set进行筛选   
	printf("Before select, isset = %d\n", isset);         //所以这里打印结果为1 
	struct timeval tTime;
	tTime.tv_sec = 10;
	tTime.tv_usec = 0;
	select(0, &ReadSet, NULL, NULL, &tTime);       //通过select筛选处于就绪状态的fd                                                  
												   //这时，刚才的ListenSocket并不在就绪状态(没有连接连入)，那么就从ReadSet中去除它    
	isset = FD_ISSET(ListenSocket, &ReadSet);
	printf("After select, isset = %d\n", isset);     //所以这里打印的结果为0 
	system("pause");
}

//服务器
int test_socket_server()
{
	int SERVER_PORT = 6789;
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret, length;
	int recv_len = 0;
	SOCKET sListen, sServer; //侦听套接字，连接套接字
	struct sockaddr_in saServer, saClient; //地址信息
	char *ptr;//用于遍历信息的指针
			  //WinSock初始化
	wVersionRequested = MAKEWORD(2, 2); //希望使用的WinSock DLL 的版本
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStartup() failed!\n");
		return -1;
	}
	//创建Socket,使用TCP协议
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET)
	{
		WSACleanup();
		printf("socket() faild!\n");
		return -1;
	}
	//构建本地地址信息
	saServer.sin_family = AF_INET; //地址家族
	saServer.sin_port = htons(SERVER_PORT); //注意转化为网络字节序
	saServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //使用INADDR_ANY 指示任意地址

													   //绑定
	ret = bind(sListen, (struct sockaddr *)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		printf("bind() faild! code:%d\n", WSAGetLastError());
		closesocket(sListen); //关闭套接字
		WSACleanup();
		return -1;
	}

	//侦听连接请求
	ret = listen(sListen, 5);
	if (ret == SOCKET_ERROR)
	{
		printf("listen() faild! code:%d\n", WSAGetLastError());
		closesocket(sListen); //关闭套接字
		return -1;
	}

	printf("Waiting for client connecting!\n");
	printf("Tips: Ctrl+c to quit!\n");
	//阻塞等待接受客户端连接
	while (1)//循环监听客户端
	{
		length = sizeof(saClient);
		sServer = accept(sListen, (struct sockaddr *)&saClient, &length);
		if (sServer == INVALID_SOCKET)
		{
			printf("accept() faild! code:%d\n", WSAGetLastError());
			closesocket(sListen); //关闭套接字
			WSACleanup();
			return -1;
		}
		printf("accept is success!\n");
		printf("receive connect ip is %s ", inet_ntoa(saClient.sin_addr));
#if 0
		char receiveMessage[1024];
		ptr = (char *)&receiveMessage;
#else
		char *receiveMessage = (char *)calloc(2048, 0);
		ptr = receiveMessage;
#endif
		while (1)
		{
			//接收数据
			recv_len = recv(sServer, ptr, 1024, 0);
			printf("===%d\n", recv_len);
			receiveMessage[recv_len] = 0;
			if (recv_len == SOCKET_ERROR)
			{
				printf("recv() failed!\n");
				return -1;
			}
			if (recv_len == 0) //客户端已经关闭连接
			{
				printf("Client has closed the connection\n");
				break;
			}
			char * sendData = "hello callback send data";
			send(sServer, sendData, strlen(sendData), 0);
			printf("recv is %s\n", ptr);
			memset(receiveMessage, 0, sizeof(receiveMessage));
		}
	}
}

int test_socket_server1()
{
	//初始化DLL
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsdata;
	if (WSAStartup(sockVersion, &wsdata) != 0)
	{
		return 1;
	}
	//创建套接字
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		cout << "Socket error" << endl;
		return 1;
	}
	//绑定套接字
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(8888);
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(serverSocket, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
		cout << "Bind error" << endl;
		return 1;
	}
	//开始监听
	if (listen(serverSocket, 10) == SOCKET_ERROR) {
		cout << "Listen error" << endl;
		return 1;
	}
	SOCKET clientSocket;
	sockaddr_in client_sin;
	char msg[100];//存储传送的消息
	int flag = 0;//是否已经连接上
	int len = sizeof(client_sin);
	while (true) 
	{
		if (!flag)
			cout << "等待连接..." << endl;
		clientSocket = accept(serverSocket, (sockaddr*)&client_sin, &len);
		if (clientSocket == INVALID_SOCKET) {
			cout << "Accept error" << endl;
			flag = 0;
			return 1;
		}
		if (!flag)
			cout << "接收到一个链接：" << inet_ntoa(client_sin.sin_addr) << endl;
		flag = 1;
		break;
	}	
	while (true)
	{
		int num = recv(clientSocket, msg, 100, 0);
		if (num > 0)
		{
			msg[num] = '\0';
			cout << "Client say: " << msg << endl;
		}
		string data;
		getline(cin, data);
		const char * sendData;
		sendData = data.c_str();
		send(clientSocket, sendData, strlen(sendData), 0);
	}
	closesocket(serverSocket);
	WSACleanup();
}

int test_socket_client()
{
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return 1;
	}

	while (true) {
		SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (clientSocket == INVALID_SOCKET) {
			cout << "Socket error" << endl;
			return 1;
		}
		sockaddr_in sock_in;
		sock_in.sin_family = AF_INET;
		sock_in.sin_port = htons(8888);
		sock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
		if (connect(clientSocket, (sockaddr*)&sock_in, sizeof(sock_in)) == SOCKET_ERROR) {
			cout << "Connect error" << endl;
			return 1;
		}
		while (true)
		{
			string data;
		getline(cin, data);
		const char * msg;
		msg = data.c_str();
		send(clientSocket, msg, strlen(msg), 0);

		char revdata[100];
		int num = recv(clientSocket, revdata, 100, 0);
		if (num > 0) {
			revdata[num] = '\0';
			cout << "Sever say:" << revdata << endl;
		}
		}
		closesocket(clientSocket);

	}

	WSACleanup();
}

//有bug ,未解决
void test_client_class()
{
	SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	sockaddr_in sock_in;
	sock_in.sin_family = AF_INET;
	sock_in.sin_port = htons(8888);
	sock_in.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	CClient *Client = new CClient(clientSocket, sock_in);
	Client->connectSocket();
	Client->startRunning();
}
int main()
{
	//test_fd_set();// test ok
	//test_socket_server();// test ok
	//test_socket_server1();// test ok
	test_socket_client();	// test ok
	//test_client_class();
    return 0;
}



