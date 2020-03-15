// socketTest.cpp : �������̨Ӧ�ó������ڵ㡣
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
	WSAStartup(MAKEWORD(2, 2), &wsaData);         //��ʼ��
	SOCKET  ListenSocket = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);  //����һ�������׽���    
																										//bind�Ȳ�������ʡ��....   //.....    
	FD_SET(ListenSocket, &ReadSet);      //���׽��ּ���ReadSet������    
	int isset = FD_ISSET(ListenSocket, &ReadSet);         //���ﲢû��ͨ��select��fd_set����ɸѡ   
	printf("Before select, isset = %d\n", isset);         //���������ӡ���Ϊ1 
	struct timeval tTime;
	tTime.tv_sec = 10;
	tTime.tv_usec = 0;
	select(0, &ReadSet, NULL, NULL, &tTime);       //ͨ��selectɸѡ���ھ���״̬��fd                                                  
												   //��ʱ���ղŵ�ListenSocket�����ھ���״̬(û����������)����ô�ʹ�ReadSet��ȥ����    
	isset = FD_ISSET(ListenSocket, &ReadSet);
	printf("After select, isset = %d\n", isset);     //���������ӡ�Ľ��Ϊ0 
	system("pause");
}

//������
int test_socket_server()
{
	int SERVER_PORT = 6789;
	WORD wVersionRequested;
	WSADATA wsaData;
	int ret, length;
	int recv_len = 0;
	SOCKET sListen, sServer; //�����׽��֣������׽���
	struct sockaddr_in saServer, saClient; //��ַ��Ϣ
	char *ptr;//���ڱ�����Ϣ��ָ��
			  //WinSock��ʼ��
	wVersionRequested = MAKEWORD(2, 2); //ϣ��ʹ�õ�WinSock DLL �İ汾
	ret = WSAStartup(wVersionRequested, &wsaData);
	if (ret != 0)
	{
		printf("WSAStartup() failed!\n");
		return -1;
	}
	//����Socket,ʹ��TCPЭ��
	sListen = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sListen == INVALID_SOCKET)
	{
		WSACleanup();
		printf("socket() faild!\n");
		return -1;
	}
	//�������ص�ַ��Ϣ
	saServer.sin_family = AF_INET; //��ַ����
	saServer.sin_port = htons(SERVER_PORT); //ע��ת��Ϊ�����ֽ���
	saServer.sin_addr.S_un.S_addr = htonl(INADDR_ANY); //ʹ��INADDR_ANY ָʾ�����ַ

													   //��
	ret = bind(sListen, (struct sockaddr *)&saServer, sizeof(saServer));
	if (ret == SOCKET_ERROR)
	{
		printf("bind() faild! code:%d\n", WSAGetLastError());
		closesocket(sListen); //�ر��׽���
		WSACleanup();
		return -1;
	}

	//������������
	ret = listen(sListen, 5);
	if (ret == SOCKET_ERROR)
	{
		printf("listen() faild! code:%d\n", WSAGetLastError());
		closesocket(sListen); //�ر��׽���
		return -1;
	}

	printf("Waiting for client connecting!\n");
	printf("Tips: Ctrl+c to quit!\n");
	//�����ȴ����ܿͻ�������
	while (1)//ѭ�������ͻ���
	{
		length = sizeof(saClient);
		sServer = accept(sListen, (struct sockaddr *)&saClient, &length);
		if (sServer == INVALID_SOCKET)
		{
			printf("accept() faild! code:%d\n", WSAGetLastError());
			closesocket(sListen); //�ر��׽���
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
			//��������
			recv_len = recv(sServer, ptr, 1024, 0);
			printf("===%d\n", recv_len);
			receiveMessage[recv_len] = 0;
			if (recv_len == SOCKET_ERROR)
			{
				printf("recv() failed!\n");
				return -1;
			}
			if (recv_len == 0) //�ͻ����Ѿ��ر�����
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
	//��ʼ��DLL
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA wsdata;
	if (WSAStartup(sockVersion, &wsdata) != 0)
	{
		return 1;
	}
	//�����׽���
	SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (serverSocket == INVALID_SOCKET)
	{
		cout << "Socket error" << endl;
		return 1;
	}
	//���׽���
	sockaddr_in sockAddr;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(8888);
	sockAddr.sin_addr.S_un.S_addr = INADDR_ANY;

	if (bind(serverSocket, (sockaddr*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR) {
		cout << "Bind error" << endl;
		return 1;
	}
	//��ʼ����
	if (listen(serverSocket, 10) == SOCKET_ERROR) {
		cout << "Listen error" << endl;
		return 1;
	}
	SOCKET clientSocket;
	sockaddr_in client_sin;
	char msg[100];//�洢���͵���Ϣ
	int flag = 0;//�Ƿ��Ѿ�������
	int len = sizeof(client_sin);
	while (true) 
	{
		if (!flag)
			cout << "�ȴ�����..." << endl;
		clientSocket = accept(serverSocket, (sockaddr*)&client_sin, &len);
		if (clientSocket == INVALID_SOCKET) {
			cout << "Accept error" << endl;
			flag = 0;
			return 1;
		}
		if (!flag)
			cout << "���յ�һ�����ӣ�" << inet_ntoa(client_sin.sin_addr) << endl;
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

//��bug ,δ���
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



