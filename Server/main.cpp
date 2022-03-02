//// socket.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
////
//
//#include "pch.h"
//#include <iostream>
//
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <stdlib.h>
//#include <WinSock2.h>
//#pragma comment(lib, "ws2_32.lib")  //加载 ws2_32.dll
//
//
//using namespace std;
//int main() {
//	//初始化DLL
//	WSADATA wsaData;
//	WSAStartup(MAKEWORD(2, 2), &wsaData);
//	//创建套接字
//	SOCKET sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
//	//向服务器发起请求
//	struct sockaddr_in sockAddr;
//	memset(&sockAddr, 0, sizeof(sockAddr));  //每个字节都用0填充
//	sockAddr.sin_family = PF_INET;
//	sockAddr.sin_addr.s_addr = inet_addr("47.109.85.214");
//	sockAddr.sin_port = htons(63543);
//	int conn = connect(sock, (SOCKADDR*)&sockAddr, sizeof(SOCKADDR));
//	cout << "conn: " << conn << endl;
//	//接收服务器传回的数据
//	char szBuffer[MAXBYTE] = { 0 };
//	recv(sock, szBuffer, MAXBYTE, NULL);
//	//输出接收到的数据
//	cout << "Message form server: " << szBuffer;
//	//关闭套接字
//	closesocket(sock);
//	//终止使用 DLL
//	WSACleanup();
//	system("pause");
//	return 0;
//}


#include "Game/24.h"
#include <iostream>

int main()
{
	Calculate24::getInstance()->init();
	//while (Calculate24::getInstance()->_isRunning);
	//time.sleep(2);
	//Calculate24::getInstance()->destoryInstance();
	std::cout << "stop server" << std::endl;
	return 0;
}